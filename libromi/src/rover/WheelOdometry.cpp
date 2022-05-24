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

#include <log.h>
#include "data_provider/JsonFieldNames.h"
#include "rover/WheelOdometry.h"

namespace romi {
        
        WheelOdometry::WheelOdometry(NavigationSettings &rover_config,
                                     IMotorDriver& driver)
                :   driver_(driver),
                    mutex_(),
                    last_timestamp_(0.0),
                    theta_(0.0),
                    wheel_circumference_(M_PI * rover_config.wheel_diameter),
                    wheeltrack_(rover_config.wheeltrack),
                    encoder_steps_(driver.get_encoder_steps())
        {
                double left, right, timestamp;
                
                if (!driver_.get_encoder_values(left, right, timestamp)) {
                        r_err("WheelOdometry::WheelOdometry: "
                              "get_encoder_values failed");
                        throw std::runtime_error("WheelOdometry::WheelOdometry: "
                                                 "get_encoder_values failed");
                }
                
                displacement_[0] = 0;
                displacement_[1] = 0;                       
                encoder_[0] = left;
                encoder_[1] = right;
                last_timestamp_ = timestamp;
                
                for (int i = 0; i < 2; i++) {
                        instantaneous_speed_[i] = 0;
                        filtered_speed_[i] = 0;
                }
        }
        
        WheelOdometry::~WheelOdometry()
        {
        }
        
        void WheelOdometry::set_displacement(double x, double y)
        {
                displacement_[0] = x;
                displacement_[1] = y;                
        }

        bool WheelOdometry::update_estimate()
        {
                bool success = false;
                double left, right, timestamp;
                if (driver_.get_encoder_values(left, right, timestamp)) {
                        set_encoders(left, right, timestamp);
                        success = true;
                }
                return success;
        }

        bool WheelOdometry::update_location_estimate()
        {
                return update_estimate();
        }
        
        bool WheelOdometry::update_orientation_estimate()
        {
                // Skip, let update_location_estimate() do the work...
                // ... for now
                return true;
        }
        
        v3 WheelOdometry::coordinates()
        {
                SynchronizedCodeBlock sync(mutex_);
                return v3(displacement_[0], displacement_[1], 0.0);
        }
        
        nlohmann::json WheelOdometry::location()
        {
                update_estimate();
                
                nlohmann::json coordinate_object;
                coordinate_object[JsonFieldNames::x_position.data()] = displacement_[0];
                coordinate_object[JsonFieldNames::y_position.data()] = displacement_[1];
                return coordinate_object.dump(4);
        }

        v3 WheelOdometry::get_speed()
        {
                SynchronizedCodeBlock sync(mutex_);
                return v3(filtered_speed_[0], filtered_speed_[1], 0.0);
        }
        
        double WheelOdometry::get_orientation()
        {
                SynchronizedCodeBlock sync(mutex_);
                return theta_;
        }

        v3 WheelOdometry::get_encoders()
        {
                SynchronizedCodeBlock sync(mutex_);
                return v3(encoder_[0], encoder_[1], 0.0);
        }
        
        void WheelOdometry::set_encoders(double left, double right,
                                         double timestamp)
        {
                double dx, dy;
                double dL, dR;
                double half_wheeltrack = 0.5 * wheeltrack_;
                double alpha;

                SynchronizedCodeBlock sync(mutex_);
                
                // r_debug("encL %f, encR %f steps", left, right);
        
                // dL and dR are the distances travelled by the left and right
                // wheel.
                dL = left - encoder_[0];
                dR = right - encoder_[1];
                r_debug("dL %f, dR %f steps", dL, dR);
        
                dL = wheel_circumference_ * dL / encoder_steps_;
                dR = wheel_circumference_ * dR / encoder_steps_;
                // r_debug("dL %f, dR %f m", dL, dR);

                // dx and dy are the changes in the location of the rover, in
                // the frame of reference of the rover.
                if (dL == dR) {
                        dx = dL;
                        dy = 0.0;
                        alpha = 0.0;
                } else {
                        double radius = 0.5 * wheeltrack_ * (dL + dR) / (dR - dL);
                        /* r_debug("radius %f", radius); */
                        if (radius >= 0) {
                                alpha = dR / (radius + half_wheeltrack);
                        } else {
                                alpha = -dL / (-radius + half_wheeltrack);
                        }
                        dx = radius * sin(alpha);
                        dy = radius - radius * cos(alpha);
                }

                // r_debug("dx %f, dy %f, alpha %f", dx, dy, alpha);

                // Convert dx and dy to the changes in the last frame of
                // reference (i.e. relative to the current orientation).
                double c = cos(theta_);
                double s = sin(theta_);
                double dx_ = c * dx - s * dy;
                double dy_ = s * dx + c * dy;

                // r_debug("dx_ %f, dy_ %f", dx_, dy_);

                displacement_[0] += dx_;
                displacement_[1] += dy_;
                theta_ += alpha;
                encoder_[0] = left;
                encoder_[1] = right;

                double dt = timestamp - last_timestamp_;
                if (dt != 0.0) {
                        instantaneous_speed_[0] = dx_ / dt;
                        instantaneous_speed_[1] = dy_ / dt;
                        
                        filtered_speed_[0] = (0.8 * filtered_speed_[0]
                                              + 0.2 * instantaneous_speed_[0]);
                        filtered_speed_[1] = (0.8 * filtered_speed_[1]
                                              + 0.2 * instantaneous_speed_[1]);
                }
                last_timestamp_ = timestamp;
        
                // r_debug("displacement:  %f %f - angle %f",
                //         displacement_[0], displacement_[1], theta_ * 180.0 / M_PI);
                //r_debug("speed:  %f %f", speed[0], speed[1]);
        }
}
