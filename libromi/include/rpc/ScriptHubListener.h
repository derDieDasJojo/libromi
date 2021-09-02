#ifndef ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H
#define ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H

#include <IMessageListener.h>
#include <ui/ScriptList.h>
#include "rover/Rover.h"

class ScriptHubListener : public rcom::IMessageListener{

    public:
        explicit ScriptHubListener(romi::Rover& rover);

        ~ScriptHubListener() override = default;
        void onmessage(rcom::IWebSocket& link, rpp::MemBuffer& message, rcom::MessageType type) override;

    private:
        rpp::MemBuffer handle_message(rpp::MemBuffer &message);
        rpp::MemBuffer handle_list_request();
        std::string handle_execute_state_change(std::string& state);
        rpp::MemBuffer handle_execute_script_request(JsonCpp &json_msg);
    private:
        static const std::vector<std::string> commands_;
        romi::Rover& rover_;


};


#endif //ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H
