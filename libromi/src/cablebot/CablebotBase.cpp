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
#include <limits.h>
#include <stdexcept>
#include <log.h>
#include <ClockAccessor.h>
#include "cablebot/CablebotBase.h"
#include <json.hpp>

namespace romi {
        
        CablebotBase::CablebotBase(std::unique_ptr<romiserial::IRomiSerialClient>& motor_serial)
                : motor_serial_(std::move(motor_serial)),
                  range_xyz_(),
                  range_angles_(),
                  diameter_(kDiameter),
                  circumference_(0.0)
        {
                circumference_ = diameter_ * M_PI;
                range_xyz_.init(v3(0.0), v3(100.0, 0.0, 0.0)); // TODO
                range_angles_.init(v3(-90.0, 0.0, 0.0), v3(90.0, 0.0, 0.0));
        }

        int16_t CablebotBase::position_to_steps(double x)
        {
                double turns = x / circumference_;
                double steps = kPrecision * turns;
                r_info("position_to_steps: position: %0.3f, diameter: %0.5f, "
                       "turns: %0.3f, steps: %0.1f, precision: %0.3f",
                       x, diameter_, turns, steps, kPrecision);
                if (steps > SHRT_MAX || steps < SHRT_MIN) {
                        r_err("CablebotBase::position_to_steps: steps out of range: "
                              "position: %0.3f, turns: 0.3f, "
                              "steps: %0.1f, precision: %0.3f",
                              x, turns, steps, kPrecision);
                        throw std::runtime_error("CablebotBase::position_to_turns: "
                                                 "steps out of range");
                }
                return (int16_t) steps;
        }
        
        double CablebotBase::steps_to_position(double steps)
        {
                double turns = (double) steps / kPrecision;
                return turns * circumference_;
        }
        
        bool CablebotBase::get_range(CNCRange &xyz, IRange &angles)
        {
                xyz = range_xyz_;
                angles = range_angles_;
                return true;
        }
        
        bool CablebotBase::send_motor_command(const char *command)
        {
                bool success = false;
                nlohmann::json response;
                
                motor_serial_->send(command, response);
                
                if (response.is_array()
                    && response[0].is_number()) {

                        success = (response[romiserial::kStatusCode] == 0);
                        if (!success) {
                                std::string message = response[romiserial::kErrorMessage];
                                r_err("CablebotBase::send_motor_command: %s: %s",
                                      command, message.c_str());
                        }
                }
                return success;
        }
        
        bool CablebotBase::moveto(double x, double y, double z, double relative_speed)
        {
                bool success = false;
                try {
                        validate_xyz_coordinates(x, y, z);
                        validate_angles(ax, ay, az);
                        validate_speed(relative_speed);
                        try_moveto(x, ax, relative_speed);
                        success = true;
                        
                } catch (std::runtime_error& e) {
                        r_err("CablebotBase::moveto: %s", e.what());
                }
                return success;
        }
        
        void CablebotBase::try_moveto(double x, double ax, double relative_speed)
        {
                motor_moveto(x, relative_speed);
                synchronize_with_motor(180.0); // Fixme: compute as distance x speed x factor
        }
        
        void CablebotBase::validate_xyz_coordinates(double x, double y, double z)
        {
                if (!range_xyz_.is_inside(x, y, z)) {
                        r_err("Cablebotbase::validate_xyz_coordinates: "
                              "Position out of range: "
                              "(%.3f,%.3f,%.3f)", x, y, z);
                        throw std::runtime_error("Cablebot::validate_xyz_coordinates: "
                                                 "Position out of range");
                }
        }
        
        void CablebotBase::validate_angles(double ax, double ay, double az)
        {
                if (!range_xyz_.is_inside(ax, ay, az)) {
                        r_err("Cablebotbase::validate_angles: "
                              "Angles out of range: "
                              "(%.3f,%.3f,%.3f)", ax, ay, az);
                        throw std::runtime_error("Cablebot::validate_angles: "
                                                 "Angles out of range");
                }
        }
        
        void CablebotBase::validate_speed(double v)
        {
                if (v < 0.0 || v > 1.0) {
                        r_err("CablebotBase::validate_speed: Invalid speed: %.3f", v);
                        throw std::runtime_error("Cablebot::validate_speed: "
                                                 "Invalid speed");
                }
        }
        
