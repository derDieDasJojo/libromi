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
#ifndef __ROMI_CAMERA_HANDLER_H
#define __ROMI_CAMERA_HANDLER_H

#include "rpc/IRPCHandler.h"
#include "api/ICamera.h"

namespace romi {

        class CameraAdaptor : public IRPCHandler
        {
        protected:
                ICamera& camera_;

                void grab_jpeg_json(nlohmann::json& result, RPCError& error);
                void encode(rcom::MemBuffer& jpeg, nlohmann::json& result);
                std::string encode_base64(const uint8_t *s, size_t ilen);
                
        public:
                CameraAdaptor(ICamera& camera);
                ~CameraAdaptor() override = default;
        
                void execute(const std::string& method,
                             nlohmann::json& params,
                             nlohmann::json& result,
                             RPCError& status) override;
                void execute(const std::string& method,
                             nlohmann::json &params,
                             rcom::MemBuffer& result,
                             RPCError &status) override;
        };
}

#endif // __ROMI_CAMERA_HANDLER_H
