/*
  romi-rover

  Copyright (C) 2019-2020 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is the main app for the Romi Rover.

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
#ifndef __ROMI_SCRIPTHUBLISTENER_H
#define __ROMI_SCRIPTHUBLISTENER_H

#include <rcom/IMessageListener.h>
#include <ui/ScriptList.h>
#include "rover/Rover.h"

namespace romi {
        
        const uint16_t ScriptHubListeningPort = 20000;
        
        class ScriptHubListener : public rcom::IMessageListener
        {

        public:
                explicit ScriptHubListener(romi::Rover& rover);

                ~ScriptHubListener() override = default;
                void onmessage(rcom::IWebSocketServer& server,
                               rcom::IWebSocket& link,
                               rcom::MemBuffer& message,
                               rcom::MessageType type) override;

        private:
                rcom::MemBuffer handle_message(rcom::MemBuffer &message);
                rcom::MemBuffer handle_list_request();
                std::string handle_execute_remote_state_change(std::string& state);
                std::string handle_execute_remote_script(const romi::ScriptList &scriptlist,
                                                         long index);
                rcom::MemBuffer handle_execute_remote_request(nlohmann::json &json_msg);
        private:
                static const std::vector<std::string> commands_;
                romi::Rover& rover_;


        };
}

#endif // __ROMI_SCRIPTHUBLISTENER_H
