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
#include "api/GimbalRange.h"

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

                        if (method == MethodsGimbal::kMoveto) {
                                execute_moveto(params, error);
                        
                        } else if (method == MethodsGimbal::kGetPosition) {
                                execute_get_position(result, error);
                        
                        } else if (method == MethodsGimbal::kGetRange) {
                                execute_get_range(result, error);
                        
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
                double x = params[MethodsGimbal::kMoveXParam];
                double y = params[MethodsGimbal::kMoveYParam];
                double z = params[MethodsGimbal::kMoveZParam];
                double v = params[MethodsGimbal::kSpeedParam];
                if (!gimbal_.moveto(x, y, z, v)) {
                        r_err("GimbalAdaptor::execute_moveto failed");
                        error.code = 1;
                        error.message = "Moveto failed";
                }
        }

        void GimbalAdaptor::execute_get_position(nlohmann::json& result, RPCError &error)
        {
                r_debug("GimbalAdaptor::get_position");
                v3 position;
                if (gimbal_.get_position(position)) {
                        result[MethodsGimbal::kPositionX] = position.x();
                        result[MethodsGimbal::kPositionY] = position.y();
                        result[MethodsGimbal::kPositionZ] = position.z();
                } else {
                        r_err("GimbalAdaptor::execute_get_position failed");
                        error.code = 1;
                        error.message = "Get position failed";
                }
        }

        void GimbalAdaptor::execute_get_range(nlohmann::json& result,
                                              RPCError &error)
        {
                r_debug("GimbalAdaptor::get_range");
                GimbalRange range;
                if (gimbal_.get_range(range)) {
                    result = nlohmann::json::array({
                                    nlohmann::json::array(
                                            {range.xmin(), range.xmax()}),
                                            nlohmann::json::array(
                                                    {range.ymin(), range.ymax()}),
                                            nlohmann::json::array(
                                                    {range.zmin(), range.zmax()})
                                            });
                } else {
                        r_err("GimbalAdaptor::execute_get_range failed");
                        error.code = 1;
                        error.message = "Get range failed";
                }
        }

        void GimbalAdaptor::execute_pause(RPCError &error)
        {
                r_debug("GimbalAdaptor::execute_pause");
                if (!gimbal_.pause_activity()) {
                        r_err("GimbalAdaptor::execute_pause failed");
                        error.code = 1;
                        error.message = "stop failed";
                }
        }

        void GimbalAdaptor::execute_continue(RPCError &error)
        {
                r_debug("GimbalAdaptor::execute_continue");
                if (!gimbal_.continue_activity()) {
                        r_err("GimbalAdaptor::execute_continue failed");
                        error.code = 1;
                        error.message = "continue failed";
                }
        }

        void GimbalAdaptor::execute_reset(RPCError &error)
        {
                r_debug("GimbalAdaptor::execute_reset");
                if (!gimbal_.reset_activity()) {
                        r_err("GimbalAdaptor::execute_reset failed");
                        error.code = 1;
                        error.message = "reset failed";
                }
        }

        void GimbalAdaptor::execute_power_up(RPCError &error)
        {
                r_debug("GimbalAdaptor::power_up");
                if (!gimbal_.power_up()) {
                        r_err("GimbalAdaptor::execute_power_up failed");
                        error.code = 1;
                        error.message = "power up failed";
                }
        }
        
        void GimbalAdaptor::execute_power_down(RPCError &error)
        {
                r_debug("GimbalAdaptor::power_down");
                if (!gimbal_.power_down()) {
                        r_err("GimbalAdaptor::execute_power_down failed");
                        error.code = 1;
                        error.message = "power down failed";
                }
        }
        
        void GimbalAdaptor::execute_stand_by(RPCError &error)
        {
                r_debug("GimbalAdaptor::stand_by");
                if (!gimbal_.stand_by()) {
                        r_err("GimbalAdaptor::execute_stand_by failed");
                        error.code = 1;
                        error.message = "stand_by failed";
                }
        }
        
        void GimbalAdaptor::execute_wake_up(RPCError &error)
        {
                r_debug("GimbalAdaptor::wake_up");
                if (!gimbal_.wake_up()) {
                        r_err("GimbalAdaptor::execute_wake_up failed");
                        error.code = 1;
                        error.message = "wake_up failed";
                }
        }
}
