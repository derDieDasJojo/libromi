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
#ifndef __ROMI_STEERING_CONTROLLER_H_
#define __ROMI_STEERING_CONTROLLER_H_


#include <RomiSerialClient.h> 
#include <RSerial.h>

#include "rover/ISteeringController.h" 
#include "api/ActivityHelper.h"


namespace romi {
                
        class SteeringController : public ISteeringController
        {
        protected: //!!!A remettre en protected

                std::unique_ptr<romiserial::IRomiSerialClient> _romi_serial;
                std::mutex _mutex;
                std::condition_variable _continue_condition;
                ActivityHelper _activity_helper;
                
        public:
                
                static constexpr const char *ClassName = "steering-controller";
        
                SteeringController(std::unique_ptr<romiserial::IRomiSerialClient>& romi_serial);
                
                virtual ~SteeringController() override = default;

                bool get_position(int32_t *pos) override;
                bool homing() override;
                
                bool set_target(int16_t left, int16_t right) override;
                bool set_mode(SteeringMode mode) override;
                
                bool synchronize(double timeout) override;

                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;
                
                bool enable() override;
                bool disable() override;

                bool stop() override;

        protected:

                int send_command(const char *cmd);
                int is_idle();                
                bool response_ok(nlohmann::json& response);
                bool send_command_without_interruption(const char *command);
                bool check_response(const char *command, nlohmann::json& response);
        };
}

#endif // __ROMI_STEERING_CONTROLLER_H
