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
#include "hal/BldcGimbalI2C.h"

namespace romi {
        
        BldcGimbalI2C::BldcGimbalI2C(II2C& bus)
                : bus_(bus),
                  power_(kDefaultPower)
        {
        }

        int BldcGimbalI2C::angle_to_arg(double angle)
        {
                return (int) (clamp(angle) * 10.0);
        }

        double BldcGimbalI2C::arg_to_angle(double arg)
        {
                return arg / 10.0;
        }
        
        double BldcGimbalI2C::clamp(double angle)
        {
                angle = std::fmod(angle, 360.0);
                if (angle < 0.0)
                        angle += 360.0;
                return angle;
        }
        
        bool BldcGimbalI2C::get_range(IRange& range)
        {
                v3 min(-360.0, 0.0, 0.0);
                v3 max(360.0, 0.0, 0.0);
                range.init(min, max);
                return true;
        }

        bool BldcGimbalI2C::moveto(double phi_x, double phi_y, double phi_z,
                                double relative_speed)
        {
                (void) phi_x;
                (void) phi_y;
                (void) phi_z;
                (void) relative_speed;
                return true;
        }

        bool BldcGimbalI2C::moveat(double wx, double wy, double wz)
        {
                (void) wx;
                (void) wy;
                (void) wz;
                return true;
        }

        bool BldcGimbalI2C::get_position(v3& position)
        {
                (void) position;
                return true;
        }
        
        bool BldcGimbalI2C::set_angle(double angle)
        {
                (void) angle;
                return true;
        }

        bool BldcGimbalI2C::pause_activity()
        {
                r_err("BldcGimbalI2C::pause_activity: Not implemented");
                throw std::runtime_error("BldcGimbalI2C::pause_activity: Not implemented");
        }

        bool BldcGimbalI2C::continue_activity()
        {
                r_err("BldcGimbalI2C::continue_activity: Not implemented");
                throw std::runtime_error("BldcGimbalI2C::continue_activity: Not implemented");
        }

        bool BldcGimbalI2C::reset_activity()
        {
                r_err("BldcGimbalI2C::reset_activity: Not implemented");
                throw std::runtime_error("BldcGimbalI2C::reset_activity: Not implemented");
        }

        bool BldcGimbalI2C::power_up()
        {
                return true; // FIXME
        }

        bool BldcGimbalI2C::power_down()
        {
                return true; // FIXME
        }

        bool BldcGimbalI2C::homing()
        {
                return true; // false?
        }

        bool BldcGimbalI2C::stand_by()
        {
                return power_down();
        }

        bool BldcGimbalI2C::wake_up()
        {
                return power_up();
        }
}
