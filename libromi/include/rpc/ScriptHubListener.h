#ifndef ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H
#define ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H

#include <rcom/IMessageListener.h>
#include <ui/ScriptList.h>
#include "rover/Rover.h"

namespace romi {

        class ScriptHubListener : public rcom::IMessageListener{

        public:
                explicit ScriptHubListener(romi::Rover& rover);

                ~ScriptHubListener() override = default;
                void onmessage(rcom::IWebSocket& link, rcom::MemBuffer& message, rcom::MessageType type) override;

        private:
                rcom::MemBuffer handle_message(rcom::MemBuffer &message);
                rcom::MemBuffer handle_list_request();
                std::string handle_execute_remote_state_change(std::string& state);
                std::string handle_execute_remote_script(const romi::ScriptList &scriptlist, long index);
                rcom::MemBuffer handle_execute_remote_request(nlohmann::json &json_msg);
        private:
                static const std::vector<std::string> commands_;
                romi::Rover& rover_;


        };
}

#endif //ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H
