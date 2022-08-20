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
#ifndef _LIBROMI_PICAMERA_H_
#define _LIBROMI_PICAMERA_H_

#include <memory>
#include <mutex>
#include "api/ICamera.h"
#include "picamera/BaseCamera.h"
#include "picamera/PiCameraSettings.h"

namespace romi {
        
        class PiCamera : public ICamera
        {
        protected:
                PiCameraSettings settings_;
                std::unique_ptr<BaseCamera> impl_;
                std::mutex mutex_;
                
                void create_implementation();
                void destroy_implementation();

        public:                
                PiCamera(PiCameraSettings& settings);
                ~PiCamera() override;

                static std::unique_ptr<ICamera> create(PiCameraSettings& settings);

                bool grab(Image &image) override;
                rcom::MemBuffer& grab_jpeg() override;                
                bool set_value(const std::string& name, double value) override;
                bool select_option(const std::string& name,
                                   const std::string& value) override;

                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;

                bool set_saturation(int32_t saturation);
                bool set_sharpness(int32_t sharpness);
                bool set_contrast(int32_t contrast);
                bool set_brightness(int32_t brightness);
                bool set_iso(uint32_t iso);
                
        };
}

#endif // _LIBROMI_PICAMERA_H_
