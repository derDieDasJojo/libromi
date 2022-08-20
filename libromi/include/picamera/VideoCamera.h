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
#ifndef _LIBROMI_VIDEO_CAMERA_H_
#define _LIBROMI_VIDEO_CAMERA_H_

//#include "picamera/PiCamera.h"
#include "picamera/BaseCamera.h"
#include "picamera/PiCameraIncludes.h"
#include "picamera/PiCameraSettings.h"
#include "picamera/CameraComponent.h"
#include "picamera/VideoEncoder.h"
#include "picamera/Connection.h"

namespace romi {
        
        class VideoCamera : public BaseCamera
        {
        protected:

                //arm::CameraComponent camera_;
                arm::VideoEncoder encoder_;
                arm::Connection encoder_connection_;
                
        public:                
                VideoCamera(PiCameraSettings& settings);
                ~VideoCamera() override;
                
                bool grab(Image &image) override;
                rcom::MemBuffer& grab_jpeg();
                
                // bool set_value(const std::string& name, double value) override;
                // bool select_option(const std::string& name,
                //                    const std::string& value) override;

                // // Power device interface
                // bool power_up() override;
                // bool power_down() override;
                // bool stand_by() override;
                // bool wake_up() override;
        };
}

#endif // _LIBROMI_VIDEO_CAMERA_H_
