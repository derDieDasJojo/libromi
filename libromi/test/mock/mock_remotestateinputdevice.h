#include "gmock/gmock.h"
#include "api/IRemoteStateInputDevice.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockRemoteStateInputDevice : public romi::IRemoteStateInputDevice
{
public:
        MOCK_METHOD(int, get_next_event, (), (override));
        MOCK_METHOD(void, set_next_event, (int), (override));
};
#pragma GCC diagnostic pop
