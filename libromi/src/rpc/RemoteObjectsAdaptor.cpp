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
#include <iostream>
#include "rpc/RemoteObjectsAdaptor.h"
#include <util/Logger.h>

namespace romi {

        RemoteObjectsAdaptor::RemoteObjectsAdaptor()
                : map_()
        {
        }
        
        void RemoteObjectsAdaptor::execute(const std::string& id,
                                           const std::string& method,
                                           nlohmann::json& params,
                                           nlohmann::json& result,
                                           rcom::RPCError& error)
        {
                rcom::IRPCHandler *adaptor = get_adaptor(id);

                if (adaptor != nullptr) {
                        adaptor->execute(id, method, params, result, error);
                                
                } else {
                        error.code = 2;
                        error.message = "Unknown object id";
                }
        }
        
        void RemoteObjectsAdaptor::execute(const std::string& id,
                                           const std::string& method,
                                           nlohmann::json& params,
                                           rcom::MemBuffer& result,
                                           rcom::RPCError &error)
        {
                rcom::IRPCHandler *adaptor = get_adaptor(id);

                if (adaptor != nullptr) {
                        adaptor->execute(id, method, params, result, error);
                                
                } else {
                        error.code = 2;
                        error.message = "Unknown object id";
                }
        }

        rcom::IRPCHandler *RemoteObjectsAdaptor::get_adaptor(const std::string& id)
        {
                rcom::IRPCHandler *result = nullptr;
                ObjectMap::iterator it;
                
                it = map_.find(id);
                if (it != map_.end()) {
                        result = it->second.get();
                }
                
                return result;
        }

        void RemoteObjectsAdaptor::add(const std::string& id,
                                       std::shared_ptr<rcom::IRPCHandler>& ptr)
        {
                map_.insert(std::pair<std::string, std::shared_ptr<rcom::IRPCHandler>>(id, ptr));
        }
}

