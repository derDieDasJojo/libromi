/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

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


#include "cablebot/Cablebot.h"
#include "picamera/PiCamera.h"
#include "hal/BldcGimbal.h"
#include "hal/FakeGimbal.h"
#include "cablebot/CablebotBase.h"

namespace romi {
        
        std::unique_ptr<ImagingDevice> Cablebot::create(CameraMode mode,
                                                        size_t width,
                                                        size_t height,
                                                        int32_t fps,
                                                        uint32_t bitrate)
        {
                // Camera
                std::unique_ptr<romi::PiCameraSettings> settings;

                if (mode == kVideoMode) {
                        r_info("Camera: video mode, %d fps, %d bps",
                               (int) fps, (int) bitrate);
                        settings = std::make_unique<romi::HQVideoCameraSettings>(width,
                                                                                 height,
                                                                                 fps);
                        settings->bitrate_ = bitrate;
                        
                } else if (mode == kStillMode) {
                        r_info("Camera: still mode.");
                        settings = std::make_unique<romi::HQStillCameraSettings>(width,
                                                                                 height);
                }
                
                std::shared_ptr<romi::ICamera> camera = romi::PiCamera::create(*settings);

                // Base
                std::unique_ptr<romiserial::IRomiSerialClient> base_serial;
                std::shared_ptr<romi::ICNC> base;
                r_info("Connecting to base at %s", kSerialBase);
                base_serial = romiserial::RomiSerialClient::create(kSerialBase,
                                                                   "CablebotBase");
                base = std::make_shared<romi::CablebotBase>(base_serial);

                // Gimbal
                //std::unique_ptr<romiserial::IRomiSerialClient> gimbal_serial;
                std::shared_ptr<romi::IGimbal> gimbal;
                
                // r_info("Connecting to gimbal at %s", kSerialGimbal);
                // gimbal_serial = romiserial::RomiSerialClient::create(kSerialGimbal,
                //                                                      "BldcGimbal");
		//gimbal = std::make_unique<romi::BldcGimbal>(*gimbal_serial);
		gimbal = std::make_shared<romi::FakeGimbal>();

                // Cablebot
                return std::make_unique<ImagingDevice>(camera, base, gimbal);
        }
}
