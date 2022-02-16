#include "gmock/gmock.h"
#include "rpc/IRPCClient.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockRPCClient : public romi::IRPCClient
{
public:
        MOCK_METHOD(void, execute, (const std::string& method, nlohmann::json& params, nlohmann::json& result, romi::RPCError& error), (override));
        MOCK_METHOD(void, execute, (const std::string& method, nlohmann::json& params, rcom::MemBuffer& result, romi::RPCError& error), (override));
        MOCK_METHOD(bool, is_connected, (), (override));
};
#pragma GCC diagnostic pop
