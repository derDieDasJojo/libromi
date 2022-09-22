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

#include "picamera/PiCamera.h"
#include "picamera/StillCamera.h"
#include "picamera/VideoCamera.h"
#include "camera/ICameraSettings.h"
#include "util/Logger.h"

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

        bool PiCamera::try_create_implementation()
        {
                bool result = false;
                try {
                        create_implementation();
                        result = true;
                } catch (std::runtime_error& e) {
                        r_err("PiCamera::try_create_implementation: failed");
                }
                return result;
        }

        void PiCamera::create_implementation()
        {
                destroy_implementation();

                if (settings_.mode_ == kStillMode)
                        impl_ = std::make_unique<StillCamera>(settings_);
                else
                        impl_ = std::make_unique<VideoCamera>(settings_);
        }

        void PiCamera::assert_implementation()
        {
                if (!impl_) {
                        r_err("PiCamera::assert_implementation");
                        throw std::runtime_error("PiCamera::assert_implementation");
                }
        }
        
        bool PiCamera::grab(Image &image)
        {
                SynchronizedCodeBlock sync(mutex_);
                assert_implementation();
                return impl_->grab(image);
        }
        
        rcom::MemBuffer& PiCamera::grab_jpeg()
        {
                SynchronizedCodeBlock sync(mutex_);
                assert_implementation();
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
                } else if (name == ICameraSettings::kShutterSpeed) {
                        result = set_shutter_speed((uint32_t) value);
                } else if (name == ICameraSettings::kAnalogGain) {
                        result = set_analog_gain((float) value);
                }
                return result;
        }
        
        bool PiCamera::select_option(const std::string& name,
                                     const std::string& value)
        {
                SynchronizedCodeBlock sync(mutex_);
                bool result = false;
                if (name == ICameraSettings::kResolution) {
                        result = set_resolution(value);
                } else if (name == ICameraSettings::kCameraMode) {
                        result = set_mode(value);
                } else if (name == ICameraSettings::kExposureMode) {
                        result = set_exposure_mode(value);
                } else {
                        r_err("PiCamera::select_option: unknown option: %s",
                              name.c_str());
                }
                return result;
        }

        bool PiCamera::set_shutter_speed(uint32_t value)
        { 
                bool result = false;
                if (settings_.set_shutter_speed(value)) {
                        result = impl_->set_shutter_speed(value);                        
                } else {
                        r_err("PiCamera::set_shutter_speed: set failed");
                }
                return result;
        }

        bool PiCamera::set_analog_gain(float value)
        { 
                bool result = false;
                if (settings_.set_analog_gain(value)) {
                        result = impl_->set_analog_gain(value);                        
                } else {
                        r_err("PiCamera::set_analog_gain: set failed");
                }
                return result;
        }
        
        bool PiCamera::set_resolution(const std::string& value)
        { 
               bool result = true;
               if (value == ICameraSettings::kResolution4056x3040) {
                       result = set_resolution(4056, 3040);
               } else if (value == ICameraSettings::kResolution2028x1520) {
                       result = set_resolution(2028, 1520);
               } else if (value == ICameraSettings::kResolution1014x760) {
                       result = set_resolution(1014, 760);
               } else if (value == ICameraSettings::kResolution3280x2464) {
                       result = set_resolution(3280, 2464);
               } else if (value == ICameraSettings::kResolution1640x1232) {
                       result = set_resolution(1640, 1232);
               } else if (value == ICameraSettings::kResolution820x616) {
                       result = set_resolution(820, 616);
               } else if (value == ICameraSettings::kResolution1920x1080) {
                       result = set_resolution(1920, 1080);
               } else if (value == ICameraSettings::kResolution640x480) {
                       result = set_resolution(640, 480);
               } else {
                       r_err("PiCamera::set_resolution: unhandled resolution: %s",
                             value.c_str());
                       result = false;
               }
               return result;
        }

        bool PiCamera::set_resolution(size_t width, size_t height)
        { 
                bool result = false;
                if (settings_.set_resolution(width, height)) {
                        result = try_create_implementation();                        
                }
                return result;
        }

        bool PiCamera::set_mode(const std::string& value)
        {
                bool result = false;
                if (value == ICameraSettings::kStillMode) {
                        settings_.mode_ = kStillMode;
                        result = try_create_implementation();
                } else if (value == ICameraSettings::kVideoMode) {
                        settings_.mode_ = kVideoMode;
                        result = try_create_implementation();
                } else {
                        r_err("PiCamera::set_mode: invalid mode: %s", value.c_str());
                }
                return result;
        }
        
        bool PiCamera::set_exposure_mode(const std::string& value)
        {
                bool result = false;
                if (value == ICameraSettings::kExposureAuto) {
                        if (settings_.set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_AUTO))
                                result = impl_->set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_AUTO);
                } else if (value == ICameraSettings::kExposureOff) {
                        if (settings_.set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_OFF))
                                result = impl_->set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_OFF);
                } else if (value == ICameraSettings::kExposureSports) {
                        if (settings_.set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_SPORTS))
                                result = impl_->set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_SPORTS);
                } else {
                        r_err("PiCamera::set_exposure_mode: unhandled mode: %s",
                              value.c_str());
                }
                return result;
        }
        
        bool PiCamera::set_saturation(int32_t saturation)
        {
                bool result = false;
                assert_implementation();

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
                assert_implementation();
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
                assert_implementation();
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
                assert_implementation();
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
                assert_implementation();
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
                assert_implementation();
                if (settings_.set_jpeg_quality(quality)) {
                        result = impl_->set_jpeg_quality(quality);
                        result = false;
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

