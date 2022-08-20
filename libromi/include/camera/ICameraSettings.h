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

#ifndef __ROMI_ICAMERASETTINGS_H
#define __ROMI_ICAMERASETTINGS_H

#include <vector>
#include <string>
#include <json.hpp>

namespace romi {

        // class CameraBaseInfo
        // {
        // protected:
        //         int32_t id_;
        //         std::string name_;
        //         bool option_;
                
        // public:
        //         CameraBaseInfo(int32_t id, std::string& name, bool option)
        //                 : id_(id),
        //                   name_(name),
        //                   option_(option) {
        //         }

        //         int32_t id() {
        //                 return id_;
        //         }
                
        //         std::string& name() {
        //                 return name_;
        //         }

        //         bool is_option() {
        //                 return option_;
        //         }
        // };
        
        // class CameraSetting : public CameraBaseInfo
        // {
        // protected:
        //         double min_;
        //         double max_;
        //         double default_;

        // public:
                
        //         CameraSetting(int32_t id, std::string& name, double min,
        //                           double max, double def)
        //                 : CameraBaseInfo(id, name, false),
        //                   min_(min),
        //                   max_(max),
        //                   default_(def) {
        //         }
        // };
        
        // class CameraOption : public CameraBaseInfo
        // {
        // protected:
        //         int default_;
        //         std::vector<std::string> options_;

        // public:
                
        //         CameraOption(int32_t id, std::string& name, int def,
        //                      std::vector<std::string>& options)
        //                 : CameraBaseInfo(id, name, true),
        //                   default_(def),
        //                   options_(options) {
        //         }
        // };
        
        class ICameraSettings
        {
        public:

                static constexpr const char* kJpegQuality = "jpeg-quality"; 
                static constexpr const char* kSaturation = "saturation"; 
                static constexpr const char* kSharpness = "sharpness"; 
                static constexpr const char* kContrast = "contrast"; 
                static constexpr const char* kBrightness = "brightness"; 
                static constexpr const char* kISO = "iso"; 
                
                virtual ~ICameraSettings() = default;

                virtual nlohmann::json get_all() = 0;
                virtual double get_value(const std::string& name) = 0;
                virtual bool set_value(const std::string& name, double value) = 0;
                virtual std::string& get_option(const std::string& name,
                                                std::string& value) = 0;
                virtual bool select_option(const std::string& name,
                                           const std::string& value) = 0;
                
                //virtual void list_settings(std::vector<CameraSettingInfo>& settings) = 0;
        };
}

#endif // __ROMI_ICAMERASETTINGS_H
