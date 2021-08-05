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

#include <algorithm>
#include <log.h>
#include <ClockAccessor.h>
#include "rover/WheelOdometry.h"
#include "rover/Navigation.h"
#include "rover/ZeroNavigationController.h"
#include "rover/L1NavigationController.h"
#include "rover/PythonTrackFollower.h"
#include "api/DataLogAccessor.h"


#include <RomiSerialClient.h>
#include <RSerial.h>
#include <oquam/StepperController.h>
#include <oquam/StepperSettings.h>

namespace romi {

        static const double kSlowNavigationSpeed = 0.3;
        static const double kDistanceSlowNavigation = 0.1;

        static const std::string kSpeedName = "navigation-speed";
        static const std::string kDistanceName = "navigation-distance";
        static const std::string kLeftSpeedName = "navigation-left-speed";
        static const std::string kRightSpeedName = "navigation-right-speed";
        static const std::string kCrossTrackErrorName = "navigation-cross-track-error";
        static const std::string kOrientationErrorName = "navigation-orientation-error";
        static const std::string kCorrectionName = "navigation-correction";
        
        

        struct Steering
        {
                double width_;
                double length_;
                double left_speed_;
                double right_speed_;
                double left_angle_;
                double right_angle_;

                Steering(double width, double length)
                        : width_(width),
                          length_(length),
                          left_speed_(0.0),
                          right_speed_(0.0),
                          left_angle_(0.0),
                          right_angle_(0.0) {
                }

                void forward(double speed) {
                        left_speed_ = speed;
                        right_speed_ = speed;
                        left_angle_ = 0.0;
                        right_angle_ = 0.0;
                }

                void turn(double speed, double radius) {
                        if (-width_ / 2.0 <= radius && radius <= width_ / 2.0) {
                                r_err("Unhandled radius: %.3f [R<%.3f or R>%.3f]",
                                      radius, -width_/2.0, width_/2.0);
                                throw std::runtime_error("Unhandled value for the radius");
                        }
                        left_speed_ = speed * (1 - width_ / (2.0 * radius));
                        right_speed_ = speed * (1 + width_ / (2.0 * radius));
                        left_angle_ = atan(length_ / (radius - width_ / 2.0));
                        right_angle_ = atan(length_ / (radius + width_ / 2.0));
                }
        };

        Navigation::Navigation(NavigationSettings &settings,
                               IMotorDriver& driver,
                               IDistanceMeasure& distance_measure,
                               ITrackFollower& track_follower,
                               INavigationController& navigation_controller,
                               ISession& session)
                : settings_(settings),
                  driver_(driver),
                  distance_measure_(distance_measure),
                  track_follower_(track_follower),
                  navigation_controller_(navigation_controller),
                  session_(session),
                  mutex_(),
                  status_(MOVEAT_CAPABLE),
                  stop_(false),
                  left_target_(0.0),
                  right_target_(0.0)
        {
                set_speed_targets(0.0, 0.0);
        }
        
        Navigation::~Navigation()
        {
                set_speed_targets(0.0, 0.0);
        }
        
        bool Navigation::set_speed_targets(double left, double right)
        {
                bool changed = (left != left_target_ && right != right_target_);
                if (changed) {
                        left_target_ = left;
                        right_target_ = right;
                        r_debug("Navigation: Speed target now (%.2f, %.2f)", left, right);
                        send_moveat(left_target_, right_target_);
                }
                return true;
        }

        bool Navigation::send_moveat(double left, double right)
        {
                double left_angular_speed = settings_.convert_to_angular_speed(left);
                double right_angular_speed = settings_.convert_to_angular_speed(right);
                r_debug("Navigation: Angular speed target now (%.2f, %.2f)",
                        left_angular_speed, right_angular_speed);
                return driver_.moveat(left_angular_speed, right_angular_speed);
        }

    bool Navigation::send_moveat(double left, double right)
    {
        // Convert the speeds to normalized speeds. The driver
        // uses normalized angular speeds which amounts to the
        // same thing.
        left /= settings_.maximum_speed;
        right /= settings_.maximum_speed;

        // Clamp to [-1,1]
        left = std::clamp(left, -1.0, 1.0);
        right = std::clamp(right, -1.0, 1.0);

        return driver_.moveat(left, right);
    }

