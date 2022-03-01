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
#include "hal/FakeGimbal.h"

namespace romi {
        
        FakeGimbal::FakeGimbal(romiserial::IRomiSerialClient& serial)
                : angle_(0.0)
        {
        }

        bool FakeGimbal::moveto(double angle)
        {
                angle_ = angle;
                return true;
        }

        bool FakeGimbal::moveat(double rps)
        {
                return true;
        }
                        
        bool FakeGimbal::get_angle(double& angle)
        {
                angle = angle_;
                return true;
        }
        
        bool FakeGimbal::set_angle(double angle)
        {
                angle_ = angle;
                return true;
        }

        bool FakeGimbal::pause_activity()
        {
                return true;
        }

        bool FakeGimbal::continue_activity()
        {
                return true;
        }

        bool FakeGimbal::reset_activity()
        {
                return true;
        }

        bool FakeGimbal::power_up()
        {
                return true;
        }

        bool FakeGimbal::power_down()
        {
                return true;
        }

        bool FakeGimbal::stand_by()
        {
                return true;
        }

        bool FakeGimbal::wake_up()
        {
                return true;
        }
}
