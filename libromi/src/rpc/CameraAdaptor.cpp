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
#include "rpc/CameraAdaptor.h"
#include "rpc/MethodsCamera.h"
#include "rpc/MethodsPowerDevice.h"

namespace romi {

        CameraAdaptor::CameraAdaptor(ICamera& camera)
                : camera_(camera)
        {
        }

        void CameraAdaptor::execute(const std::string& method,
                                    JsonCpp &params,
                                    rpp::MemBuffer& result,
                                    RPCError &error)
        {
                (void) params;

                error.code = 0;
                result.clear();
                
                try {
                        if (method == MethodsCamera::grab_jpeg_binary) {
                                
                                rpp::MemBuffer& jpeg = camera_.grab_jpeg();
                                result.append(jpeg); // TODO: can we avoid a copy?
                                
                        } else {
                                r_warn("Unknown method: %s", method.c_str());
                                r_debug("Known methods: %s", MethodsCamera::grab_jpeg_binary);
                                error.code = RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }
                        
                } catch (std::exception& e) {
                        error.code = RPCError::kInternalError;
                        error.message = e.what();
                }
        }
        
        void CameraAdaptor::execute(const std::string& method, JsonCpp& params,
                                    JsonCpp& result, RPCError& error)
        {
                (void) params;
                (void) result;

                error.code = 0;
                                
                try {
                        if (method == MethodsPowerDevice::power_up) {
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
                        
                } catch (std::exception& e) {
                        error.code = RPCError::kInternalError;
                        error.message = e.what();
                }
        }

        void CameraAdaptor::execute_power_up(RPCError &error)
        {
                r_debug("CameraAdaptor::power_up");
                if (!camera_.power_up()) {
                        error.code = 1;
                        error.message = "power up failed";
                }
        }
        
        void CameraAdaptor::execute_power_down(RPCError &error)
        {
                r_debug("CameraAdaptor::power_down");
                if (!camera_.power_down()) {
                        error.code = 1;
                        error.message = "power down failed";
                }
        }
        
        void CameraAdaptor::execute_stand_by(RPCError &error)
        {
                r_debug("CameraAdaptor::stand_by");
                if (!camera_.stand_by()) {
                        error.code = 1;
                        error.message = "stand_by failed";
                }
        }
        
        void CameraAdaptor::execute_wake_up(RPCError &error)
        {
                r_debug("CameraAdaptor::wake_up");
                if (!camera_.wake_up()) {
                        error.code = 1;
                        error.message = "wake_up failed";
                }
        }
}
