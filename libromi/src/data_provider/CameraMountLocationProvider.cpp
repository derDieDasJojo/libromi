/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Douglas Boari, Peter Hanappe

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
#include <stdexcept>
#include "data_provider/JsonFieldNames.h"
#include "data_provider/CameraMountLocationProvider.h"
#include <json.hpp>
#include <log.h>

namespace romi {

        CameraMountLocationProvider::CameraMountLocationProvider(std::shared_ptr<ICameraMount>& mount)
                : mount_(mount),
                  xyz_(0.0),
                  angles_(0.0)
        {
        }

        bool CameraMountLocationProvider::update_location_estimate()
        {
                return mount_->get_position(xyz_, angles_); 
        }

        nlohmann::json CameraMountLocationProvider::location()
        {
                if (!update_location_estimate()) {
                        r_warn("CameraMountLocationProvider: update failed. "
                               "Returning old estimates");
                }

                nlohmann::json coordinate_object = nlohmann::json::object({
                                {JsonFieldNames::x_position.data(), xyz_.x()},
                                {JsonFieldNames::y_position.data(), xyz_.y()},
                                {JsonFieldNames::z_position.data(), xyz_.z()}
                        });
                return coordinate_object;
        }

        v3 CameraMountLocationProvider::coordinates()
        {
                if (!update_location_estimate()) {
                        r_warn("CameraMountLocationProvider: update failed. "
                               "Returning old estimates");
                }
                return xyz_;
        }
}
