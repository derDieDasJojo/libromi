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

#include "cv/ImageIO.h"
#include "rpc/RemoteCamera.h"
#include "rpc/MethodsCamera.h"
#include "rpc/MethodsPowerDevice.h"

namespace romi {
        
        RemoteCamera::RemoteCamera(std::unique_ptr<IRPCClient>& client)
                : RemoteStub(client), output_()
        {
        }
        
        bool RemoteCamera::grab(Image &image) 
        {
                bool success = false;
                rpp::MemBuffer& jpeg = grab_jpeg();
                if (jpeg.size() > 0) {
                        success = ImageIO::load_from_buffer(image, jpeg.data());
                }
                return success;
        }
        
        rpp::MemBuffer& RemoteCamera::grab_jpeg()
        {
                JsonCpp params;
                RPCError error;

                output_.clear();
                
                client_->execute(MethodsCamera::grab_jpeg_binary,
                                 params, output_, error);
                
                if (error.code != 0) {
                        r_warn("RemoteCamera::grab_jpeg: %s", error.message.c_str());
                }

                return output_;
        }
        
        bool RemoteCamera::power_up()
        {
                r_debug("RemoteCamera::power_up");
                return execute_simple_request(MethodsPowerDevice::power_up);
        }
        
        bool RemoteCamera::power_down()
        {
                r_debug("RemoteCamera::power_down");
                return execute_simple_request(MethodsPowerDevice::power_down);
        }
        
        bool RemoteCamera::stand_by()
        {
                r_debug("RemoteCamera::stand_by");
                return execute_simple_request(MethodsPowerDevice::stand_by);
        }
        
        bool RemoteCamera::wake_up()
        {
                r_debug("RemoteCamera::wake_up");
                return execute_simple_request(MethodsPowerDevice::wake_up);
        }
}
