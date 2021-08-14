#include "gmock/gmock.h"
#include "api/IMotorDriver.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockMotorDriver : public romi::IMotorDriver
{
public:

        MOCK_METHOD(bool, disable_controller, (), (override));
        MOCK_METHOD(bool, enable_controller, (), (override));
        MOCK_METHOD(bool, moveat, (double left, double right), (override));
        MOCK_METHOD(bool, stop, (), (override));
        MOCK_METHOD(bool, get_encoder_values, (double& left, double& right,
                                               double& timestamp), (override));
        MOCK_METHOD(int32_t, get_encoder_steps, (), (override));
        MOCK_METHOD(bool, get_speeds, (double& left_target, double& right_target,
                                       double& left_current, double& right_current,
                                       double& left_measured, double& right_measured),
                    (override));
        /* MOCK_METHOD(bool, get_pid_values, (Axis axis,  */
        /*                                    double& target, double& input, double& output, */
        /*                                    double& error_p, double& error_i, */
        /*                                    double& error_d, double& ctrl_in), (override)); */
};
#pragma GCC diagnostic pop
