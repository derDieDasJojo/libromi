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
#ifndef __ROMI_FAKE_MOTORDRIVER_H
#define __ROMI_FAKE_MOTORDRIVER_H


#include "api/IMotorDriver.h"

namespace romi {
        
        class FakeMotorDriver : public IMotorDriver
        {
        public:
                
                ~FakeMotorDriver() override = default;

                // TODO: simulate displacements over time
                bool moveat(double left, double right) override {
                        r_debug("FakeMotorDriver::moveat (%.3f,%.3f)", left, right);
                        return true;
                }

                bool stop() override {
                        return true;
                }

                // TODO: simulate displacements over time
                bool get_encoder_values(double &left, double &right,
                                        double &timestamp) override {
                        r_debug("FakeMotorDriver::get_encoder_values");
                        left = 0; 
                        right = 0;
                        timestamp = 0.0;
                        return true;
                }

                bool get_pid_values(Axis axis,
                                    double& target_speed,
                                    double& measured_speed,
                                    double& pid_output,
                                    double& pid_error_p,
                                    double& pid_error_i,
                                    double& pid_error_d,
                                    double& controller_input) {
                        (void) axis;
                        target_speed = 0.0;
                        measured_speed = 0.0;
                        pid_output = 0.0;
                        pid_error_p = 0.0;
                        pid_error_i = 0.0;
                        pid_error_d = 0.0;
                        controller_input = 0.0;
                        return true;
                }
                
        };
}

#endif // __ROMI_FAKE_MOTORDRIVER_H
