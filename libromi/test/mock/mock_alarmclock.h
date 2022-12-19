#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "util/IAlarmClock.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

namespace romi {
        
        class MockAlarmClock : public IAlarmClock {
        public:
                MOCK_METHOD(void, set_listener, (std::shared_ptr<IAlarmClockListener>& listener), (override));
                // MOCK_METHOD(void, add_wakeup_time, (uint8_t hour, uint8_t minute), (override));
                MOCK_METHOD(void, start, (), (override));
                // MOCK_METHOD(bool, check, (), (override));
                // MOCK_METHOD(bool, check_at, (uint8_t hour, uint8_t minute), (override));
        };
}
#pragma GCC diagnostic pop
