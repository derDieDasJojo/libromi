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

#include <stdexcept>
#include "util/Logger.h"
#include "cablebot/CablebotProgramListAdaptor.h"
#include "cablebot/CablebotProgramIO.h"

namespace romi {

        CablebotProgramListAdaptor::CablebotProgramListAdaptor(
                std::shared_ptr<ICablebotProgramList>& programs,
                std::shared_ptr<ICablebotProgramIO>& program_io)
                : programs_(programs),
                  io_(program_io)                  
        {
        }

        void CablebotProgramListAdaptor::execute(const std::string& method,
                                                 nlohmann::json& params,
                                                 nlohmann::json& result,
                                                 rcom::RPCError &error)
        {
                r_debug("CablebotProgramListAdaptor::execute");
                
                error.code = 0;
                
                try {
                        if (method == kGet) {
                                execute_get(result, error);
                        
                        } else if (method == kUpdate) {
                                execute_update(params, error);
                        
                        } else {
                                error.code = rcom::RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }
                        
                } catch (std::exception &e) {
                        error.code = rcom::RPCError::kInternalError;
                        error.message = e.what();
                }
        }
        
        void CablebotProgramListAdaptor::execute(const std::string& method,
                                                 nlohmann::json &params,
                                                 rcom::MemBuffer& result,
                                                 rcom::RPCError &error)
        {
                (void) method;
                (void) params;
                (void) result;
                error.code = rcom::RPCError::kMethodNotFound;
                error.message = "Unknown method";
        }

        void CablebotProgramListAdaptor::execute_get(nlohmann::json& result,
                                                     rcom::RPCError &error)
        {
                (void) error;
                result = CablebotProgramIO::to_json(*programs_);
        }
                                
        void CablebotProgramListAdaptor::execute_update(nlohmann::json& params,
                                                        rcom::RPCError &error)
        {
                nlohmann::json json_program = params[kProgram];
                r_debug("CablebotProgramListAdaptor::execute_update: %s",
                        json_program.dump().c_str());
                auto new_values = CablebotProgramIO::load_program(json_program);
                auto original = programs_->find(new_values->id());
                if (original) {
                        original->update(*new_values);
                        io_->store(*programs_);
                        
                } else {
                        error.code = 1;;
                        error.message = "Couldn't find the program";;
                }
        }
}
