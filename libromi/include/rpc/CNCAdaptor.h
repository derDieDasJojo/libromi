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
#ifndef __ROMI_CNC_ADAPTER_H
#define __ROMI_CNC_ADAPTER_H

#include "rpc/IRPCHandler.h"
#include "api/ICNC.h"

namespace romi {
        
        class CNCAdaptor : public IRPCHandler
        {
        protected:
                ICNC &_cnc;
                
                void execute_get_range(JsonCpp& params, JsonCpp& result,
                                       RPCError &error);
                void execute_moveto(JsonCpp& params, JsonCpp& result,
                                    RPCError &error);
                void execute_spindle(JsonCpp& params, JsonCpp& result,
                                     RPCError &error);
                void execute_travel(JsonCpp& params, JsonCpp& result,
                                    RPCError &error);
                void execute_homing(RPCError &error);
                void execute_pause(RPCError &error);
                void execute_continue(RPCError &error);
                void execute_reset(RPCError &error);
                void execute_power_up(RPCError &error);
                void execute_power_down(RPCError &error);
                void execute_stand_by(RPCError &error);
                void execute_wake_up(RPCError &error);


        public:
                explicit CNCAdaptor(ICNC &cnc) : _cnc(cnc) {}
                ~CNCAdaptor() override = default;
                
                void execute(const std::string& method, JsonCpp& params,
                             JsonCpp& result, RPCError &error) override;
        };
}

#endif // __ROMI_CNC_ADAPTER_H
