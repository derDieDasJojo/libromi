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

// FIXME: Avoid pre-compiler macro
#define PI_BUILD
#ifdef PI_BUILD
#include "picamera/PiCamera.h"
#else
#endif
        
#include "cablebot/Cablebot.h"
#include "hal/BldcGimbalI2C.h"
#include "hal/FakeGimbal.h"
#include "hal/CNCAndGimbal.h"
#include "hal/ICameraMount.h"
#include "cablebot/CablebotBase.h"
#include "util/Logger.h"
#include "util/RomiSerialLog.h"
#include "camera/FakeCamera.h"
#include "cablebot/FakeMotorController.h"
#include "camera/CameraWithConfig.h"
#include "hal/I2C.h"

namespace romi {
        
        std::unique_ptr<ImagingDevice> Cablebot::create(rcom::ILinux& linux,
                                                        std::shared_ptr<ICameraInfoIO>& io)
        {
                auto info = io->load();
                
                // Camera
                std::shared_ptr<ICamera> real_camera
                        = make_camera(info->get_settings());
                std::shared_ptr<ICamera> camera
                        = std::make_shared<CameraWithConfig>(io, real_camera);

                // Gimbal
                std::unique_ptr<IGimbal> gimbal = make_gimbal(linux);
                
                // Base
                std::unique_ptr<romiserial::IRomiSerialClient> base_serial
                        = connect_base();
                std::shared_ptr<ICameraMount> mount
                        = std::make_shared<romi::CablebotBase>(base_serial, gimbal);
                        
                // Cablebot
                return std::make_unique<ImagingDevice>(camera, mount);
        }

        Cablebot::CameraMode Cablebot::get_mode(ICameraSettings& settings)
        {
                std::string value;
                settings.get_option(ICameraSettings::kCameraMode, value);
                if (value == ICameraSettings::kStillMode)
                        return kStillMode;
                else if (value == ICameraSettings::kVideoMode)
                        return kVideoMode;
                else
                        throw std::runtime_error("Cablebot::get_mode: unknown mode");
        }
        
        void Cablebot::get_resolution(ICameraSettings& settings,
                                      size_t& width, size_t& height)
        {
                std::string value;
                settings.get_option(ICameraSettings::kResolution, value);
                
                if (value == ICameraSettings::kResolution4056x3040) {
                        width = 4056;
                        height = 3040;
                } else if (value == ICameraSettings::kResolution2028x1520) {
                        width = 2028;
                        height = 1520;
                } else if (value == ICameraSettings::kResolution1014x760) {
                        width = 1014;
                        height = 760;
                } else if (value == ICameraSettings::kResolution3280x2464) {
                        width = 3280;
                        height = 2464;
                } else if (value == ICameraSettings::kResolution1640x1232) {
                        width = 1640;
                        height = 1232;
                } else if (value == ICameraSettings::kResolution820x616) {
                        width = 820;
                        height = 616;
                } else if (value == ICameraSettings::kResolution1920x1080) {
                        width = 1920;
                        height = 1080;
                } else if (value == ICameraSettings::kResolution640x480) {
                        width = 640;
                        height = 480;
                } else {
                        throw std::runtime_error("Cablebot::get_mode: unknown resolution");
                }
        }
                
        int32_t Cablebot::get_framerate(ICameraSettings& settings)
        {
                return (int32_t) settings.get_value(ICameraSettings::kFramerate);
        }
                
        uint32_t Cablebot::get_bitrate(ICameraSettings& settings)
        {
                return (uint32_t) settings.get_value(ICameraSettings::kBitrate);
        }

        std::shared_ptr<ICamera> Cablebot::make_camera(ICameraSettings& settings)
        {
#ifdef PI_BUILD
                return make_pi_camera(settings);
#else
                return make_fake_camera(settings);
#endif
        }

        std::shared_ptr<ICamera>
        Cablebot::make_pi_camera(ICameraSettings& settings)
        {
#ifdef PI_BUILD
                std::unique_ptr<romi::PiCameraSettings> pi_settings;

                CameraMode mode = get_mode(settings);
                size_t width, height;
                
                get_resolution(settings, width, height);
                
                if (mode == kVideoMode) {
                        int32_t fps = get_framerate(settings);
                        uint32_t bitrate = get_bitrate(settings);

                        r_info("Camera: video mode, %d fps, %d bps",
                               (int) fps, (int) bitrate);
                        pi_settings = std::make_unique<romi::HQVideoCameraSettings>(width,
										    height,
										    fps);
                        pi_settings->bitrate_ = bitrate;
                        
                } else if (mode == kStillMode) {
                        r_info("Camera: still mode.");
                        pi_settings = std::make_unique<romi::HQStillCameraSettings>(width,
										    height);
                }
                
                std::shared_ptr<ICamera> camera = romi::PiCamera::create(*pi_settings);
                return camera;
#else
                (void) settings;
                throw std::runtime_error("Not implemented");
#endif
        }
        
        std::shared_ptr<ICamera> Cablebot::make_fake_camera(ICameraSettings& settings)
        {
                size_t width, height;
                get_resolution(settings, width, height);

                int32_t fps = get_framerate(settings);
                
                std::shared_ptr<ICamera> camera
                        = std::make_shared<FakeCamera>(width, height, fps);
                return camera;
        }

        std::unique_ptr<romiserial::IRomiSerialClient> Cablebot::connect_base()
        {
#ifdef PI_BUILD
                return connect_real_base();
#else
                return connect_fake_base();
#endif
        }
        
        std::unique_ptr<romiserial::IRomiSerialClient> Cablebot::connect_real_base()
        {
#ifdef PI_BUILD
                std::shared_ptr<romiserial::ILog> log
                        = std::make_shared<romi::RomiSerialLog>();
                std::unique_ptr<romiserial::IRomiSerialClient> serial;
                serial = romiserial::RomiSerialClient::create(kSerialBase,
                                                              "CablebotBase",
                                                              log);
                return serial;
#else
                throw std::runtime_error("Not implemented");
#endif
        }
        
        std::unique_ptr<romiserial::IRomiSerialClient> Cablebot::connect_fake_base()
        {
                std::unique_ptr<romiserial::IRomiSerialClient> fake
                        = std::make_unique<FakeMotorController>("CablebotBase");
                return fake;
        }

        std::unique_ptr<IGimbal> Cablebot::make_gimbal(rcom::ILinux& linux)
        {
#ifdef PI_BUILD
                return make_i2c_gimbal(linux);
#else
                (void) linux;
                return make_fake_gimbal();
#endif
        }

        std::unique_ptr<IGimbal> Cablebot::make_i2c_gimbal(rcom::ILinux& linux)
        {
#ifdef PI_BUILD
                std::unique_ptr<II2C> bus = std::make_unique<I2C>(linux,
                                                                  kGimbalI2CDevice,
                                                                  11 /*kGimbalI2CAddress*/);
                std::unique_ptr<IGimbal> gimbal = std::make_unique<BldcGimbalI2C>(bus);
                return gimbal;
#else
                (void) linux;
                throw std::runtime_error("Cablebot::make_i2c_gimbal: Not implemented");
#endif
        }

        std::unique_ptr<IGimbal> Cablebot::make_fake_gimbal()
        {
                std::unique_ptr<IGimbal> gimbal = std::make_unique<FakeGimbal>();
                return gimbal;
        }
}
