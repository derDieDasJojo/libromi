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

#include <Logger.h>
#include "rover/IMUTrackFollower.h"

namespace romi {

        IMUTrackFollower::IMUTrackFollower(std::unique_ptr<romiserial::IRomiSerialClient>& imu)
                : romi_client_(),
                  theta0_(0.0),
                  orientation_error_(0.0)
        {
                romi_client_ = std::move(imu);
        }

        bool IMUTrackFollower::start_line()
        {
                bool success = false;
                try {
                        theta0_ = get_orientation();
                        success = true;
                } catch (const std::runtime_error& e) {
                }
                return success;
        }

        double IMUTrackFollower::get_cross_track_error()
        {
                return 0.0;
        }

        bool IMUTrackFollower::update_error_estimate()
        {
                bool success = false;
                try {
                        double orientation = get_orientation();
                        orientation_error_ = orientation - theta0_;
                        success = true;
                } catch (const std::runtime_error& e) {
                }
                return success;
        }
        
        double IMUTrackFollower::get_orientation_error()
        {
                return orientation_error_;
        }

        
        double IMUTrackFollower::get_orientation()
        {
                nlohmann::json response;
                double orientation;
                
                romi_client_->send("o", response);
                bool success = response[0] == 0;
                if (success) {
                        orientation = response[1];
                } else {
                        r_err("IMUTrackFollower::get_orientation failed: code %d",
                              (int) response[0]);
                        throw std::runtime_error("IMUTrackFollower::get_orientation failed");
                }
                 return orientation;
        }
}
