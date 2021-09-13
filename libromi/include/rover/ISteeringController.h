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
                        
                ~ISteeringController() override = default;

                virtual bool get_position(int32_t *pos) = 0;

                /** The homing operation sets the ICNC in the home
                 * position AND resets the origin to the location
                 * after the homing operation. */
                virtual bool homing() = 0;
                virtual bool synchronize(double timeout) = 0;
                
                /* Move a given amount relative to the current position */
                virtual bool move(int16_t millis,
                                  int16_t steps_left,
                                  int16_t steps_right) = 0;
                /* Move to an absolute position */
                virtual bool moveto(int16_t millis,
                                    int16_t position_left,
                                    int16_t position_right) = 0;
                /* Move at a give speed */
                virtual bool moveat(int16_t speed_left, int16_t speed_right) = 0;
                
                virtual bool enable() = 0;
                virtual bool disable() = 0;
                virtual bool stop() = 0;
        };
}

#endif // _ROMI_I_STEERING_CONTROLLER_H_
