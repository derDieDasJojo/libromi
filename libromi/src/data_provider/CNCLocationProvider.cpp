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
#include "data_provider/CNCLocationProvider.h"
#include "JsonCpp.h"

namespace romi {

        CNCLocationProvider::CNCLocationProvider(std::shared_ptr<ICNC>& cnc)
                : cnc_(cnc),
                  position_(0.0)
        {
        }

        bool CNCLocationProvider::update_location_estimate()
        {
                return cnc_->get_position(position_); 
        }
        
        std::string CNCLocationProvider::get_location_string()
        {
                if (!update_location_estimate()) {
                        r_warn("CNCLocationProvider: update failed. "
                               "Returning old estimates");
                }

                json_object_t coordinate_object = json_object_create();
                json_object_setnum(coordinate_object,
                                   JsonFieldNames::x_position.data(),
                                   position_.x());
                json_object_setnum(coordinate_object,
                                   JsonFieldNames::y_position.data(),
                                   position_.y());
                json_object_setnum(coordinate_object,
                                   JsonFieldNames::z_position.data(),
                                   position_.z());
                
                std::string locationString;
                JsonCpp locationData(coordinate_object);
                locationData.tostring(locationString, k_json_pretty | k_json_sort_keys);

                json_unref(coordinate_object);
                return locationString;
        }

        v3 CNCLocationProvider::get_location()
        {
                if (!update_location_estimate()) {
                        r_warn("CNCLocationProvider: update failed. "
                               "Returning old estimates");
                }
                return position_;
        }
}
