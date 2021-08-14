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
#include <r.h>
#include <ClockAccessor.h>
#include "rover/L1NavigationController.h"
#include "api/DataLogAccessor.h"

namespace romi {

        static const std::string kErrorDistanceName = "l1-error-distance";
        static const std::string kErrorAngleName = "l1-error-angle";
        static const std::string kGammaName = "l1-gamma";
        static const std::string kThetaName = "l1-theta";
        static const std::string kRName = "l1-r";
        static const std::string kCorrectionName = "l1-correction";
        
        L1NavigationController::L1NavigationController(double width, double L)
                : w_(width), L_(L) {
                assert_valid_parameters();
        }

        void L1NavigationController::assert_valid_parameters()
        {
                assert_valid_width();
                assert_valid_distance();
        }

        void L1NavigationController::assert_valid_width()
        {
                if (w_ <= 0.0) {
                        r_err("L1NavigationController: The width should be > 0: %f ", w_);
                        throw std::runtime_error("L1NavigationController: Invalid width");
                }
                if (w_ > 5.0) {
                        r_err("L1NavigationController: Please verify the width, "
                              "should be < 5 m: %f ", w_);
                        throw std::runtime_error("L1NavigationController: Invalid width");
                }
        }

        void L1NavigationController::assert_valid_distance()
        {
                if (L_ < w_) {
                        r_err("L1NavigationController: L1 should be at least equal "
                              "to the width: %f < %f", L_, w_);
                        throw std::runtime_error("L1NavigationController: Invalid L1");
                }
                if (L_ > 20.0) {
                        r_err("L1NavigationController: Please verify the value of L1."
                              "The current value is high: %d > 20m", L_);
                        throw std::runtime_error("L1NavigationController: Invalid L1");
                }
        }
        
        double L1NavigationController::estimate_correction(double d, double phi) 
        {
                if (d >= L_) {
                        r_err("L1NavigationController: d>=L");
                        throw std::runtime_error("L1NavigationController: d>=L");
                }
                
                auto clock = rpp::ClockAccessor::GetInstance();
                double now = clock->time();
                double correction = 0.0;
                double gamma = -atan(d / sqrt(L_ * L_ - d * d));
                double theta = phi - gamma;
                
                log_data(now, kErrorDistanceName, d);
                log_data(now, kErrorAngleName, phi);
                log_data(now, kGammaName, gamma);
                log_data(now, kThetaName, theta);

                r_debug("L1NavigationController: d=%.5f, phi=%.5f, gamma=%.5f, theta=%.5f",
                        d, phi, gamma, theta);
                if (theta != 0) {
                        double R = -L_ / (2.0 * sin(theta));
                        correction = w_ / (2.0 * R);
                        log_data(now, kRName, R);
                } else {
                        log_data(now, kRName, 0.0);
                }
                log_data(now, kCorrectionName, correction);
                return correction;
        }
}
