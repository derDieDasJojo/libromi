#ifndef ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUB_H
#define ROMI_ROVER_BUILD_AND_TEST_SCRIPTHUB_H

#include <RawMessageHub.h>
#include "ui/ScriptList.h"
#include "rpc/IScriptHub.h"

const uint16_t ScriptHubListeningPort = 20000;

class ScriptHub : public rcom::RawMessageHub {

    public:
        ScriptHub(const std::shared_ptr<rcom::IMessageListener> &listener, uint16_t port);
        ~ScriptHub()override  = default;
        private:


};
#endif
