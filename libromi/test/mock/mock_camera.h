#include "gmock/gmock.h"
#include "api/ICamera.h"
#include "camera/ICameraSettings.h"

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
        MOCK_METHOD(bool, set_value, (const std::string&, double), (override));
        MOCK_METHOD(bool, select_option, (const std::string&, const std::string&), (override));
        MOCK_METHOD(const romi::ICameraSettings&, get_settings, (), (override));
};
#pragma GCC diagnostic pop
