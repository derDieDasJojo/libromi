#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "util/IAlarmClockListener.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

namespace romi {
        
        class MockAlarmClockListener : public IAlarmClockListener {
        public:
                MOCK_METHOD(void, wakeup, (uint8_t hour, uint8_t minute), (override));
        };
}
#pragma GCC diagnostic pop
