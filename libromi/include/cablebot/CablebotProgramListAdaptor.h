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

#ifndef __ROMI_CABLEBOTPROGRAMLISTADAPTOR_H
#define __ROMI_CABLEBOTPROGRAMLISTADAPTOR_H

#include <string>
#include <rcom/IRPCHandler.h>
#include "cablebot/ICablebotProgramList.h"
#include "cablebot/ICablebotProgramIO.h"

namespace romi {

        class CablebotProgramListAdaptor : public rcom::IRPCHandler
        {
        protected:

                static constexpr const char *kGet = "get";
                static constexpr const char *kUpdate = "update";
                static constexpr const char *kProgram = "program";
                
                std::shared_ptr<ICablebotProgramList> programs_;
                std::shared_ptr<ICablebotProgramIO>& io_;
                
                void execute_get(nlohmann::json& params, rcom::RPCError &error);
                void execute_update(nlohmann::json& params, rcom::RPCError &error);

        public:
                explicit
                CablebotProgramListAdaptor(std::shared_ptr<ICablebotProgramList>& programs,
                                           std::shared_ptr<ICablebotProgramIO>& program_io);
                ~CablebotProgramListAdaptor() override = default;

                void execute(const std::string& id,
                             const std::string& method,
                             nlohmann::json& params,
                             nlohmann::json& result,
                             rcom::RPCError &error) override;
                void execute(const std::string& id,
                             const std::string& method,
                             nlohmann::json &params,
                             rcom::MemBuffer& result,
                             rcom::RPCError &status) override;
        };
}

#endif // __ROMI_CABLEBOTPROGRAMLISTADAPTOR_H