        bool Navigation::do_move(double distance, double speed)
        {
                bool success = false;

                if (distance == 0.0) {
                        success = true;

                } else if (speed != 0.0
                           && speed >= -1.0
                           && speed <= 1.0
                           && distance >= -50.0 // 50 meters max!
                           && distance <= 50.0) {

                        if (distance * speed >= 0.0) {
                                // All is well, moving forward
                                distance = fabs(distance);
                                speed = fabs(speed);
                        } else {
                                // Moving backwards. Make sur the
                                // distance is positive and the speed
                                // negative.
                                distance = -fabs(distance);
                                speed = -fabs(speed);
                        }

                        if (driver_.stop()) {

                                success = travel(distance, speed);

                        } else {
                                r_err("Navigation::do_move: stop failed");
                        }

                } else {
                        r_err("Navigation::do_move: invalid speed or distance: "
                              "speed=%f, distance=%f", speed, distance);
                }

                // A bit of a hack: in any case, make sure that the
                // rover comes to a standstill.
                driver_.stop();

                return success;
        }

        bool Navigation::travel(double distance, double speed)
        {
                bool success = false;

                try {
                        double timeout = compute_timeout(distance, speed);
                        success = try_travel(speed, distance, timeout);

                } catch (const std::runtime_error &re) {
                        r_err("Navigation::travel: %s", re.what());
                }

                stop();
        }
        
        // TODO: Spin off a seperate thread so the main event loop can continue?
        bool Navigation::try_travel(double speed, double distance, double timeout)
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double start_time = clock->time();
                v3 start_location;
                double left_speed = speed;
                double right_speed = speed;
                bool success = false;
                double last_distance = 0.0;
                double now;

                /*
                  Estimate the distance needed to slow down from
                  travel speed to stand-still.

                  v1 = v0 - a.dt (v0=speed; v1=0)
                  => 0 = v - a.dt => dt = v/a
                
                  x1 = x0 + v0.dt - 0.5 a.dt² (x0=0, v0=speed, dt=v/a)
                  => x = 0.5 v²/a
                */
                double slowdown_distance = (0.5 * speed * speed
                                             / settings_.maximum_acceleration);
                r_debug("Navigation: slowdown_distance %f", slowdown_distance);
                if (slowdown_distance < kDistanceSlowNavigation)
                        slowdown_distance = kDistanceSlowNavigation;

                stop_ = false;
                
                if (!distance_measure_.set_distance_to_navigate(distance)) {
                        r_err("Navigation::travel: pose estimation failed");
                        return false;
                }

                now = clock->time();
                log_data(now, kSpeedName, speed);
                log_data(now, kDistanceName, distance);


                ////////////////////////
                auto steering_serial = romiserial::RomiSerialClient::create("/dev/ttyACM0");
                romi::StepperController steering_controller(steering_serial);
                double max_rpm = 500;
                double max_rps = max_rpm / 60.0;
                double default_rps = max_rps / 2.0;
                double steps_per_revolution = 200; 
                double microsteps = 8; 
                double gears = 77.0; 
                        
                double steering_steps_per_revolution = steps_per_revolution * microsteps * gears;
                double steering_millis_per_step = 1000.0 / (default_rps * steps_per_revolution * microsteps);
                double width = 1.0;
                double length = 1.4;
                Steering steering(width, length);

