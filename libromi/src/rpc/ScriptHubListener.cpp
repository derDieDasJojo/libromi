
#include <rpc/ScriptHubListener.h>
#include <iostream>


const std::vector<std::string> ScriptHubListener::commands_ ({"list"});
// ADD to constructor link to script engine or state machine?
// call state machine with message once parameters are verified.

ScriptHubListener::ScriptHubListener(const romi::IScriptList &script, romi::Rover& rover)
    : scriptList_(script) , rover_(rover){

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
    std::cout << "Received<" << message.tostring() << ">" << std::endl;
    JsonCpp json_msg = JsonCpp::parse(message);
    std::string request = json_msg.str("request");
    if (request == "list") {
        response = handle_list_request();
    }
    else if (request == "execute") {
        response = handle_execute_request(json_msg);
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

rpp::MemBuffer ScriptHubListener::handle_execute_request(JsonCpp &json_msg) {
    rpp::MemBuffer response;
    std::string id = json_msg.str("id");
    auto index = scriptList_.get_id_index(id);
    if ((index > -1) && ((size_t)index < scriptList_.size()))
        rover_.script_engine.execute_script(rover_, (size_t)index);
    std::string reply("Script = " + scriptList_[(size_t)index].id);
    response.append_string(reply.c_str());
    return response;
}
