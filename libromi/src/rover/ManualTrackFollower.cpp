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
#include "rover/ManualTrackFollower.h"

namespace romi {

        ManualTrackFollower::ManualTrackFollower(IInputDevice& input_device,
                                                 double maximum_angle)
                : input_device_(input_device),
                  maximum_angle_(maximum_angle)
        {
        }

        bool ManualTrackFollower::start_line()
        {
                return true;
        }

        bool ManualTrackFollower::update_error_estimate()
        {
                return true;
        }

        double ManualTrackFollower::get_cross_track_error()
        {
                return 0.0;
        }
        
        double ManualTrackFollower::get_orientation_error()
        {
                return maximum_angle_ * input_device_.get_direction();
        }
}

