/*
  libromi

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  libromi is the base library for the Romi devices.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */
#include <stdexcept>
#include <r.h>
#include "picamera/ImageEncoder.h"

namespace romi::arm {

        ImageEncoder::ImageEncoder()
                : EncoderComponent(MMAL_COMPONENT_DEFAULT_IMAGE_ENCODER),
                  semaphore_(),
                  capture_failed_(false)
        {
                set_format();
                set_jpeg_quality();
                disable_jpeg_restart();
                disable_thumbnail();
                enable();
                create_header_pool();
        }
        
        ImageEncoder::~ImageEncoder()
        {
        }

        void ImageEncoder::set_format()
        {
                MMAL_STATUS_T status;
                
                // We want same format on input and output
                mmal_format_copy(output_->format, input_->format);

                // Specify the output format
                output_->format->encoding = MMAL_ENCODING_JPEG;
                
                output_->buffer_size = output_->buffer_size_recommended;
                if (output_->buffer_size < output_->buffer_size_min)
                        output_->buffer_size = output_->buffer_size_min;

                output_->buffer_num = output_->buffer_num_recommended;
                if (output_->buffer_num < output_->buffer_num_min)
                        output_->buffer_num = output_->buffer_num_min;

                status = mmal_port_format_commit(output_);
                assert_status("set_format", status);
        }

        void ImageEncoder::set_jpeg_quality()
        {
                MMAL_STATUS_T status;                
                status = mmal_port_parameter_set_uint32(output_,
                                                        MMAL_PARAMETER_JPEG_Q_FACTOR,
                                                        85);
                assert_status("set_jpeg_quality", status);
        }

        void ImageEncoder::disable_jpeg_restart()
        {
                MMAL_STATUS_T status;                
                status = mmal_port_parameter_set_uint32(output_,
                                                        MMAL_PARAMETER_JPEG_RESTART_INTERVAL,
                                                        0);
                assert_status("disable_jpeg_restart", status);
        }

        void ImageEncoder::prepare_capture()
        {
                capture_failed_ = false;
                buffer_.clear();
                disable_exif();
                enable_output();
                send_buffers();
        }

        void ImageEncoder::finish_capture()
        {
                wait_capture();
                disable_output();
                assert_successful_capture();
        }

        void ImageEncoder::assert_successful_capture()
        {
                if (capture_failed_)
                        throw std::runtime_error("Capture failed");                
        }

        void ImageEncoder::disable_thumbnail()
        {
                MMAL_STATUS_T status;
                MMAL_PARAMETER_THUMBNAIL_CONFIG_T param_thumb = {
                        { MMAL_PARAMETER_THUMBNAIL_CONFIGURATION,
                          sizeof(MMAL_PARAMETER_THUMBNAIL_CONFIG_T)},
                        0, 0, 0, 0
                };
                status = mmal_port_parameter_set(ptr_->control, &param_thumb.hdr);
                assert_status("disable_thumbnail", status);
        }

        void ImageEncoder::create_header_pool()
        {
                /* Create pool of buffer headers for the output port to consume */
                pool_ = mmal_port_pool_create(output_, output_->buffer_num,
                                              output_->buffer_size);
                if (!pool_) {
                        r_err("Failed to create buffer header pool for "
                              "encoder output port %s", output_->name);
                        throw std::runtime_error("ImageEncoder::"
                                                 "create_header_pool failed");
                }
        }

        void ImageEncoder::disable_exif()
        {
                mmal_port_parameter_set_boolean(output_,
                                                MMAL_PARAMETER_EXIF_DISABLE,
                                                1);
        }
        
        void ImageEncoder::post_capture()
        {
                semaphore_.post();
        }
        
        void ImageEncoder::wait_capture()
        {
                semaphore_.wait();
        }

        void ImageEncoder::encoder_output_callback(MMAL_PORT_T *port,
                                                       MMAL_BUFFER_HEADER_T *buffer)
        {
                ImageEncoder *encoder = (ImageEncoder *) port->userdata;
                encoder->handle_output_callback(buffer);
        }
        
        void ImageEncoder::enable_output()
        {
                MMAL_STATUS_T status;
                output_->userdata = (struct MMAL_PORT_USERDATA_T *) this;
                status = mmal_port_enable(output_, ImageEncoder::encoder_output_callback);
                assert_status("ImageEncoder::enable_output", status);
        }

        void ImageEncoder::disable_output()
        {
                MMAL_STATUS_T status;
                status = mmal_port_disable(output_);
                assert_status("ImageEncoder::disable_output", status);
        }

        void ImageEncoder::send_buffers()
        {
                // Send all the buffers to the encoder output port
                unsigned int num = mmal_queue_length(pool_->queue);
                MMAL_STATUS_T status;

                for (unsigned int q = 0; q < num; q++) {
                        MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get(pool_->queue);
                        if (buffer) {
                                status = mmal_port_send_buffer(output_, buffer);
                                if (status != MMAL_SUCCESS)
                                        r_err("Unable to send a "
                                                "buffer to encoder");
                        } else {
                                r_err("Unable to get buffer %u from queue", q);
                        }
                }
        }

        void ImageEncoder::handle_output_callback(MMAL_BUFFER_HEADER_T *buffer)
        {
                MMAL_STATUS_T status;
                bool complete = 0;

                mmal_buffer_header_mem_lock(buffer);
                buffer_.append(buffer->data, buffer->length);
                mmal_buffer_header_mem_unlock(buffer);

                if (buffer->flags & MMAL_BUFFER_HEADER_FLAG_FRAME_END)
                        complete = true;
                
                if (buffer->flags & MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED) {
                        complete = true;
                        capture_failed_ = true;
                }

                mmal_buffer_header_release(buffer);

                if (output_->is_enabled) {
                        MMAL_BUFFER_HEADER_T *new_buffer;
                        new_buffer = mmal_queue_get(pool_->queue);
                        if (new_buffer) {
                                status = mmal_port_send_buffer(output_, new_buffer);
                        }
                        if (!new_buffer || status != MMAL_SUCCESS)
                                r_err("Unable to return a buffer "
                                        "to the encoder port");
                }

                if (complete)
                        post_capture();
        }
}

