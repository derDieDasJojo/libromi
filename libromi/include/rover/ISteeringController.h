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
#ifndef _ROMI_I_STEERING_CONTROLLER_H_
#define _ROMI_I_STEERING_CONTROLLER_H_

#include "api/IActivity.h"

namespace romi {

        class ISteeringController : public IActivity
        {
        public:
                enum { RUNNING, HOMING, ERROR };
                enum SteeringMode { kOpenLoop, kClosedLoop };
                        
                ~ISteeringController() override = default;

                virtual bool get_position(int32_t *pos) = 0;

                /** The homing operation sets the wheels in the
                 * straight position using the index signal of the
                 * encoders. It also resets the origin to that
                 * position, and sets the target angles to zero as
                 * well. */
                virtual bool homing() = 0;
                virtual bool synchronize(double timeout) = 0;

                virtual bool set_mode(SteeringMode mode) = 0;
                
                /** The angles are given in 10th of a degree. For
                 * example, for an angle of -5°, the value should be
                 * -50. */
                virtual bool set_target(int16_t left, int16_t right) = 0;
                
                virtual bool enable() = 0;
                virtual bool disable() = 0;
                virtual bool stop() = 0;
        };
}

#endif // _ROMI_I_STEERING_CONTROLLER_H_
