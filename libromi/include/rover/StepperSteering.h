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
#ifndef __ROMI_STEPPER_STEERING_H
#define __ROMI_STEPPER_STEERING_H

#include "rover/ISteering.h"
#include "rover/ISteeringController.h"
#include "rover/NavigationSettings.h"

namespace romi {
        
        class StepperSteering : public ISteering
        {
        protected:
                ISteeringController& controller_;
                NavigationSettings& settings_;
                int16_t steps_per_second_;
                double steps_per_revolution_;
                double left_current_;
                double right_current_;
                double left_target_;
                double right_target_;
                int16_t steps_left_;
                int16_t steps_right_;
                double max_angular_speed_;
                double update_interval_;
                double last_update_;
                
                bool set_target(double target_left, double target_right);
                int16_t radians_to_10th_of_degrees(double angle);
                bool homing();

        public:
                StepperSteering(ISteeringController& stepper_controller,
                                NavigationSettings& settings,
                                int16_t steps_per_second,
                                double steps_per_revolution);
                ~StepperSteering() override = default;
                
                bool enable() override;
                bool disable() override;
                bool stop() override;
                bool drive(double speed, SteeringData steering) override;
                bool forward(double speed) override;
                bool turn(double speed, double radius) override;

                bool turn_wheels(double left_angle, double right_angle);
                bool initialise() override;
        };
}

#endif // __ROMI_STEPPER_STEERING_H
