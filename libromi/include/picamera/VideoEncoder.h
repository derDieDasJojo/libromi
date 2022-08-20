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
#ifndef _LIBROMI_VIDEO_ENCODER_
#define _LIBROMI_VIDEO_ENCODER_

#include <stdint.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <MemBuffer.h>

#include "picamera/Component.h"
#include "picamera/EncoderComponent.h"
#include "picamera/PiCameraSettings.h"

namespace romi::arm {
        
        class VideoEncoder : public EncoderComponent
        {
        protected:
                rcom::MemBuffer last_frame_;
                std::condition_variable condition_;
                std::mutex mutex_;
                bool wants_frame_;
                bool has_frame_;
                
        public:
                VideoEncoder(PiCameraSettings& settings);
                ~VideoEncoder() override;
                
                void wait_capture();
                void enable_output();
                void disable_output();
                void send_buffers();
                
                bool set_jpeg_quality(uint32_t value) override;
                
        protected:

                static void encoder_output_callback(MMAL_PORT_T *port,
                                                    MMAL_BUFFER_HEADER_T *buffer);
                void handle_output_callback(MMAL_BUFFER_HEADER_T *buffer);

                void set_format(PiCameraSettings& settings);
                void set_immutable();
                void create_header_pool();
                void send_buffer();
                void copy_buffer(MMAL_BUFFER_HEADER_T *buffer);
                void notify_client();

        private:
                VideoEncoder(VideoEncoder& other) = default;
                VideoEncoder& operator=(const VideoEncoder& other) = default;
        };
}

#endif // _LIBROMI_VIDEO_ENCODER_
