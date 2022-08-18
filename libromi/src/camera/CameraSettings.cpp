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
#include "camera/CameraSettings.h"
#include <iostream>

namespace romi {
        
        CameraSettings::CameraSettings(nlohmann::json& json)
                : settings_(json)
        {
        }

        nlohmann::json CameraSettings::get_all()
        {
                return settings_;
        }
        
        double CameraSettings::get_value(const std::string& name)
        {
                return settings_[name];//.get<double>();
        }
        
        bool CameraSettings::set_value(const std::string& name, double value)
        {
                settings_[name] = value;
                return true;
        }
        
        std::string& CameraSettings::get_option(const std::string& name,
                                                std::string& value)
        {
                value = settings_[name];
                return value;
        }
        
        bool CameraSettings::select_option(const std::string& name,
                                           const std::string& value)
        {
                settings_[name] = value;
                return true;
        }
}
