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

#include <log.h>
#include "rpc/GimbalAdaptor.h"
#include "rpc/MethodsGimbal.h"
#include "rpc/MethodsPowerDevice.h"
#include "rpc/MethodsActivity.h"

namespace romi {
        
        GimbalAdaptor::GimbalAdaptor(IGimbal& gimbal)
                : gimbal_(gimbal)
        {
        }

        void GimbalAdaptor::execute(const std::string& method,
                                    nlohmann::json &params,
                                    rcom::MemBuffer& result,
                                    RPCError &error)
        {
                (void) method;
                (void) params;
                (void) result;
                error.code = RPCError::kMethodNotFound;
                error.message = "Unknown method";
        }
        
        void GimbalAdaptor::execute(const std::string& method,
                                    nlohmann::json& params,
                                    nlohmann::json& result,
                                    RPCError &error)
        {
                r_debug("GimbalAdaptor::execute");
                
                error.code = 0;
                
                try {

                        if (method == MethodsGimbal::moveto) {
                                execute_moveto(params, error);
                        
                        } else if (method == MethodsGimbal::moveat) {
                                execute_moveat(params, error);
                        
                        } else if (method == MethodsGimbal::get_angle) {
                                execute_get_angle(result, error);
                        
                        } else if (method == MethodsGimbal::set_angle) {
                                execute_set_angle(params, error);
                        
                        } else if (method == MethodsActivity::activity_pause) {
                                execute_pause(error);
                                
                        } else if (method == MethodsActivity::activity_continue) {
                                execute_continue(error);
                                
                        } else if (method == MethodsActivity::activity_reset) {
                                execute_reset(error);
                                
                        } else if (method == MethodsPowerDevice::power_up) {
                                execute_power_up(error);
                                
                        } else if (method == MethodsPowerDevice::power_down) {
                                execute_power_down(error);
                                
                        } else if (method == MethodsPowerDevice::stand_by) {
                                execute_stand_by(error);
                                
                        } else if (method == MethodsPowerDevice::wake_up) {
                                execute_wake_up(error);
                                
                        } else {
                                error.code = RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }
                        
                } catch (std::exception &e) {
                        error.code = RPCError::kInternalError;
                        error.message = e.what();
                }
        }

        void GimbalAdaptor::execute_moveto(nlohmann::json& params, RPCError &error)
        {
                r_debug("GimbalAdaptor::execute_moveto");
                double angle = params[MethodsGimbal::angle_param];
                if (!gimbal_.moveto(angle)) {
                        error.code = 1;
                        error.message = "Moveto failed";
                }
        }

        void GimbalAdaptor::execute_moveat(nlohmann::json& params, RPCError &error)
        {
                r_debug("GimbalAdaptor::execute_moveat");
                double rps = params[MethodsGimbal::rps_param];
                if (!gimbal_.moveat(rps)) {
                        error.code = 1;
                        error.message = "Moveat failed";
                }
        }

        void GimbalAdaptor::execute_get_angle(nlohmann::json& result, RPCError &error)
        {
                r_debug("GimbalAdaptor::get_angle");
                double angle;
                if (gimbal_.get_angle(angle)) {
                        result[MethodsGimbal::angle_result] = angle;
                } else {
                        error.code = 1;
                        error.message = "Get position failed";
                }
        }

        void GimbalAdaptor::execute_set_angle(nlohmann::json& params, RPCError &error)
        {
                r_debug("GimbalAdaptor::set_angle");
                double angle = params[MethodsGimbal::angle_param];
                if (!gimbal_.set_angle(angle)) {
                        error.code = 1;
                        error.message = "set_angle failed";
                }
        }

        void GimbalAdaptor::execute_pause(RPCError &error)
        {
                r_debug("GimbalAdaptor::execute_pause");
                if (!gimbal_.pause_activity()) {
                        error.code = 1;
                        error.message = "stop failed";
                }
        }

        void GimbalAdaptor::execute_continue(RPCError &error)
        {
                r_debug("GimbalAdaptor::execute_continue");
                if (!gimbal_.continue_activity()) {
                        error.code = 1;
                        error.message = "continue failed";
                }
        }

        void GimbalAdaptor::execute_reset(RPCError &error)
        {
                r_debug("GimbalAdaptor::execute_reset");
                if (!gimbal_.reset_activity()) {
                        error.code = 1;
                        error.message = "reset failed";
                }
        }

        void GimbalAdaptor::execute_power_up(RPCError &error)
        {
                r_debug("GimbalAdaptor::power_up");
                if (!gimbal_.power_up()) {
                        error.code = 1;
                        error.message = "power up failed";
                }
        }
        
        void GimbalAdaptor::execute_power_down(RPCError &error)
        {
                r_debug("GimbalAdaptor::power_down");
                if (!gimbal_.power_down()) {
                        error.code = 1;
                        error.message = "power down failed";
                }
        }
        
        void GimbalAdaptor::execute_stand_by(RPCError &error)
        {
                r_debug("GimbalAdaptor::stand_by");
                if (!gimbal_.stand_by()) {
                        error.code = 1;
                        error.message = "stand_by failed";
                }
        }
        
        void GimbalAdaptor::execute_wake_up(RPCError &error)
        {
                r_debug("GimbalAdaptor::wake_up");
                if (!gimbal_.wake_up()) {
                        error.code = 1;
                        error.message = "wake_up failed";
                }
        }
}
