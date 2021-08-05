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
#include <functional>
#include <log.h>
#include <util.h>
#include <ClockAccessor.h>
#include "hal/BrushMotorDriver.h"

namespace romi {
        
        BrushMotorDriver::BrushMotorDriver(std::unique_ptr<romiserial::IRomiSerialClient>& serial,
                                           JsonCpp &config,
                                           double max_angular_speed,
                                           double max_angular_acceleration)
                : serial_(),
                  settings_(),
                  max_angular_speed_(max_angular_speed),
                  recording_pid_(false),
                  pid_thread_(),
                  recording_speeds_(false),
                  speeds_thread_()
        {
                //_serial.set_debug(true);
                serial_ = std::move(serial);
                if (!disable_controller()
                    || !configure_controller(config,
                                             max_angular_speed,
                                             max_angular_acceleration)
                    || !enable_controller()) {
                        throw std::runtime_error("BrushMotorDriver: "
                                                 "Initialization failed");
                }
        }
        
        BrushMotorDriver::~BrushMotorDriver()
        {
                // stop_recording_pid();
                // stop_recording_speeds();
        }

        const BrushMotorDriverSettings& BrushMotorDriver::get_settings()
        {
                return settings_;
        }

        int32_t BrushMotorDriver::get_encoder_steps()
        {
                return settings_.encoder_steps;
        }
        
        bool BrushMotorDriver::configure_controller(JsonCpp &config,
                                                    double max_angular_speed,
                                                    double max_angular_acceleration)
        {
                settings_.parse(config);

                char command[100];
                rprintf(command, 100, "C[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]",
                        settings_.encoder_steps,
                        settings_.dir_left,
                        settings_.dir_right,
                        (int) (1000.0 * max_angular_speed),
                        (int) (1000.0 * max_angular_acceleration),
                        settings_.kp_numerator,
                        settings_.kp_denominator,
                        settings_.ki_numerator,
                        settings_.ki_denominator,
                        settings_.max_signal);
                        
                JsonCpp response;
                serial_->send(command, response);
                return check_response(command, response);
        }
        
        bool BrushMotorDriver::check_response(const char *command,
                                              JsonCpp& response)
        {
                bool success = (response.num(romiserial::kStatusCode) == 0);
                if (!success) {
                        const char *message = "No message";
                        if (response.length() > 1)
                                message = response.str(romiserial::kErrorMessage);
                        r_warn("BrushMotorDriver: command %s returned error: %s",
                               command, message);
                }
                return success;
        }

        bool BrushMotorDriver::enable_controller()
        {
                JsonCpp response;
                const char *command = "E[1]";
                serial_->send(command, response);
                return check_response(command, response);
        }

        bool BrushMotorDriver::disable_controller()
        {
                JsonCpp response;
                const char *command = "E[0]";
                serial_->send(command, response);
                return check_response(command, response);
        }

        bool BrushMotorDriver::stop()
        {
                JsonCpp response;
                const char *command = "X";
                serial_->send(command, response);
                return check_response(command, response);
        }

        bool BrushMotorDriver::moveat(double left, double right)
        {
                bool success = false;

                r_debug("BrushMotorDriver: moveat (%f, %f)", left, right);

                if (-max_angular_speed_ <= left  && left <= max_angular_speed_
                    && -max_angular_speed_  <= right && right <= max_angular_speed_) {
                                
                        int32_t ileft = (int32_t) (1000.0 * left);
                        int32_t iright = (int32_t) (1000.0 * right);
                                
                        char command[64];
                        rprintf(command, 64, "V[%d,%d]", ileft, iright);
                                
                        JsonCpp response;
                        serial_->send(command, response);
                        success = check_response(command, response);
                        
                } else {
                        r_warn("BrushMotorDriver::moveat: invalid speeds: (%f, %f)",
                               left, right);
                }

                return success;
        }

        bool BrushMotorDriver::get_encoder_values(double& left,
                                                  double& right,
                                                  double& timestamp)
        {
                JsonCpp response;
                const char *command = "e";
                serial_->send(command, response);
                bool success = check_response(command, response);
                if (success) {
                        left = response.num(1);
                        right = response.num(2);
                        timestamp = response.num(3) / 1000.0;
                }
                return success;
        }

        // void BrushMotorDriver::start_recording_pid()
        // {
        //         recording_pid_ = true;
        //         FILE* fp = fopen("pid.csv", "w");
        //         if (fp) {
        //                 fprintf(fp,
        //                         "# time\t"
        //                         "target\t"
        //                         "measured speed\t"
        //                         "output\t"
        //                         "error_p\t"
        //                         "error_i\t"
        //                         "error_d\t"
        //                         "controller input\n");
        //                 fclose(fp);
        //         }
                
        //         pid_thread_ = std::make_unique<std::thread>(
        //                 [this]() {
        //                         record_pid_main();
        //                 });
        // }

        // void BrushMotorDriver::stop_recording_pid()
        // {
        //         recording_pid_ = false;
        //         if (pid_thread_) {
        //                 pid_thread_->join();
        //                 pid_thread_ = nullptr;
        //         }
        // }

        // void BrushMotorDriver::record_pid_main()
        // {
        //         auto clock = rpp::ClockAccessor::GetInstance();
        //         double start_time = clock->time();
        //         std::vector<PidStatus> recording;
                
        //         while (recording_pid_) {
        //                 double now;
        //                 double target;
        //                 double measured_speed;
        //                 double output;
        //                 double error_p;
        //                 double error_i;
        //                 double error_d;
        //                 double controller_input;

        //                 now = clock->time();
        //                 bool success = get_pid_values(kLeftWheel, target,
        //                                               measured_speed, output, error_p,
        //                                               error_i, error_d, controller_input);