        void CablebotBase::motor_moveto(double x, double relative_speed)
        {
                (void) relative_speed; // TODO: improve firmware API for speeds
                char command[64];
                snprintf(command, sizeof(command), "m[%d]", position_to_steps(x));
                if (!send_motor_command(command)) {
                        throw std::runtime_error("Cablebot::send_motor_command: "
                                                 "send_motor_command failed");
                }
        }
        
        void CablebotBase::synchronize_with_motor(double timeout)
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double start_time = clock->time();
                
                while (true) {
                        if (is_motor_on_target()) {
                                break;
                        } else {
                                clock->sleep(0.2);
                        }

                        double now = clock->time();
                        if (timeout >= 0.0 && (now - start_time) >= timeout) {
                                throw std::runtime_error("CablebotBase::synchronize_with_motor: "
                                                         "time out");
                        }
                }
        }
        
        bool CablebotBase::is_motor_on_target()
        {
                nlohmann::json response;
                motor_serial_->send("S", response);

                bool success = (response[0] == 0.0);
                if (!success) {
                        std::string message = response[1];
                        r_err("CablebotBase::is_motor_on_target: %s", message.c_str());
                        throw std::runtime_error("CablebotBase::is_motor_on_target");
                }

                int error = (int) response[1];
                bool on_target = (bool) (response[2] != 0.0);
                double distance = response[3];
                double voltage = response[4];
                double current = response[6];
                double position = response[7];
                r_debug("CablebotBase::is_motor_on_target: %s, distance: %f, position: %f, error: %d, voltage: %0.3f, current: %0.3f",
                        on_target? "yes" : "no", distance, position, error, voltage, current);
                return on_target;
        }
        
        bool CablebotBase::homing()
        {
                return send_motor_command("H");
        }
 
        bool CablebotBase::get_position(v3& xyz, v3& angles)
        {
                angles.set(0.0);
                return get_motor_position(xyz);
        }

        bool CablebotBase::get_motor_position(v3& position)
        {
            nlohmann::json response;
            motor_serial_->send("P", response);

            bool success = (response[romiserial::kStatusCode] == 0);
            if (success) {
                    double steps = response[1];
                    position.x(steps_to_position(steps));
                    position.y(0.0);
                    position.z(0.0);
            }
            return success;
        }
        
        bool CablebotBase::spindle(double speed)
        {
                (void) speed;
                r_err("CablebotBase::spindle: Not implemented");
                throw std::runtime_error("CablebotBase::spindle: Not implemented");
                return false;
        }
        
        bool CablebotBase::travel(Path &path, double relative_speed)
        {
                (void) path;
                (void) relative_speed;
                r_err("CablebotBase::travel: Not implemented");
                throw std::runtime_error("CablebotBase::travel: Not implemented");
                return false;
        }
        
        bool CablebotBase::helix(double xc, double yc, double alpha, double z,
                                 double relative_speed)
        {
                (void) xc;
                (void) yc;
                (void) alpha;
                (void) z;
                (void) relative_speed;
                r_err("CablebotBase::helix: Not implemented");
                throw std::runtime_error("CablebotBase::helix: Not implemented");
                return false;
        }

        bool CablebotBase::pause_activity()
        {
                r_err("CablebotBase::pause_activity: Not implemented");
                throw std::runtime_error("CablebotBase::pause_activity: Not implemented");
        }
        
        bool CablebotBase::continue_activity()
        {
                r_err("CablebotBase::continue_activity: Not implemented");
                throw std::runtime_error("CablebotBase::continue_activity: Not implemented");
        }
        
        bool CablebotBase::reset_activity()
        {
                r_err("CablebotBase::reset_activity: Not implemented");
                throw std::runtime_error("CablebotBase::reset_activity: Not implemented");
        }

        bool CablebotBase::enable_driver()
        {
                return send_motor_command("E[1]");
        }

        bool CablebotBase::disable_driver()
        {
                return send_motor_command("E[0]");
        }

        bool CablebotBase::power_up()
        {
                return enable_driver() && homing();
        }
        
        bool CablebotBase::power_down()
        {
                return disable_driver();
        }
        
        bool CablebotBase::stand_by()
        {
                return disable_driver();
        }
        
        bool CablebotBase::wake_up()
        {
                return enable_driver();
        }
}
