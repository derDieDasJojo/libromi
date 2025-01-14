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
#ifndef __ROMI_MANUAL_TRACKFOLLOWER_H
#define __ROMI_MANUAL_TRACKFOLLOWER_H

#include "rover/ITrackFollower.h"
#include "api/IInputDevice.h"

namespace romi {
        
        class ManualTrackFollower : public ITrackFollower
        {
        protected:
                IInputDevice& input_device_;
                double maximum_angle_;
                
        public:
                ManualTrackFollower(IInputDevice& input_device, double maximum_angle);
                virtual ~ManualTrackFollower() override = default;

                bool start_line() override;
                bool update_error_estimate() override;
                double get_cross_track_error() override;
                double get_orientation_error() override;
        };
}

#endif // __ROMI_I_TRACKFOLLOWER_H
