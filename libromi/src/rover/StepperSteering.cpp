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
#include <algorithm>
#include <ClockAccessor.h>
#include "rover/StepperSteering.h"
#include "api/DataLogAccessor.h"
#include <Logger.h>

namespace romi {

        static const std::string kTargetAngleLeft = "steering-target-angle-left";
        static const std::string kTargetAngleRight = "steering-target-angle-right";        
        static const std::string kAngleLeft = "steering-angle-left";
        static const std::string kAngleRight = "steering-angle-right";        
        
        StepperSteering::StepperSteering(ISteeringController& stepper_controller,
                                         NavigationSettings& settings,
                                         int16_t steps_per_second,
                                         double steps_per_revolution)
                : controller_(stepper_controller),
                  settings_(settings),
                  steps_per_second_(steps_per_second),
                  steps_per_revolution_(steps_per_revolution),
                  left_current_(0),
                  right_current_(0),
                  left_target_(0),
                  right_target_(0),
                  steps_left_(0),
                  steps_right_(0),
                  max_angular_speed_(0.0),
                  update_interval_(0.0),
                  last_update_(0.0),
                  mutex_(),
                  thread_(nullptr),
                  quitting_(false)
        {
                max_angular_speed_ = (2.0 * M_PI
                                      * (double) steps_per_second
                                      / steps_per_revolution);
                update_interval_ = 0.200;
                last_update_ = rpp::ClockAccessor::GetInstance()->time();
                
                if (!enable())
                        throw std::runtime_error("StepperSteering: enable failed");

                if (!homing())
                        throw std::runtime_error("StepperSteering: homing failed");

                // thread_ = std::make_unique<std::thread>([this]() {
                //                 this->run_target_updates();
                //         });
        }

        StepperSteering::~StepperSteering()
        {
                quitting_ = true;
                if (thread_ != nullptr)
                        thread_->join();
        }
        
        bool StepperSteering::homing()
        {
                r_info("StepperSteering::homing()");
                bool success = false;
                try {
                        success = controller_.homing(); 
                        
                } catch (const std::runtime_error& re) {
                        r_err("StepperSteering: homing failed: %s", re.what());
                }
                return success;
        }
        
        bool StepperSteering::drive(double speed, SteeringData steering)
        {
                bool success;
                if (steering.type_ == kStraight)
                        success = forward(speed);
                else
                        success = turn(speed, steering.radius_);
                return success;
        }

        bool StepperSteering::forward(double speed)
        {
                (void) speed;
                return turn_wheels(0, 0);
        }
        
        bool StepperSteering::turn(double speed, double radius)
        {
                (void) speed;
                // Ackerman steering
                double left_angle = atan(settings_.wheelbase
                                         / (radius - settings_.wheeltrack / 2.0));
                double right_angle = atan(settings_.wheelbase
                                          / (radius + settings_.wheeltrack / 2.0));
                return turn_wheels(left_angle, right_angle);
        }
        
        bool StepperSteering::turn_wheels(double left_angle, double right_angle)
        {
                return set_target(left_angle, right_angle);
        }

        bool StepperSteering::set_target(double left, double right)
        {
                std::lock_guard sync(mutex_);
                left_target_ = left;
                right_target_ = right;

                int16_t tl = radians_to_10th_of_degrees(left); 
                int16_t tr = radians_to_10th_of_degrees(right);
                
                controller_.set_target(tl, tr);

                r_debug("StepperSteering::set_target: %d, %d", tl, tr);
                
                log_data(kTargetAngleLeft, left_target_); 
                log_data(kTargetAngleRight, right_target_);
                return true;
        }
        
        int16_t StepperSteering::radians_to_10th_of_degrees(double angle)
        {
                return (int16_t) (3600.0 * angle / (2.0 * M_PI)); 
        }
        
        
        bool StepperSteering::stop()
        {
                return true;
        }

        bool StepperSteering::enable()
        {
                return controller_.enable();
        }
        
        bool StepperSteering::disable()
        {
                return controller_.disable();
        }

        bool StepperSteering::initialise() {
                return homing();
        }
}
