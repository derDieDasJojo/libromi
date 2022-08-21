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
#include "picamera/VideoEncoder.h"

namespace romi::arm {

        VideoEncoder::VideoEncoder(PiCameraSettings& settings)
                : EncoderComponent(MMAL_COMPONENT_DEFAULT_VIDEO_ENCODER),
                  last_frame_(),
                  condition_(),
                  mutex_(),
                  wants_frame_(false),
                  has_frame_(false)
        {
                set_format(settings);
                set_immutable();
                enable();
                create_header_pool();
        }
        
        VideoEncoder::~VideoEncoder()
        {
        }

        void VideoEncoder::set_format(PiCameraSettings& settings)
        {
                MMAL_STATUS_T status;
                
                // We want same format on input and output
                mmal_format_copy(output_->format, input_->format);

                // Specify the output format
                output_->format->encoding = MMAL_ENCODING_MJPEG;

                if (settings.bitrate_ > kMaxBitrateMJPEG)
                        output_->format->bitrate = kMaxBitrateMJPEG;
                else
                        output_->format->bitrate = settings.bitrate_;
                
                output_->buffer_size = 512 * 1024;

                if (output_->buffer_size < output_->buffer_size_min)
                        output_->buffer_size = output_->buffer_size_min;

                output_->buffer_num = output_->buffer_num_recommended;

                if (output_->buffer_num < output_->buffer_num_min)
                        output_->buffer_num = output_->buffer_num_min;

                // We need to set the frame rate on output to 0, to ensure it gets
                // updated correctly from the input framerate when port connected
                output_->format->es->video.frame_rate.num = 0;
                output_->format->es->video.frame_rate.den = 1;

                status = mmal_port_format_commit(output_);
                assert_status("set_format", status);
        }

        void VideoEncoder::set_immutable()
        {
                MMAL_STATUS_T status;                
                status = mmal_port_parameter_set_boolean(input_,
                                                         MMAL_PARAMETER_VIDEO_IMMUTABLE_INPUT,
                                                         1);
                assert_status("set_immutable", status);
        }
        
        void VideoEncoder::wait_capture()
        {
                std::unique_lock<std::mutex> sync(mutex_);
                //r_debug("wait_capture: start");
                wants_frame_ = true;
                condition_.wait(sync,
                                [&] {
                                        // r_debug("wait_capture: has_frame=%d",
                                        //         (int) has_frame_);
                                        return has_frame_;
                                });
                has_frame_ = false;
                //r_debug("wait_capture: done");
        }

        void VideoEncoder::create_header_pool()
        {
                /* Create pool of buffer headers for the output port to consume */
                pool_ = mmal_port_pool_create(output_, output_->buffer_num,
                                              output_->buffer_size);
                if (!pool_) {
                        r_err("Failed to create buffer header pool for "
                              "encoder output port %s", output_->name);
                        throw std::runtime_error("VideoEncoder::"
                                                 "create_header_pool failed");
                }
        }

        void VideoEncoder::encoder_output_callback(MMAL_PORT_T *port,
                                                   MMAL_BUFFER_HEADER_T *buffer)
        {
                VideoEncoder *encoder = (VideoEncoder *) port->userdata;
                encoder->handle_output_callback(buffer);
        }

        void VideoEncoder::handle_output_callback(MMAL_BUFFER_HEADER_T *buffer)
        {
                copy_buffer(buffer);
                notify_client();
                send_buffer();
        }
        
        void VideoEncoder::copy_buffer(MMAL_BUFFER_HEADER_T *buffer)
        {
                std::lock_guard<std::mutex> sync(mutex_);
                mmal_buffer_header_mem_lock(buffer);

                // if (buffer->flags & MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED) {
                //         complete = true;
                //         capture_failed_ = true;
                // }
                
                if (buffer->length) {
                        if (wants_frame_) {
                                buffer_.clear();
                                buffer_.append(buffer->data, buffer->length);
                                wants_frame_ = false;
                                has_frame_ = true;
                        } else {
                                //r_debug("Frame dropped");
                        }
                }                        
                
                mmal_buffer_header_mem_unlock(buffer);
                mmal_buffer_header_release(buffer);
        }
        
        void VideoEncoder::notify_client()
        {
                if (has_frame_) {
                        condition_.notify_one();
                }
        }
        
        void VideoEncoder::send_buffer()
        {
                MMAL_STATUS_T status;

                if (output_->is_enabled) {
                        MMAL_BUFFER_HEADER_T *buffer;
                        buffer = mmal_queue_get(pool_->queue);
                        if (buffer) {
                                status = mmal_port_send_buffer(output_, buffer);
                        } else {
                                r_warn("VideoEncoder: mmal_queue_get failed");
                        }
                        if (!buffer || status != MMAL_SUCCESS)
                                r_err("Unable to return a buffer "
                                        "to the encoder port");
                }
        }
        
        void VideoEncoder::enable_output()
        {
                MMAL_STATUS_T status;
                output_->userdata = (struct MMAL_PORT_USERDATA_T *) this;
                status = mmal_port_enable(output_, VideoEncoder::encoder_output_callback);
                assert_status("VideoEncoder::enable_output", status);
        }

        void VideoEncoder::disable_output()
        {
                MMAL_STATUS_T status;
                status = mmal_port_disable(output_);
                assert_status("VideoEncoder::disable_output", status);
        }

        void VideoEncoder::send_buffers()
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

        bool VideoEncoder::set_jpeg_quality(uint32_t value)
        {
                (void) value;
                r_info("VideoEncoder::set_jpeg_quality has no effect. "
                       "Use the bitrate instead");
                return false;
        }
}

