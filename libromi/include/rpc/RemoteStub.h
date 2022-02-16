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
#ifndef __ROMI_REMOTE_STUB_H
#define __ROMI_REMOTE_STUB_H

#include <memory>
#include <utility>
#include "rpc/IRPCClient.h"

namespace romi {
        
        class RemoteStub
        {
        protected:
                std::unique_ptr<IRPCClient> client_;

                bool execute(const std::string& method, nlohmann::json& params, nlohmann::json& result);
                bool execute_with_params(const std::string& method, nlohmann::json& params);
                bool execute_with_result(const std::string& method, nlohmann::json& result);
                bool execute_simple_request(const std::string& method);

        public:
                explicit RemoteStub(std::unique_ptr<IRPCClient>& client)
                        : client_() {
                        client_ = std::move(client);
                }
                virtual ~RemoteStub() = default;
        };
}

#endif // __ROMI_REMOTE_STUB_H
