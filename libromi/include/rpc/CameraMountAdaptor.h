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

#ifndef __ROMI_CAMERAMOUNTADAPTOR_H
#define __ROMI_CAMERAMOUNTADAPTOR_H

#include <string>
#include "rpc/IRPCHandler.h"
#include "hal/ICameraMount.h"

namespace romi {

        class CameraMountAdaptor : public IRPCHandler
        {
        protected:
                ICameraMount& mount_;
                
                void execute_moveto(nlohmann::json& params, RPCError &error);
                void execute_get_position(nlohmann::json& result, RPCError &error);
                void execute_get_range(nlohmann::json& result, RPCError &error);
                void execute_homing(RPCError &error);
                
                void execute_stop(RPCError &error);
                void execute_pause(RPCError &error);
                void execute_continue(RPCError &error);
                void execute_reset(RPCError &error);
                void execute_power_up(RPCError &error);
                void execute_power_down(RPCError &error);
                void execute_stand_by(RPCError &error);
                void execute_wake_up(RPCError &error);

        public:
                explicit CameraMountAdaptor(ICameraMount& client);
                ~CameraMountAdaptor() override = default;

                void execute(const std::string& method,
                             nlohmann::json& params,
                             nlohmann::json& result,
                             RPCError &error) override;
                void execute(const std::string& method,
                             nlohmann::json &params,
                             rcom::MemBuffer& result,
                             RPCError &status) override;
        };
}

#endif // __ROMI_CAMERAMOUNTADAPTOR_H
