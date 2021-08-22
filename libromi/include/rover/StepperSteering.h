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

#include <thread>
#include <mutex>
#include <atomic>
#include "rover/ISteering.h"
#include "oquam/ICNCController.h"
#include "rover/NavigationSettings.h"

namespace romi {
        
        class StepperSteering : public ISteering
        {
        protected:
                ICNCController& controller_;
                NavigationSettings& settings_;
                int16_t steps_per_second_;
                double steps_per_revolution_;
                double left_angle_;
                double right_angle_;
                double left_target_;
                double right_target_;
                int16_t steps_left_;
                int16_t steps_right_;
                std::mutex mutex_;
                std::unique_ptr<std::thread> thread_;
                std::atomic<bool> quitting_;
                
                bool turn_wheels(double left_angle, double right_angle);
                bool set_target(double target_left, double target_right);
                void run_target_updates();
                bool angles_changed();
                bool send_wheel_angles();
                bool moveto(int16_t steps_left, int16_t steps_right);
                int16_t angle_to_steps(double angle);

        public:
                StepperSteering(ICNCController& stepper_controller,
                                NavigationSettings& settings,
                                int16_t steps_per_second,
                                double steps_per_revolution);
                ~StepperSteering() override;
                
                bool enable() override;
                bool disable() override;
                bool stop() override;
                bool drive(double speed, SteeringData steering) override;
                bool forward(double speed) override;
                bool turn(double speed, double radius) override;
        };
}

#endif // __ROMI_STEPPER_STEERING_H
