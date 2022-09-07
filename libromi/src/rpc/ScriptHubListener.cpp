
#include <stdexcept>
#include <rpc/RemoteFunctionCallNames.h>
#include <rpc/ScriptHubListener.h>
#include "rover/EventsAndStates.h"
#include <iostream>

namespace romi {

        ScriptHubListener::ScriptHubListener(romi::Rover& rover)
                : rover_(rover){

        }

        void ScriptHubListener::onmessage(rcom::IWebSocketServer&,
                                          rcom::IWebSocket &link,
                                          rcom::MemBuffer &message,
                                          rcom::MessageType type)
        {

                if (type == rcom::kTextMessage) {
                        auto reply = handle_message(message);
                        link.send(reply, rcom::kTextMessage);
                } else {
                        throw std::runtime_error("ScriptHubListener::onmessage: "
                                                 "invalid binary message received.");
                }
        }

        rcom::MemBuffer ScriptHubListener::handle_message(rcom::MemBuffer &message)
        {
                rcom::MemBuffer response;
                std::cout << "Received<" << message.tostring() << ">" << std::endl;
                nlohmann::json json_msg = nlohmann::json::parse(message.tostring());
                std::string request = json_msg.at("request");

                if (request == "list") {
                        response = handle_list_request();
                } else if (request == "execute") {
                        response = handle_execute_remote_request(json_msg);
                } else {
                        response.printf(R"({"success":false, "message":"%s"})",
                                        "unknown request type");
                }
                return response;
        }

        rcom::MemBuffer ScriptHubListener::handle_list_request()
        {
                rcom::MemBuffer response;
                auto json_scripts = rover_.script_engine.scriptList().json_scripts();
                response.append(json_scripts);
                return response;
        }

        rcom::MemBuffer ScriptHubListener::handle_execute_remote_request(nlohmann::json &json_msg)
        {
                rcom::MemBuffer response;
                std::string id = json_msg["id"];
                std::string reply;

                auto scriptlist = rover_.script_engine.scriptList();
                long index = scriptlist.get_id_index(id);
                if ((index > -1) && ((size_t)index < scriptlist.size())){
                        reply = handle_execute_remote_script(scriptlist, index);
                } else {
                        reply = handle_execute_remote_state_change(id);
                }

                response.append(reply);
                return response;
        }

        std::string
        ScriptHubListener::handle_execute_remote_script(const romi::ScriptList &scriptlist,
                                                        long index)
        {
                rover_.remote_state_input_device_.set_next_script_index(index);
                rover_.remote_state_input_device_.set_next_event(romi::event_run_script_remote_request);
                std::string reply = "SCRIPT: " + scriptlist[(size_t)index].id + " executed";
                return reply;
        }

        std::string ScriptHubListener::handle_execute_remote_state_change(std::string& state)
        {
                std::string reply = "STATE: " + state + " executed";
                if (romi::RemoteMessgeTypes::execute_type_start_navigation == state){
                        rover_.remote_state_input_device_.set_next_event(romi::event_enter_navigation_remote_request);
                } else if (romi::RemoteMessgeTypes::execute_type_stop_navigation == state){
                        rover_.remote_state_input_device_.set_next_event(romi::event_stop);
                }
                return reply;
        }
}
