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
#include "util/Logger.h"
#include "hal/BldcGimbal.h"

namespace romi {
        
        BldcGimbal::BldcGimbal(romiserial::IRomiSerialClient& serial)
                : serial_(serial),
                  power_(kDefaultPower)
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
        
        bool BldcGimbal::get_range(IRange& range)
        {
                v3 min(-360.0, 0.0, 0.0);
                v3 max(360.0, 0.0, 0.0);
                range.init(min, max);
                return true;
        }

        bool BldcGimbal::moveto(double phi_x, double phi_y, double phi_z,
                                double relative_speed)
        {
                (void) phi_y;
                (void) phi_z;
                (void) relative_speed;
                
                char command[64];
                snprintf(command, sizeof(command), "M[%d]", angle_to_arg(phi_x));
                
                nlohmann::json response;
                serial_.send(command, response);
                
                bool success = (response[0] == 0);
                if (!success) {
                        r_err("BldcGimbal::moveto: %s", to_string(response[1]).c_str());
                }
                return success;
        }

        bool BldcGimbal::moveat(double wx, double wy, double wz)
        {
                (void) wy;
                (void) wz;
                
                char command[64];
                double rps = wx / 360.0;
                // FIXME: firmware should use degrees/second also
                snprintf(command, sizeof(command), "V[%d]", (int) (rps * 1000.0));
                
                nlohmann::json response;
                serial_.send(command, response);
                
                bool success = (response[0] == 0);
                if (!success) {
                        r_err("BldcGimbal::moveat: %s", to_string(response[1]).c_str());
                }
                return success;
        }

        bool BldcGimbal::get_position(v3& position)
        {
                nlohmann::json response;
                serial_.send("s", response);

                position.set(0.0);
                             
                bool success = (response[0] == 0);
                if (success) {
                        position.x(arg_to_angle(response[1]));
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
                r_err("BldcGimbal::pause_activity: Not implemented");
                throw std::runtime_error("BldcGimbal::pause_activity: Not implemented");
        }

        bool BldcGimbal::continue_activity()
        {
                r_err("BldcGimbal::continue_activity: Not implemented");
                throw std::runtime_error("BldcGimbal::continue_activity: Not implemented");
        }

        bool BldcGimbal::reset_activity()
        {
                r_err("BldcGimbal::reset_activity: Not implemented");
                throw std::runtime_error("BldcGimbal::reset_activity: Not implemented");
        }

        bool BldcGimbal::power_up()
        {
            char command[64];
            nlohmann::json response;
            snprintf(command, sizeof(command), "P[%d]", (int) (power_ * 100.0));
            serial_.send(command, response);
            return (response[0] == 0);
        }

        bool BldcGimbal::power_down()
        {
                nlohmann::json response;
                serial_.send("P[0]", response);
                return (response[0] == 0);
        }

        bool BldcGimbal::homing()
        {
                return true; // false?
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
