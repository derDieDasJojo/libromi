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
#include <stdexcept>

#include "util/Logger.h"
#include "rover/L1NavigationController.h"
#include "api/DataLogAccessor.h"

namespace romi {

        static const std::string kErrorDistanceName = "l1-error-distance";
        static const std::string kErrorAngleName = "l1-error-angle";
        static const std::string kGammaName = "l1-gamma";
        static const std::string kThetaName = "l1-theta";
        static const std::string kRName = "l1-r";
        static const std::string kCorrectionName = "l1-correction";
        
        L1NavigationController::L1NavigationController(double L)
                : L_(L) {
                assert_valid_distance();
        }

        void L1NavigationController::assert_valid_distance()
        {
                if (L_ <= 0.0) {
                        r_err("L1NavigationController: L1 should be strictly positive: "
                              "%f < 0.0", L_);
                        throw std::runtime_error("L1NavigationController: Invalid L1");
                }
                if (L_ > 20.0) {
                        r_err("L1NavigationController: Please verify the value of L1."
                              "The current value is high: %d > 20m", L_);
                        throw std::runtime_error("L1NavigationController: Invalid L1");
                }
        }

        SteeringData L1NavigationController::estimate_steering(double d, double phi)
        {
                if (d >= L_) {
                        r_err("L1NavigationController: d>=L");
                        throw std::runtime_error("L1NavigationController: d>=L");
                }
                
                SteeringData steering(kStraight);
                double gamma = -atan(d / sqrt(L_ * L_ - d * d));
                double theta = phi - gamma;
                if (theta != 0) {
                        steering.type_ = kTurn;
                        steering.radius_ = -L_ / (2.0 * sin(theta));
                }
                
                log_data(kErrorDistanceName, d);
                log_data(kErrorAngleName, phi);
                log_data(kGammaName, gamma);
                log_data(kThetaName, theta);
                log_data(kRName, steering.radius_);
                
                return steering;
        }
}