                int32_t steering_pos0[3];
                steering_controller.get_position(steering_pos0);
                ////////////////////////

                
                while (!stop_) {

                        // TODO: HANDLE USER INPUT AND HANDLE STOP
                        // REQUESTED USER-REQUESTED

                        // How far off-track is the rover?
                        if (!track_follower_.update_error_estimate()) {
                                r_err("Navigation::travel: track error estimation failed");
                                break;
                        }
                        
                        double cross_track_error = track_follower_.get_cross_track_error();
                        double orientation_error = track_follower_.get_orientation_error();

                        // By how much should the wheel speeds be
                        // adapted to get the rover back on the track?
                        double correction = navigation_controller_.estimate_correction(
                                cross_track_error, orientation_error);
                        
                        left_speed = speed * (1.0 - correction);
                        right_speed = speed * (1.0 + correction);

                        ////////////////////////
                        double R = width / (2.0 * correction);
                        steering.turn(speed, R);

                        int16_t steps_left = (int16_t) (steering.left_angle_
                                                        * steering_steps_per_revolution
                                                        / (2.0 * M_PI));
                        int16_t steps_right = (int16_t) (steering.right_angle_
                                                         * steering_steps_per_revolution
                                                         / (2.0 * M_PI));

                        int16_t max_steps = steps_left > steps_right? steps_left : steps_right; 
                        int16_t dt = (int16_t) ceil(steering_millis_per_step
                                                    * (double) max_steps);
                        
                        steering_controller.moveto(dt,
                                                   (int16_t)(steps_left + steering_pos0[0]),
                                                   (int16_t)(steps_right + steering_pos0[1]),
                                                   (int16_t) steering_pos0[2]); 
                        ////////////////////////
                        
                        
                        // Make sure the speeds don't pass the maximum speed.
                        if (left_speed > settings_.maximum_speed) {
                                double scale = settings_.maximum_speed / left_speed;
                                left_speed *= scale;
                                right_speed *= scale;
                        }
                        if (right_speed > settings_.maximum_speed) {
                                double scale = settings_.maximum_speed / right_speed;
                                left_speed *= scale;
                                right_speed *= scale;
                        }
                        
                        now = clock->time();
                        log_data(now, kCrossTrackErrorName, cross_track_error);
                        log_data(now, kOrientationErrorName, orientation_error);
                        log_data(now, kCorrectionName, correction);
                        log_data(now, kLeftSpeedName, left_speed);
                        log_data(now, kRightSpeedName, right_speed);
                                
                        // Keep going
                        if (!set_speed_targets(left_speed, right_speed)) {
                                r_err("Navigation::travel: moveat failed");
                                break;
                        }
                        
                        // Where are we now?
                        if (!distance_measure_.update_distance_estimate()) {
                                r_err("Navigation::travel: distance estimation failed");
                                break;
                        }
                        double distance_to_end = distance_measure_.get_distance_to_end();

                        // If the rover is close to the end, force a slow-down.
                        if (distance_to_end <= slowdown_distance
                            && fabs(speed) > kSlowNavigationSpeed) {
                                if (speed > 0.0) {
                                        left_speed = kSlowNavigationSpeed;
                                        right_speed = kSlowNavigationSpeed;
                                } else {
                                        left_speed = -kSlowNavigationSpeed;
                                        right_speed = -kSlowNavigationSpeed;
                                }
                        }

                        // The rover has arrived
                        if (distance_to_end <= 0.0) {
                                success = true;
                                break;
                        }

                        now = clock->time();
                        if (now - start_time >= timeout) {
                                r_err("Navigation::travel: time out (%f s)", timeout);
                                break;
                        }

                        if (last_distance != distance_measure_.get_distance_from_start()) {
                                r_debug("Distance: from start: %f, to end: %f",
                                        distance_measure_.get_distance_from_start(),
                                        distance_measure_.get_distance_to_end());
                                last_distance = distance_measure_.get_distance_from_start();
                        }

                        clock->sleep(0.020);
                }

