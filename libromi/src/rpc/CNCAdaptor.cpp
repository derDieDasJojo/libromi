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
#include <r.h>
#include "rpc/CNCAdaptor.h"
#include "rpc/MethodsRover.h"

namespace romi {

        CNCAdaptor::CNCAdaptor(ICNC &cnc)
                : cnc_(cnc)
        {
        }

        void CNCAdaptor::execute(const std::string& method,
                                 JsonCpp &params,
                                 rpp::MemBuffer& result,
                                 RPCError &error)
        {
                (void) method;
                (void) params;
                (void) result;
                error.code = RPCError::kMethodNotFound;
                error.message = "Unknown method";
        }

        void CNCAdaptor::execute(const std::string& method, JsonCpp& params,
                                 JsonCpp& result, RPCError &error)
        {
                r_debug("CNCAdaptor::execute");

                error.code = 0;
                
                try {

                        if (method.empty()) {
                                error.code = RPCError::kMethodNotFound;
                                error.message = "No method specified";
                                
                        } else if (method == MethodsCNC::homing) {
                                execute_homing(error);
                                 
                        } else if (method == MethodsCNC::moveto) {
                                execute_moveto(params, result, error);
                                
                        } else if (method == MethodsCNC::spindle) {
                                execute_spindle(params, result, error);
                                
                        } else if (method == MethodsCNC::travel) {
                                execute_travel(params, result, error);
                                
                        } else if (method == MethodsCNC::helix) {
                                execute_helix(params, result, error);
                                
                        } else if (method == MethodsCNC::get_range) {
                                execute_get_range(params, result, error);
                                
                        } else if (method == MethodsCNC::get_position) {
                                execute_get_position(params, result, error);
                                
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
                                r_err("CNCAdaptor::execute: method not found: %s",
                                      method.c_str());
                                error.code = RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }

                } catch (std::exception &e) {
                        error.code = RPCError::kInternalError;
                        error.message = e.what();
                }
        }

        void CNCAdaptor::execute_get_range(JsonCpp& params, JsonCpp& result,
                                           RPCError &error)
        {
                (void) params;
                r_debug("CNCAdaptor::execute_get_range");
                CNCRange range;
                if (cnc_.get_range(range)) {
                        result = JsonCpp::construct("[[%f,%f],[%f,%f],[%f,%f]]",
                                                    range.xmin(), range.xmax(),
                                                    range.ymin(), range.ymax(),
                                                    range.zmin(), range.zmax());
                } else {
                        r_err("CNCAdaptor::execute_get_range failed");
                        error.code = 1;
                        error.message = "get_range failed";
                }
        }
        
        void CNCAdaptor::execute_get_position(JsonCpp& params, JsonCpp& result,
                                           RPCError &error)
        {
                (void) params;
                r_debug("CNCAdaptor::execute_get_position");
                v3 position;
                if (cnc_.get_position(position)) {
                        result = JsonCpp::construct("{\"x\":%f,\"y\":%f,\"z\":%f}",
                                                    position.x(), position.y(),
                                                    position.z());
                } else {
                        r_err("CNCAdaptor::execute_get_position failed");
                        error.code = 1;
                        error.message = "get_position failed";
                }
        }
        
        void CNCAdaptor::execute_moveto(JsonCpp& params, JsonCpp& result,
                                        RPCError &error)
        {
                (void) result;
                r_debug("CNCAdaptor::execute_moveto");

                {
                        char buffer[256];
                        json_tostring(params.ptr(), k_json_compact, buffer, 256);
                        r_debug("CNCAdaptor::execute_moveto: %s", buffer);
                }

                if (!params.has(MethodsCNC::kMoveXParam)
                    && !params.has(MethodsCNC::kMoveYParam)
                    && !params.has(MethodsCNC::kMoveZParam)) {
                        r_err("CNCAdaptor::execute_moveto failed: missing parameters");
                        error.code = RPCError::kInvalidParams;
                        error.message = "missing x, y, or z parameters";
                        
                } else {
                        
                        double x = params.num(MethodsCNC::kMoveXParam, ICNC::UNCHANGED);
                        double y = params.num(MethodsCNC::kMoveYParam, ICNC::UNCHANGED);
                        double z = params.num(MethodsCNC::kMoveZParam, ICNC::UNCHANGED);
                        double v = params.num(MethodsCNC::kSpeedParam, 0.2);
                        
                        r_debug("CNCAdaptor::execute_moveto: %f, %f, %f", x, y, z);
                                
                        if (!cnc_.moveto(x, y, z, v)) {
                                error.code = 1;
                                error.message = "moveto failed";
                        }
                }
        }
        
