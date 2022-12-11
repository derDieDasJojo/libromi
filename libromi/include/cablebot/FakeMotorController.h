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
#ifndef __ROMI_FAKEMOTORCONTROLLER_H
#define __ROMI_FAKEMOTORCONTROLLER_H

#if !defined(ARDUINO)

#include <string>
#include <memory>
#include <mutex>
#include <MessageParser.h>
#include <RomiSerialErrors.h>
#include <IRomiSerialClient.h>

namespace romi {
        
        class FakeMotorController : public romiserial::IRomiSerialClient
        {
        protected:
                const std::string client_name_;
                romiserial::MessageParser parser_;
                bool debug_;
                bool enabled_;
                int32_t position_;

                void handle_command(nlohmann::json& response);
                void handle_moveto(nlohmann::json& response);
                void send_position(nlohmann::json& response);
                void send_on_position(nlohmann::json& response);
                void handle_homing(nlohmann::json& response);
                void handle_enable(nlohmann::json& response);
                void send_status(nlohmann::json& response);
                void handle_restart(nlohmann::json& response);
                void send_info(nlohmann::json& response);
                void make_response(nlohmann::json& response, int code);
                void make_response(nlohmann::json& response, int code, const char *message);
                void make_response(nlohmann::json& response, int code, int32_t value);
                
        public:
        
                explicit FakeMotorController(const std::string& client_name);
                FakeMotorController(const FakeMotorController&) = delete;
                FakeMotorController& operator=(const FakeMotorController&) = delete;
                ~FakeMotorController() override = default;

                void send(const char *command, nlohmann::json& response) override;        
                void set_debug(bool value) override;
        };
}

#endif 
#endif // __ROMI_FAKEMOTORCONTROLLER_H
