#include "gmock/gmock.h"
#include "rpc/IRPCHandler.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockRPCHandler : public romi::IRPCHandler
{
public:
        MOCK_METHOD(void, execute, (const std::string& method, nlohmann::json& params, nlohmann::json& result, romi::RPCError& error), (override));
        MOCK_METHOD(void, execute, (const std::string& method, nlohmann::json& params, rcom::MemBuffer& result, romi::RPCError& error), (override));
};
#pragma GCC diagnostic pop