        return success;
    }

    double Navigation::compute_timeout(double distance, double absolute_speed)
    {
        double timeout = 0.0;
        if (absolute_speed != 0.0) {
            double time = fabs(distance) / fabs(absolute_speed);
            timeout = 7.0 + 2.0 * time;

            r_debug("Navigation::compute_timeout: v=%f, v_max=%f, "
                    "dist=%f, dt=%f, timeout=%f", absolute_speed,
                    settings_.maximum_speed, distance, time, timeout);
        }
        return timeout;
    }

    // TODO: Spin off a seperate thread so the main event loop can continue?
    bool Navigation::try_travel(double speed, double distance, double timeout) {
        auto clock = rpp::ClockAccessor::GetInstance();
        double start_time = clock->time();
        v3 start_location;
        std::vector<NavRecording> recording;
        double left_speed = speed;
        double right_speed = speed;
        bool success = false;
        double last_distance = 0.0;

        /*
          Estimate the distance needed to slow down from
          travel speed to stand-still.

          v1 = v0 - a.dt (v0=speed; v1=0)
          => 0 = v - a.dt => dt = v/a

          x1 = x0 + v0.dt - 0.5 a.dt² (x0=0, v0=speed, dt=v/a)
          => x = 0.5 v²/a
        */
        double slowdown_distance = (0.5 * speed * speed
                                    / settings_.maximum_acceleration);
        r_debug("Navigation: slowdown_distance %f", slowdown_distance);
        if (slowdown_distance < kDistanceSlowNavigation)
            slowdown_distance = kDistanceSlowNavigation;

        stop_ = false;

        if (!distance_measure_.set_distance_to_navigate(distance)) {
            r_err("Navigation::travel: pose estimation failed");
            return false;
        }

        while (!stop_) {

            // Keep going
            if (!set_speed_targets(left_speed, right_speed)) {
                r_err("Navigation::travel: moveat failed");
                break;
            }

            // TODO: HANDLE USER INPUT AND HANDLE STOP
            // REQUESTED USER-REQUESTED

            // How far off-track is the rover?
            if (!track_follower_.update_error_estimate()) {
                r_err("Navigation::travel: track error estimation failed");
                break;
            }

            double cross_track_error = track_follower_.get_cross_track_error();
            double orientation_error = track_follower_.get_orientation_error();

            // By how much should the wheel speeds be
            // adapted to get the rover back on the track?
            double correction = navigation_controller_.estimate_correction(
                    cross_track_error, orientation_error);
            left_speed = speed * (1.0 + correction);
            right_speed = speed * (1.0 - correction);

            // Make sure the speeds don't pass the maximum speed.
            if (left_speed > settings_.maximum_speed) {
                double scale = settings_.maximum_speed / left_speed;
                left_speed *= scale;
                right_speed *= scale;
            }
            if (right_speed > settings_.maximum_speed) {
                double scale = settings_.maximum_speed / right_speed;
                left_speed *= scale;
                right_speed *= scale;
            }

            // Where are we now?
            if (!distance_measure_.update_distance_estimate()) {
                r_err("Navigation::travel: distance estimation failed");
                break;
            }
            double distance_to_end = distance_measure_.get_distance_to_end();

            // If the rover is close to the end, force a slow-down.
            if (distance_to_end <= slowdown_distance
                && fabs(speed) > kSlowNavigationSpeed) {
                if (speed > 0.0) {
                    left_speed = kSlowNavigationSpeed;
                    right_speed = kSlowNavigationSpeed;
                } else {
                    left_speed = -kSlowNavigationSpeed;
                    right_speed = -kSlowNavigationSpeed;
                }
            }

            // The rover has arrived
            if (distance_to_end <= 0.0) {
                success = true;
                break;
            }

            double now = clock->time();
            if (now - start_time >= timeout) {
                r_err("Navigation::travel: time out (%f s)", timeout);
                break;
            }

            if (last_distance != distance_measure_.get_distance_from_start()) {
                r_debug("Distance: from start: %f, to end: %f",
                        distance_measure_.get_distance_from_start(),
                        distance_measure_.get_distance_to_end());
                last_distance = distance_measure_.get_distance_from_start();
            }

            recording.emplace_back(now - start_time,
                                   distance_measure_.get_distance_from_start(),
                                   cross_track_error,
                                   orientation_error,
                                   correction,
                                   left_speed,
                                   right_speed);

            clock->sleep(0.025);
        }

        set_speed_targets(0.0, 0.0);
        left_speed = 0.0;
        right_speed = 0.0;

        FILE *fp = fopen("nav.csv", "w");
        if (fp) {
            fprintf(fp,
                    "# time\tx\ty\tdistance\tcross-track error\torientation\t"
                    "correction\tleft speed\tright speed\n");
            for (size_t i = 0; i < recording.size(); i++) {
                fprintf(fp,
                        "%.6f\t%.6f\t%.6f\t%.6f\t"
                        "%.6f\t%.6f\t%.6f\n",
                        recording[i].time_,
                        recording[i].distance_,
                        recording[i].cross_track_error_,
                        recording[i].orientation_error_,
                        recording[i].correction_,
                        recording[i].left_speed_,
                        recording[i].right_speed_);


                printf("%.6f\t%.6f\t%.6f\t%.6f\t"
                       "%.6f\t%.6f\t%.6f\n",
                       recording[i].time_,
                       recording[i].distance_,
                       recording[i].cross_track_error_,
                       recording[i].orientation_error_,
                       recording[i].correction_,
                       recording[i].left_speed_,
                       recording[i].right_speed_);
            }
            fclose(fp);
        }

        return success;
    }

    bool Navigation::moveat(double left, double right) {
        SynchronizedCodeBlock sync(mutex_);
        bool success = false;
        if (status_ == MOVEAT_CAPABLE)
            success = set_speed_targets(left, right);
        else
            r_warn("Navigation::moveat: still moving");
        return success;
    }

    bool Navigation::move(double distance, double speed) {
        bool success = false;
        {
            SynchronizedCodeBlock sync(mutex_);
            if (status_ == MOVEAT_CAPABLE) {
                status_ = MOVING;
            } else {
                r_warn("Navigation::move: already moving");
            }
        }

        if (status_ == MOVING)
            success = do_move(distance, speed);

        status_ = MOVEAT_CAPABLE;

        return success;
    }

    bool Navigation::stop() {
        // This flag should assure that we break out
        // the travel loop.
        stop_ = true;
        bool success = driver_.stop();
        status_ = MOVEAT_CAPABLE;
        return success;
    }

    bool Navigation::pause_activity() {
        r_warn("*** Navigation::pause_activity NOT YET IMPLEMENTED");
        stop();
        return true;
    }

    bool Navigation::continue_activity() {
        r_warn("*** Navigation::continue_activity NOT YET IMPLEMENTED");
        return true;
    }

    bool Navigation::reset_activity() {
        stop();
        return true;
    }

    bool Navigation::enable_controller() {
        return driver_.enable_controller();
    }

    bool Navigation::disable_controller() {
        return driver_.disable_controller();
    }
}