        //                 if (success) {
        //                         recording.emplace_back(now - start_time, target,
        //                                                measured_speed, output, error_p,
        //                                                error_i, error_d, controller_input);
        //                         if (recording.size() >= 100) {
        //                                 store_pid_recordings(recording);
        //                                 recording.clear();
        //                         }
        //                 }
                        
        //                 clock->sleep(0.050);
        //         }
                
        //         store_pid_recordings(recording);
        // }

        // bool BrushMotorDriver::get_pid_values(Axis axis,
        //                                       double& target_speed,
        //                                       double& measured_speed,
        //                                       double& pid_output,
        //                                       double& pid_error_p,
        //                                       double& pid_error_i,
        //                                       double& pid_error_d,
        //                                       double& controller_input)
        // {
        //         JsonCpp response;
        //         char command[16];
        //         snprintf(command, sizeof(command), "p[%d]", axis);

        //         serial_->send(command, response);
        //         bool success = check_response(command, response);
        //         if (success) {
        //                 target_speed = response.num(1);
        //                 measured_speed = response.num(2);
        //                 pid_output = response.num(3);
        //                 pid_error_p = response.num(4);
        //                 pid_error_i = response.num(5);
        //                 pid_error_d = response.num(6);
        //                 controller_input = response.num(7);
        //         }
        //         return success;
        // }

        // void BrushMotorDriver::store_pid_recordings(std::vector<PidStatus>& recording)
        // {
        //         FILE* fp = fopen("speeds.csv", "a");
        //         if (fp) {
        //                 for (size_t i = 0; i < recording.size(); i++)
        //                         fprintf(fp,
        //                                 "%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n",
        //                                 recording[i].time_,
        //                                 recording[i].target_,
        //                                 recording[i].measured_speed_,
        //                                 recording[i].output_,
        //                                 recording[i].error_p_,
        //                                 recording[i].error_i_,
        //                                 recording[i].error_d_,
        //                                 recording[i].controller_input_);
        //                 fclose(fp);
        //         }
                
        // }

        // void BrushMotorDriver::start_recording_speeds()
        // {
        //         recording_speeds_ = true;
        //         FILE* fp = fopen("speeds.csv", "w");
        //         if (fp) {
        //                 fprintf(fp, "# time\tleft\tright\n");
        //                 fclose(fp);
        //         }
                
        //         speeds_thread_ = std::make_unique<std::thread>(
        //                 [this]() {
        //                         record_speeds_main();
        //                 });
        // }

        // void BrushMotorDriver::stop_recording_speeds()
        // {
        //         recording_speeds_ = false;
        //         if (speeds_thread_) {
        //                 speeds_thread_->join();
        //                 speeds_thread_ = nullptr;
        //         }
        // }

        // void BrushMotorDriver::record_speeds_main()
        // {
        //         auto clock = rpp::ClockAccessor::GetInstance();
        //         double start_time = clock->time();
        //         std::vector<Speeds> recording;
                
        //         while (recording_speeds_) {
        //                 double now;
        //                 double left_absolute;
        //                 double right_absolute;
        //                 double left_normalized;
        //                 double right_normalized;

        //                 now = clock->time();
        //                 bool success = get_speeds_values(left_absolute, right_absolute,
        //                                                  left_normalized, right_normalized);

        //                 if (success) {
        //                         recording.emplace_back(now - start_time,
        //                                                left_absolute, right_absolute,
        //                                                left_normalized, right_normalized);
        //                         if (recording.size() >= 100) {
        //                                 store_speed_recordings(recording);
        //                                 recording.clear();
        //                         }
        //                 }
                        
        //                 clock->sleep(0.020);
        //         }
                
        //         store_speed_recordings(recording);
        // }

        // bool BrushMotorDriver::get_speeds_values(double& left_absolute,
        //                                          double& right_absolute,
        //                                          double& left_normalized,
        //                                          double& right_normalized)
        // {
        //         JsonCpp response;
        //         char command[16];
        //         snprintf(command, sizeof(command), "v");

        //         serial_->send(command, response);
        //         bool success = check_response(command, response);
        //         if (success) {
        //                 // These speeds are driver works with speeds,
        //                 // not m/s.
        //                 left_absolute = response.num(1);
        //                 right_absolute = response.num(2);

        //                 // The angular speed as a factor of the
        //                 // maximum angular speed (1 = maximum speed)
        //                 left_normalized = response.num(3);
        //                 right_normalized = response.num(4);
        //         }
        //         return success;
        // }

        // void BrushMotorDriver::store_speed_recordings(std::vector<Speeds>& recording)
        // {
        //         FILE* fp = fopen("speeds.csv", "a");
        //         if (fp) {
        //                 for (size_t i = 0; i < recording.size(); i++)
        //                         fprintf(fp,
        //                                 "%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n",
        //                                 recording[i].time_,
        //                                 recording[i].left_absolute_,
        //                                 recording[i].right_absolute_,
        //                                 recording[i].left_normalized_,
        //                                 recording[i].right_normalized_);
        //                 fclose(fp);
        //         }
                
        // }

        bool BrushMotorDriver::get_speeds(double& left_target, double& right_target,
                                          double& left_current, double& right_current,
                                          double& left_measured, double& right_measured)
        {
                JsonCpp response;
                char command[16];
                snprintf(command, sizeof(command), "v");

                serial_->send(command, response);
                bool success = check_response(command, response);
                if (success) {
                        left_target = response.num(1) / 1000.0;
                        right_target = response.num(2) / 1000.0;
                        left_current = response.num(3) / 1000.0;
                        right_current = response.num(4) / 1000.0;
                        left_measured = response.num(5) / 1000.0;
                        right_measured = response.num(6) / 1000.0;
                }
                return success;
        }
}
