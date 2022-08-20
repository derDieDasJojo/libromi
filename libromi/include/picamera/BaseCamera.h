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
#ifndef _LIBROMI_BASECAMERA_H_
#define _LIBROMI_BASECAMERA_H_

#include "picamera/PiCameraIncludes.h"
#include "picamera/PiCameraSettings.h"
#include "picamera/CameraComponent.h"
#include "cv/Image.h"
#include "MemBuffer.h"

namespace romi {
        
        class BaseCamera
        {
        protected:

                arm::CameraComponent camera_;
                
        public:
                BaseCamera(PiCameraSettings& settings);
                virtual ~BaseCamera();
                
                virtual bool grab(Image &image) = 0;
                virtual rcom::MemBuffer& grab_jpeg() = 0;                
                virtual void set_jpeg_quality(uint32_t value) = 0;
                
                bool set_saturation(int32_t saturation);
                bool set_sharpness(int32_t sharpness);
                bool set_contrast(int32_t contrast);
                bool set_brightness(int32_t brightness);
                bool set_iso(uint32_t iso);
        };
}

#endif // _LIBROMI_BASECAMERA_H_
