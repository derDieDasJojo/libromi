/*
  romi-rover

  Copyright (C) 2019-2020 Sony Computer Science Laboratories
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
#include <algorithm>
#include "rover/SpeedController.h"
#include <stdexcept>

namespace romi {
        
        SpeedController::SpeedController(INavigation &navigation,
                                         nlohmann::json& config)
                : _navigation(navigation), _fast(), _accurate()
        {
                try {
                        nlohmann::json fast_config(config.at("user-interface").at("speed-controller").at("fast"));
                        nlohmann::json accurate_config = (config.at("user-interface").at("speed-controller").at("accurate"));
                        _fast.parse(fast_config);
                        _accurate.parse(accurate_config);

                        if (!_accurate.is_valid())
                            throw std::range_error("Invalid settings for accurate speed controller");
                        if (!_fast.is_valid())
                            throw std::range_error("Invalid settings for fast speed controller");
                } catch (nlohmann::json::exception& je) {
                        r_err("SpeedController::SpeedController: failed to "
                              "parse the configuration");
                        throw;
                }

        }

        SpeedController::SpeedController(INavigation &navigation,
                                         SpeedConverter &fast,
                                         SpeedConverter &accurate)
                : _navigation(navigation), _fast(fast), _accurate(accurate)
        {
                if (!_fast.is_valid()) 
                        throw std::range_error("Invalid settings for "
                                               "fast speed controller");

                if (!_accurate.is_valid()) 
                        throw std::range_error("Invalid settings for "
                                               "accurate speed controller");
        }

        bool SpeedController::send_moveat(double left, double right)
        {
                // r_debug("ISpeedController::moveat(left=%.3f,right=%.3f)", left, right);
                left = std::clamp(left, -1.0, 1.0); 
                right = std::clamp(right, -1.0, 1.0);
                return _navigation.moveat(left, right);
        }
                
        bool SpeedController::stop()
        {
                return _navigation.stop();
        }
        
        bool SpeedController::drive_at(double speed, double direction)
        {
                WheelSpeeds speeds = _fast.compute_speeds(speed, direction);
                return send_moveat(speeds.left, speeds.right);
        }
                
        bool SpeedController::drive_accurately_at(double speed, double direction)
        {
                WheelSpeeds speeds = _accurate.compute_speeds(speed, direction);
                return send_moveat(speeds.left, speeds.right);
        }

        bool SpeedController::spin(double direction)
        {
                WheelSpeeds speeds = _accurate.compute_speeds(0.0, direction);
                return send_moveat(speeds.left, speeds.right);
        }        
}
