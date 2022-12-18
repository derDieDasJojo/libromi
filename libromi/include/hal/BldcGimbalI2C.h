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
#ifndef __ROMI_BLDCGIMBALI2C_H
#define __ROMI_BLDCGIMBALI2C_H

#include <memory>
#include "hal/II2C.h"
#include "api/IGimbal.h"

namespace romi {
        
        class BldcGimbalI2C : public IGimbal
        {
        protected:
                
                enum I2cCommands {
                        kFollow,
                        kAngle,
                        kZeroOffset,
                        kZero,
                        kMax,
                        kMotorSleep,
                        kMotorPower,
                        kMotorPosition,
                        kKp,
                        kMaxAccelation,
                        kReset
                }; 
                
                static const constexpr double kDefaultPower = 0.4; // in range ]0,1] 

                std::unique_ptr<II2C> bus_;
                double power_;
                
        public:
                BldcGimbalI2C(std::unique_ptr<II2C>& bus);
                virtual ~BldcGimbalI2C() = default;

                // bonus:
                //bool moveat(double wx, double wy, double wz); //override;
                //bool set_angle(double value); //override;
                                
                // IGimbal
                bool moveto(double phi_x, double phi_y, double phi_z,
                            double relative_speed) override;
                bool get_position(v3& position) override; 
                bool get_range(IRange& range) override;
                bool homing() override;

                // IActivity
                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;

                // IPowerDevice
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;

        protected:

                
                double clamp(double angle_in_degrees);
                int angle_to_arg(double angle);
                double arg_to_angle(double arg);

                int32_t scale_up(double value); 
                double scale_down(int32_t value); 
                
                void set_follow(bool value);
                double get_angle();
                void set_angle(double angle);
                double get_zero_offset();
                void set_zero_offset(double offset);
                void set_zero();
                double get_max();
                void set_max(double value);
                void motor_sleep();
                void motor_wake();
                double get_motor_power();
                void set_motor_power(double power);
                double get_motor_position();
                void set_motor_position(double position);
                double get_kp();
                void set_kp(double kp);
                double get_max_accel();
                void set_max_accel(double value);
                void reset();
        };
}

#endif // __ROMI_BLDCGIMBALI2C_H
