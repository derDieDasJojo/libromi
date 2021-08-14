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
#include "rover/NavigationSettings.h"

namespace romi {
        
        NavigationSettings::NavigationSettings(JsonCpp &config)
                : wheel_diameter(0.0),
                  wheel_base(0.0),
                  maximum_speed(0.0),
                  maximum_acceleration(0.0)
        {
                wheel_diameter = config.num(kWheelDiameterKey);
                wheel_base = config.num(kWheelBaseKey);
                maximum_speed = config.num(kMaximumSpeedKey);
                maximum_acceleration = config.num(kMaximumAccelerationKey);
        }
        
        double NavigationSettings::convert_to_angular_speed(double linear_speed)
        {
                double wheel_circumference = M_PI * wheel_diameter;
                return linear_speed / wheel_circumference;
        }

        double NavigationSettings::compute_max_angular_speed()
        {
                double wheel_circumference = M_PI * wheel_diameter;
                return maximum_speed / wheel_circumference;
        }
        
        double NavigationSettings::compute_max_angular_acceleration()
        {
                double wheel_circumference = M_PI * wheel_diameter;
                return maximum_acceleration / wheel_circumference;
        }
}
