#include "gmock/gmock.h"
#include "IRomiSerialClient.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockRomiSerialClient : public romiserial::IRomiSerialClient
{
public:
        MOCK_METHOD(void, send, (const char *request, nlohmann::json& response), (override));
        MOCK_METHOD(void, set_debug, (bool value), (override));
};
#pragma GCC diagnostic pop
