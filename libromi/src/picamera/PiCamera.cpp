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
#include "picamera/PiCamera.h"
#include "picamera/StillCamera.h"
#include "picamera/VideoCamera.h"
#include "camera/ICameraSettings.h"

namespace romi {

        using SynchronizedCodeBlock = std::lock_guard<std::mutex>;
        
        std::unique_ptr<ICamera> PiCamera::create(PiCameraSettings& settings)
        {
                return std::make_unique<PiCamera>(settings);
        }
        
        PiCamera::PiCamera(PiCameraSettings& settings)
                : settings_(settings),
                  impl_(),
                  mutex_()
        {
                create_implementation();
        }

        PiCamera::~PiCamera()
        {
        }

        void PiCamera::destroy_implementation()
        {
                impl_.reset();
        }

        void PiCamera::create_implementation()
        {
                destroy_implementation();

                if (settings_.mode_ == kStillMode)
                        impl_ = std::make_unique<StillCamera>(settings_);
                else
                        impl_ = std::make_unique<VideoCamera>(settings_);
        }

        bool PiCamera::grab(Image &image)
        {
                SynchronizedCodeBlock sync(mutex_);
                return impl_->grab(image);
        }
        
        rcom::MemBuffer& PiCamera::grab_jpeg()
        {
                SynchronizedCodeBlock sync(mutex_);
                return impl_->grab_jpeg();
        }
                
        bool PiCamera::set_value(const std::string& name, double value)
        {
                SynchronizedCodeBlock sync(mutex_);
                bool result = false;
                if (name == ICameraSettings::kJpegQuality) {
                        result = set_jpeg_quality((uint32_t) value);
                } else if (name == ICameraSettings::kContrast) {
                        result = set_contrast((int32_t) value);
                } else if (name == ICameraSettings::kSaturation) {
                        result = set_saturation((int32_t) value);
                } else if (name == ICameraSettings::kSharpness) {
                        result = set_sharpness((int32_t) value);
                } else if (name == ICameraSettings::kBrightness) {
                        result = set_brightness((int32_t) value);
                } else if (name == ICameraSettings::kISO) {
                        result = set_iso((uint32_t) value);
                }
                return result;
        }
        
        bool PiCamera::select_option(const std::string& name,
                                     const std::string& value)
        {
                (void) name;
                (void) value;
                SynchronizedCodeBlock sync(mutex_);
                r_debug("PiCamera::select_option: %s='%s'", name.c_str(), value.c_str());
                return false;
        }
        
        bool PiCamera::set_saturation(int32_t saturation)
        {
                bool result = false;
                if (settings_.set_saturation(saturation)) {
                        result = impl_->set_saturation(saturation);
                } else {
                        r_err("PiCamera::set_saturation: failed to set the saturation");
                }
                return result;
        }
        
        bool PiCamera::set_sharpness(int32_t sharpness)
        {
                bool result = false;
                if (settings_.set_sharpness(sharpness)) {
                        result = impl_->set_sharpness(sharpness);
                } else {
                        r_err("PiCamera::set_sharpness: failed to set the sharpness");
                }
                return result;
        }
        
        bool PiCamera::set_contrast(int32_t contrast)
        {
                bool result = false;
                if (settings_.set_contrast(contrast)) {
                        result = impl_->set_contrast(contrast);
                } else {
                        r_err("PiCamera::set_contrast: failed to set the contrast");
                }
                return result;
        }
        
        bool PiCamera::set_brightness(int32_t brightness)
        {
                bool result = false;
                if (settings_.set_brightness(brightness)) {
                        result = impl_->set_brightness(brightness);
                } else {
                        r_err("PiCamera::set_brightness: failed to set the brightness");
                }
                return result;
        }
        
        bool PiCamera::set_iso(uint32_t iso)
        {
                bool result = false;
                if (settings_.set_iso(iso)) {
                        result = impl_->set_iso(iso);
                } else {
                        r_err("PiCamera::set_iso: failed to set the iso");
                }
                return result;
        }
        
        bool PiCamera::set_jpeg_quality(uint32_t quality)
        {
                bool result = false;
                if (settings_.set_jpeg_quality(quality)) {
                        result = impl_->set_jpeg_quality(quality);
                } else {
                        r_err("PiCamera::set_iso: failed to set the jpeg quality");
                }
                return result;
        }

        bool PiCamera::power_up()
        {
                return true;
        }
        
        bool PiCamera::power_down()
        {
                return false;
        }
        
        bool PiCamera::stand_by()
        {
                return false;
        }
        
        bool PiCamera::wake_up()
        {
                return false;
        }
}

