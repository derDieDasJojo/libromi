#include "gmock/gmock.h"
#include "api/ICamera.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockCamera : public romi::ICamera
{
public:
        MOCK_METHOD(bool, grab, (romi::Image &image), (override));
        MOCK_METHOD(rcom::MemBuffer&, grab_jpeg, (), (override));
        MOCK_METHOD(bool, power_up, (), (override));
        MOCK_METHOD(bool, power_down, (), (override));
        MOCK_METHOD(bool, stand_by, (), (override));
        MOCK_METHOD(bool, wake_up, (), (override));
};
#pragma GCC diagnostic pop
