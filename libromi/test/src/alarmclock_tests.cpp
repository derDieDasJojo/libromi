#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "util/AlarmClock.h"
#include "util/Clock.h"
#include "util/ClockAccessor.h"
#include "mock_alarmclocklistener.h"
#include "mock_clock.h"

using namespace std;
using namespace testing;
using namespace romi;

class alarmclock_tests : public ::testing::Test
{
protected:
        std::shared_ptr<MockAlarmClockListener> listener_;
        std::shared_ptr<MockClock> clock_;
        
	alarmclock_tests()
                : listener_(),
                  clock_() {
                listener_ = std::make_shared<MockAlarmClockListener>();
                clock_ = std::make_shared<romi::MockClock>();
                romi::ClockAccessor::SetInstance(clock_);
        }

	~alarmclock_tests() override {
                romi::ClockAccessor::SetInstance(nullptr);
        }

	void SetUp() override {
        }

	void TearDown() override {}
};

TEST_F(alarmclock_tests, test_constructor)
{
        // Arrange
        
        // Act
        std::shared_ptr<IAlarmClockListener> listener = listener_;
        AlarmClock alarm_clock(listener);

        // Assert
}

// TEST_F(alarmclock_tests, test_check_at_returns_false)
// {
//         // Arrange
//         std::shared_ptr<IAlarmClockListener> listener = listener_;
//         AlarmClock alarm_clock(listener);
//         alarm_clock.add_wakeup_time(4, 5);
        
//         // Act
//         bool alarm = alarm_clock.check_at(4, 4);
        
//         // Assert
//         ASSERT_EQ(alarm, false);
// }

// TEST_F(alarmclock_tests, test_check_at_calls_listener_and_returns_true)
// {
//         // Arrange
//         EXPECT_CALL(*listener_, wakeup(4, 5));;
//         std::shared_ptr<IAlarmClockListener> listener = listener_;
//         AlarmClock alarm_clock(listener);
//         alarm_clock.add_wakeup_time(4, 5);
        
//         // Act
//         bool alarm = alarm_clock.check_at(4, 5);
        
//         // Assert
//         ASSERT_EQ(alarm, true);
// }

// TEST_F(alarmclock_tests, test_check_returns_false)
// {
//         // Arrange
//         EXPECT_CALL(*clock_, date())
//                 .WillOnce(Return(Date(1, 2, 3, 4, 5, 6)));
//         std::shared_ptr<IAlarmClockListener> listener = listener_;
//         AlarmClock alarm_clock(listener);
//         alarm_clock.add_wakeup_time(4, 4);
        
//         // Act
//         bool alarm = alarm_clock.check();
        
//         // Assert
//         ASSERT_EQ(alarm, false);
// }

// TEST_F(alarmclock_tests, test_check_calls_listener_and_returns_true)
// {
//         // Arrange
//         EXPECT_CALL(*clock_, date())
//                 .WillOnce(Return(Date(1, 2, 3, 4, 5, 6)));
//         EXPECT_CALL(*listener_, wakeup(4, 5));;
//         std::shared_ptr<IAlarmClockListener> listener = listener_;
//         AlarmClock alarm_clock(listener);
//         alarm_clock.add_wakeup_time(4, 5);
        
//         // Act
//         bool alarm = alarm_clock.check();
        
//         // Assert
//         ASSERT_EQ(alarm, true);
// }

