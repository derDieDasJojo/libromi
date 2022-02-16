
#include <MessageHub.h>
#include "rpc/ScriptHub.h"
#include <WebSocketServerFactory.h>

ScriptHub::ScriptHub(const std::shared_ptr<rcom::IMessageListener> &listener,
                     const std::shared_ptr<rcom::IWebSocketServerFactory>& webSocketServerFactory,
                     uint16_t port)
    : RawMessageHub("script", listener, std::make_shared<rcom::SocketFactory>(), webSocketServerFactory, port)
{
}