        void CNCAdaptor::execute_spindle(JsonCpp& params, JsonCpp& result, RPCError &error)
        {
                (void) result;
                r_debug("CNCAdaptor::execute_spindle");
                
                try {
                        double speed = params.num(MethodsCNC::kSpeedParam);
                        if (!cnc_.spindle(speed)) {
                                error.code = 1;
                                error.message = "spindle failed";
                        }

                } catch (JSONError &je) {
                        r_err("CNCAdaptor::execute_spindle failed: %s", je.what());
                        error.code = RPCError::kInvalidParams;
                        error.message = je.what();
                }
        }
        
        void CNCAdaptor::execute_travel(JsonCpp& params, JsonCpp& result, RPCError &error)
        {
                (void) result;
                r_debug("CNCAdaptor::execute_travel");
                
                try {
                        Path path;
                        double speed = params.num(MethodsCNC::kSpeedParam, 0.1);
                        JsonCpp p = params.array(MethodsCNC::kTravelPathParam);
                        for (size_t i = 0; i < p.length(); i++) {
                                v3 pt;
                                JsonCpp a = p.array(i);
                                pt.set(a.num(0), a.num(1), a.num(2));
                                path.push_back(pt);
                        }

                        if (!cnc_.travel(path, speed)) {
                                error.code = 1;
                                error.message = "travel failed";
                        }

                } catch (JSONError &je) {
                        r_err("CNCAdaptor::execute_travel failed: %s", je.what());
                        error.code = RPCError::kInvalidParams;
                        error.message = je.what();
                }
        }
        
        void CNCAdaptor::execute_helix(JsonCpp& params, JsonCpp& result, RPCError &error)
        {
                (void) result;
                r_debug("CNCAdaptor::execute_helix");
                
                try {
                        double xc = params.num(MethodsCNC::kHelixXcParam);
                        double yc = params.num(MethodsCNC::kHelixYcParam);
                        double alpha = params.num(MethodsCNC::kHelixAlphaParam);
                        double z = params.num(MethodsCNC::kHelixZParam);
                        double speed = params.num(MethodsCNC::kSpeedParam, 0.1);
                        
                        if (!cnc_.helix(xc, yc, alpha, z, speed)) {
                                error.code = 1;
                                error.message = "helix failed";
                        }

                } catch (JSONError &je) {
                        r_err("CNCAdaptor::execute_helix failed: %s", je.what());
                        error.code = RPCError::kInvalidParams;
                        error.message = je.what();
                }
        }
        
        void CNCAdaptor::execute_homing(RPCError &error)
        {
                r_debug("CNCAdaptor::execute_homing");
                
                if (!cnc_.homing()) {
                        error.code = 1;
                        error.message = "homing failed";
                }
        }

        void CNCAdaptor::execute_pause(RPCError &error)
        {
                r_debug("CNCAdaptor::execute_pause");
                if (!cnc_.pause_activity()) {
                        error.code = 1;
                        error.message = "stop failed";
                }
        }

        void CNCAdaptor::execute_continue(RPCError &error)
        {
                r_debug("CNCAdaptor::execute_continue");
                if (!cnc_.continue_activity()) {
                        error.code = 1;
                        error.message = "continue failed";
                }
        }

        void CNCAdaptor::execute_reset(RPCError &error)
        {
                r_debug("CNCAdaptor::execute_reset");
                if (!cnc_.reset_activity()) {
                        error.code = 1;
                        error.message = "reset failed";
                }
        }

        void CNCAdaptor::execute_power_up(RPCError &error)
        {
                r_debug("CNCAdaptor::power_up");
                if (!cnc_.power_up()) {
                        error.code = 1;
                        error.message = "power up failed";
                }
        }
        
        void CNCAdaptor::execute_power_down(RPCError &error)
        {
                r_debug("CNCAdaptor::power_down");
                if (!cnc_.power_down()) {
                        error.code = 1;
                        error.message = "power down failed";
                }
        }
        
        void CNCAdaptor::execute_stand_by(RPCError &error)
        {
                r_debug("CNCAdaptor::stand_by");
                if (!cnc_.stand_by()) {
                        error.code = 1;
                        error.message = "stand_by failed";
                }
        }
        
        void CNCAdaptor::execute_wake_up(RPCError &error)
        {
                r_debug("CNCAdaptor::wake_up");
                if (!cnc_.wake_up()) {
                        error.code = 1;
                        error.message = "wake_up failed";
                }
        }
}

