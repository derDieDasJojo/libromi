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
#include <cmath>
#include <log.h>
#include "hal/BldcGimbal.h"

namespace romi {
        
        BldcGimbal::BldcGimbal(romiserial::IRomiSerialClient& serial)
                : serial_(serial)
        {
        }

        int BldcGimbal::angle_to_arg(double angle)
        {
                return (int) (clamp(angle) * 10.0);
        }

        double BldcGimbal::arg_to_angle(double arg)
        {
                return arg / 10.0;
        }
        
        double BldcGimbal::clamp(double angle)
        {
                angle = std::fmod(angle, 360.0);
                if (angle < 0.0)
                        angle += 360.0;
                return angle;
        }

        bool BldcGimbal::moveto(double angle)
        {
                char command[64];
                snprintf(command, sizeof(command), "M[%d]", angle_to_arg(angle));
                
                nlohmann::json response;
                serial_.send(command, response);
                
                bool success = (response[0] == 0);
                if (!success) {
                        r_err("BldcGimbal::moveto: %s", to_string(response[1]).c_str());
                }
                return success;
        }

        bool BldcGimbal::moveat(double rps)
        {
                char command[64];
                snprintf(command, sizeof(command), "V[%d]", (int) (rps * 1000.0));
                
                nlohmann::json response;
                serial_.send(command, response);
                
                bool success = (response[0] == 0);
                if (!success) {
                        r_err("BldcGimbal::moveat: %s", to_string(response[1]).c_str());
                }
                return success;
        }
                        
        bool BldcGimbal::get_angle(double& angle)
        {
                nlohmann::json response;
                serial_.send("s", response);
                
                bool success = (response[0] == 0);
                if (success) {
                        angle = arg_to_angle(response[1]);
                } else {
                        r_err("BldcGimbal::get_position: %s", to_string(response[1]).c_str());
                }
                return success;
        }
        
        bool BldcGimbal::set_angle(double angle)
        {
                char command[64];
                snprintf(command, sizeof(command), "A[%d]", angle_to_arg(angle));
                
                nlohmann::json response;
                serial_.send(command, response);
                
                bool success = (response[0] == 0);
                if (!success) {
                        r_err("BldcGimbal::set_angle: %s", to_string(response[1]).c_str());
                }
                return success;
        }

        bool BldcGimbal::pause_activity()
        {
                return true;
        }

        bool BldcGimbal::continue_activity()
        {
                return true;
        }

        bool BldcGimbal::reset_activity()
        {
                return true;
        }

        bool BldcGimbal::power_up()
        {
                nlohmann::json response;
                serial_.send("P[40]", response);
                return (response[0] == 0);
        }

        bool BldcGimbal::power_down()
        {
                nlohmann::json response;
                serial_.send("P[0]", response);
                return (response[0] == 0);
        }

        bool BldcGimbal::stand_by()
        {
                return power_down();
        }

        bool BldcGimbal::wake_up()
        {
                return power_up();
        }
}
