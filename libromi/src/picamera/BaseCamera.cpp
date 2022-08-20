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
#include "picamera/BaseCamera.h"

namespace romi {
        
        BaseCamera::BaseCamera(PiCameraSettings& settings)
                : camera_(settings)
        {
        }

        BaseCamera::~BaseCamera()
        {
        }

        bool BaseCamera::set_saturation(int32_t saturation)
        {
                bool result = true;
                camera_.set_saturation(saturation);
                return result;
        }
        
        bool BaseCamera::set_sharpness(int32_t sharpness)
        {
                bool result = true;
                camera_.set_sharpness(sharpness);
                return result;
        }
        
        bool BaseCamera::set_contrast(int32_t contrast)
        {
                bool result = true;
                camera_.set_contrast(contrast);
                return result;
        }
        
        bool BaseCamera::set_brightness(int32_t brightness)
        {
                bool result = true;
                camera_.set_brightness(brightness);
                return result;
        }
        
        bool BaseCamera::set_iso(uint32_t iso)
        {
                bool result = true;
                camera_.set_iso(iso);
                return result;
        }
}

