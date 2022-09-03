#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "util/ILogWriter.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

namespace romi {
        class MockLogWriterFactory : public ILogWriterFactory {
        public:
            MOCK_METHOD(std::shared_ptr<ILogWriter>, create_console_writer, (), (override));
            MOCK_METHOD(std::shared_ptr<ILogWriter>, create_file_writer, (), (override));
        };
}
#pragma GCC diagnostic pop
