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
#include "picamera/VideoCamera.h"

namespace romi {
        
        VideoCamera::VideoCamera(PiCameraSettings& settings)
                : camera_(settings),
                  encoder_(settings),
                  encoder_connection_(camera_.get_video_port(),
                                      encoder_.get_input_port())
        {
                encoder_.enable_output();
                encoder_.send_buffers();
                camera_.start_video();
        }

        VideoCamera::~VideoCamera()
        {
                encoder_.disable_output();
        }
        
        bool VideoCamera::grab(romi::Image &image)
        {
                (void) image;
                r_err("VideoCamera::grab not implemented, yet");
                return false;
        }

        rpp::MemBuffer& VideoCamera::grab_jpeg()
        {
                encoder_.wait_capture();
                return encoder_.get_buffer();
        }

        bool VideoCamera::power_up()
        {
                return true;
        }
        
        bool VideoCamera::power_down()
        {
                return true;
        }
        
        bool VideoCamera::stand_by()
        {
                return true;
        }
        
        bool VideoCamera::wake_up()
        {
                return true;
        }
}

