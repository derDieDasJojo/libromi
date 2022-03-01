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

#include <math.h>
#include <limits.h>
#include <stdexcept>
#include <r.h>
#include "cablebot/CablebotBase.h"

namespace romi {
        
        CablebotBase::CablebotBase(std::unique_ptr<romiserial::IRomiSerialClient>& serial)
                : serial_(std::move(serial)),
                  range_(),
                  diameter_(0.04),
                  circumference_(0.0)
        {
                circumference_ = diameter_ * M_PI;
                range_.init(v3(0.0), v3(20.0, 0.0, 0.0)); // TODO
        }

        int16_t CablebotBase::position_to_steps(double x)
        {
                double turns = x / circumference_;
                double steps = kPrecision * turns;
                if (steps > SHRT_MAX || steps < SHRT_MIN) {
                        r_err("CablebotBase::position_to_turns: steps out of range: "
                              "position: %0.3f, turns: 0.3f, "
                              "steps: %0.1f, precision: %0.3f",
                              x, turns, steps, kPrecision);
                        throw std::runtime_error("CablebotBase::position_to_turns: "
                                                 "steps out of range");
                }
                return (int16_t) steps;
        }
        
        double CablebotBase::steps_to_position(double steps)
        {
                double turns = (double) steps / kPrecision;
                return turns * circumference_;
        }
        
        bool CablebotBase::get_range(CNCRange &range)
        {
                range = range_;
                return true;
        }
        
        bool CablebotBase::send_command(const char *command)
        {
                JsonCpp response;
                serial_->send(command, response);
                
                bool success = (response.num(0) == 0);
                if (!success)
                        r_err("CablebotBase::send_command: %s: %s",
                              command, response.str(1));
                
                return success;
        }
        
        bool CablebotBase::moveto(double x, double y, double z, double relative_speed)
        {
                validate_coordinates(x, y, z);
                validate_speed(relative_speed);
                
                char command[64];
                snprintf(command, sizeof(command), "m[%d]", position_to_steps(x));
                
                return send_command(command);
        }
        
        void CablebotBase::validate_coordinates(double x, double y, double z)
        {
                if (!range_.is_inside(x, y, z)) {
                        r_err("Cablebotbase::validate_coordinates: Position out of range: "
                                "(%.3f,%.3f,%.3f)", x, y, z);
                        throw std::runtime_error("Cablebot::validate_coordinates: "
                                                 "Position out of range");
                }
        }
        
        void CablebotBase::validate_speed(double v)
        {
                if (v < 0.0 || v > 1.0) {
                        r_err("CablebotBase::validate_speed: Invalid speed: %.3f", v);
                        throw std::runtime_error("Cablebot::validate_speed: "
                                                 "Invalid speed");
                }
        }
                       
        bool CablebotBase::moveat(int16_t speed_x, int16_t speed_y, int16_t speed_z)
        {
                (void) speed_x;
                (void) speed_y;
                (void) speed_z;
                r_err("CablebotBase::moveat: Not implemented");
                throw std::runtime_error("CablebotBase::moveat: Not implemented");
                return false;
        }
        
        bool CablebotBase::homing()
        {
                return send_command("H");
        }
        
        bool CablebotBase::get_position(v3& position)
        {
                JsonCpp response;
                serial_->send("P", response);
                
                bool success = (response.num(0) == 0);
                if (success) {
                        double steps = response.num(1);
                        position.x(steps_to_position(steps));
                        position.y(0.0);
                        position.z(0.0);
                }
                return success;
        }
        
        bool CablebotBase::spindle(double speed)
        {
                (void) speed;
                r_err("CablebotBase::spindle: Not implemented");
                throw std::runtime_error("CablebotBase::spindle: Not implemented");
                return false;
        }
        
        bool CablebotBase::travel(Path &path, double relative_speed)
        {
                (void) path;
                (void) relative_speed;
                r_err("CablebotBase::travel: Not implemented");
                throw std::runtime_error("CablebotBase::travel: Not implemented");
                return false;
        }
        
        bool CablebotBase::helix(double xc, double yc, double alpha, double z,
                                 double relative_speed)
        {
                (void) xc;
                (void) yc;
                (void) alpha;
                (void) z;
                (void) relative_speed;
                r_err("CablebotBase::helix: Not implemented");
                throw std::runtime_error("CablebotBase::helix: Not implemented");
                return false;
        }

        bool CablebotBase::pause_activity()
        {
                r_err("CablebotBase::pause_activity: Not implemented");
                throw std::runtime_error("CablebotBase::pause_activity: Not implemented");
        }
        
        bool CablebotBase::continue_activity()
        {
                r_err("CablebotBase::continue_activity: Not implemented");
                throw std::runtime_error("CablebotBase::continue_activity: Not implemented");
        }
        
        bool CablebotBase::reset_activity()
        {
                r_err("CablebotBase::reset_activity: Not implemented");
                throw std::runtime_error("CablebotBase::reset_activity: Not implemented");
        }

        bool CablebotBase::enable_driver()
        {
                return send_command("E[1]");
        }

        bool CablebotBase::disable_driver()
        {
                return send_command("E[0]");
        }

        bool CablebotBase::power_up()
        {
                return enable_driver() && homing();
        }
        
        bool CablebotBase::power_down()
        {
                return disable_driver();
        }
        
        bool CablebotBase::stand_by()
        {
                return disable_driver();
        }
        
        bool CablebotBase::wake_up()
        {
                return enable_driver();
        }
}
