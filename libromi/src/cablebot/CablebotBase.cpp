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

#include <r.h>
#include "cablebot/CablebotBase.h"

namespace romi {
        
        CablebotBase::CablebotBase(std::unique_ptr<romiserial::IRomiSerialClient>& serial)
                : serial_(std::move(serial)),
                  range_()
        {
                range_.init(v3(0.0), v3(20.0, 0.0, 0.0)); // TODO
        }

        bool CablebotBase::get_range(CNCRange &range)
        {
                range = range_;
                return true;
        }
        
        
        bool CablebotBase::moveto(double x, double y, double z, double relative_speed)
        {
                (void) x;
                (void) y;
                (void) z;
                (void) relative_speed;
                r_err("CablebotBase::moveto: Not implemented");
                throw std::runtime_error("CablebotBase::moveto: Not implemented");
                return false;
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
                r_err("CablebotBase::homing: Not implemented");
                throw std::runtime_error("CablebotBase::homing: Not implemented");
                return false;
        }
        
        bool CablebotBase::get_position(v3& position)
        {
                (void) position;
                r_err("CablebotBase::get_position: Not implemented");
                throw std::runtime_error("CablebotBase::get_position: Not implemented");
                return false;
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
}
