
#include <MessageHub.h>
#include "rpc/ScriptHub.h"
#include "rpc/ScriptHubListener.h"

ScriptHub::ScriptHub(const std::shared_ptr<rcom::IMessageListener> &listener, uint16_t port)
    : RawMessageHub("script", listener, port)
{
}
