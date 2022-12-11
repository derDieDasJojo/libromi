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

#include "cablebot/FakeMotorController.h"
#include "util/Logger.h"

namespace romi {

        FakeMotorController::FakeMotorController(const std::string& client_name)
                :   client_name_(client_name),
                    parser_(),
                    debug_(true),
                    enabled_(false),
                    position_(0)
        {
        }

        void FakeMotorController::make_response(nlohmann::json& response, int code)
        {
                response = nlohmann::json::array({code});
        }
        
        void FakeMotorController::make_response(nlohmann::json& response, int code,
                                                const char *message)
        {
                response = nlohmann::json::array({code, message});
        }
        
        void FakeMotorController::make_response(nlohmann::json& response, int code,
                                                int32_t value)
        {
                response = nlohmann::json::array({code, value});
        }

        void FakeMotorController::send(const char *command, nlohmann::json& response)
        {
                std::string request;

                // len must include the \0 character at the end of the string
                size_t len = strlen(command) + 1; 
                r_debug("FakeMotorController::send: '%s', len=%d", command, (int) len);
                
                bool success = parser_.parse(command, len); 
                if (success) {
                        handle_command(response);
                } else {
                        make_response(response, parser_.error(), "Message parsing failed");
                }
        }

        void FakeMotorController::handle_command(nlohmann::json& response)
        {
                /*
                  { 'm', 1, false, handle_moveto },
                  { 'P', 0, false, send_position },
                  { 'p', 0, false, send_on_position },
                  { 'H', 0, false, handle_homing },
                  { 'E', 1, false, handle_enable },
                  { 'S', 0, false, send_status },
                  { 'R', 0, false, handle_restart },
                  { '?', 0, false, send_info },
                */
                switch (parser_.opcode()) {
                case 'm':
                        handle_moveto(response);
                        break;
                case 'P':
                        send_position(response);
                        break;
                case 'p':
                        send_on_position(response);
                        break;
                case 'H':
                        handle_homing(response);
                        break;
                case 'E':
                        handle_enable(response);
                        break;
                case 'S':
                        send_status(response);
                        break;
                case 'R':
                        handle_restart(response);
                        break;
                case '?':
                        send_info(response);
                        break;
                default:
                        make_response(response, romiserial::kUnknownOpcode,
                                      "Unknown opcode");
                        break;
                }
        }

        void FakeMotorController::handle_moveto(nlohmann::json& response)
        {
                if (parser_.length() == 1) {
                        if (enabled_) {
                                position_ = parser_.value(0);
                        }
                        make_response(response, 0);
                } else {
                        make_response(response, romiserial::kBadNumberOfArguments,
                                      "Bad number of arguments");
                }
        }

        void FakeMotorController::send_position(nlohmann::json& response)
        {
                make_response(response, 0, position_);
        }

        void FakeMotorController::send_on_position(nlohmann::json& response)
        {
                make_response(response, 0, 0);
        }

        void FakeMotorController::handle_homing(nlohmann::json& response)
        {
                position_ = 0;
                make_response(response, 0);
        }

        void FakeMotorController::handle_enable(nlohmann::json& response)
        {
                if (parser_.length() == 1) {
                        enabled_ = (parser_.value(0) != 0);
                        make_response(response, 0);
                } else {
                        make_response(response, romiserial::kBadNumberOfArguments,
                                      "Bad number of arguments");
                }
        }

        void FakeMotorController::send_status(nlohmann::json& response)
        {
                int state_error = 0;
                int on_target = 1;
                float distance = 0.0f;
                float voltage = 16.8f;
                float current_limit = -1.0f;
                float current = -1.0f;
                float position = (float) position_;
                int axis_error = 0;
                int ctrl_error = 0;
                int motor_error = 0;
                float version = 0.1f;
                response = nlohmann::json::array({0, state_error, on_target, distance,
                                voltage, current_limit, current, position, axis_error,
                                ctrl_error, motor_error, version});
        }

        void FakeMotorController::handle_restart(nlohmann::json& response)
        {
                make_response(response, 0);
        }

        void FakeMotorController::send_info(nlohmann::json& response)
        {
                response = nlohmann::json::array({0, "CablebotBase", "0.1", "FAKE"});
        }
        
        void FakeMotorController::set_debug(bool value)
        {
                debug_ = value;
        }
        
}
