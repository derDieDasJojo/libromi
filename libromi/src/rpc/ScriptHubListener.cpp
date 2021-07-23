
#include <rpc/ScriptHubListener.h>


const std::vector<std::string> ScriptHubListener::commands_ ({"list"});
// ADD to constructor link to script engine or state machine?
// call state machine with message once parameters are verified.

ScriptHubListener::ScriptHubListener(const romi::IScriptList &script) : scriptList_(script){

}

void ScriptHubListener::onmessage(rcom::IWebSocket &link, rpp::MemBuffer &message, rcom::MessageType type) {

    if (type == rcom::kTextMessage) {

        auto reply = handle_message(message);

        // Parse Message,
        // check valid
        // Raise script event
        // Change state.
        link.send(reply, rcom::kTextMessage);

    }
    else throw std::runtime_error("ScriptHubListener::onmessage - invalid binary message received.");
}

rpp::MemBuffer ScriptHubListener::handle_message(rpp::MemBuffer &message) {
    rpp::MemBuffer response;
    JsonCpp json_msg = JsonCpp::parse(message);
    std::string request = json_msg.str("request");
    if (request == "list") {
        response = handle_list_request();
    }
    else
    {
        response.printf(R"({"success":false, "message":"%s"})", "unknown request type");
    }
    return response;
}

rpp::MemBuffer ScriptHubListener::handle_list_request() {
    rpp::MemBuffer response;
    auto json_scripts = scriptList_.json_scripts();
    response.append_string(json_scripts.c_str());
    return response;
}
