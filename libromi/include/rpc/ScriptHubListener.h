#ifndef ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H
#define ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H

#include <IMessageListener.h>
#include <ui/ScriptList.h>



class ScriptHubListener : public rcom::IMessageListener{

    public:
        explicit ScriptHubListener(const romi::IScriptList& script);

        ~ScriptHubListener() override = default;
        void onmessage(rcom::IWebSocket& link, rpp::MemBuffer& message, rcom::MessageType type) override;

    private:
        rpp::MemBuffer handle_message(rpp::MemBuffer &message);
        rpp::MemBuffer handle_list_request();
    private:
        static const std::vector<std::string> commands_;
        const romi::IScriptList& scriptList_;
};


#endif //ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUBLISTENER_H
