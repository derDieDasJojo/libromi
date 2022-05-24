#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <RSerial.h>
#include <RomiSerialClient.h>
#include <RomiSerialErrors.h>

using namespace romiserial;

class romiserial_arduino_tests : public ::testing::Test
{
protected:
        std::string controller_name;
        std::shared_ptr<RSerial> serial;
        RomiSerialClient romiserial;

	romiserial_arduino_tests()
                : controller_name("romiserial_arduino_tests"), serial(std::make_shared<RSerial>("/dev/ttyACM0", 115200, 1)),
                  romiserial(serial, serial, 255, controller_name) {
                //romiserial.set_debug(true);
	}
        
	~romiserial_arduino_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
};

TEST_F(romiserial_arduino_tests, test_success_of_simple_command)
{
        nlohmann::json response;
        romiserial.send("a", response);
        int code = response[0];
        
        ASSERT_EQ(0, code);        
}

TEST_F(romiserial_arduino_tests, test_failure_bad_number_of_arguments)
{
        nlohmann::json response;
        romiserial.send("b", response);
        int code = response[0];
        
        ASSERT_EQ(kBadNumberOfArguments, code);
}

TEST_F(romiserial_arduino_tests, test_success_two_arguments)
{
        nlohmann::json response;
        romiserial.send("b[1,2]", response);
        int code = response[0];
        
        ASSERT_EQ(0, code);
}

TEST_F(romiserial_arduino_tests, test_success_string_argument)
{
        nlohmann::json response;
        romiserial.send("c[1,\"Dinner's ready\"]", response);
        int code = response[0];
        
        ASSERT_EQ(0, code);
}

TEST_F(romiserial_arduino_tests, test_success_two_arguments_returning_value)
{
        nlohmann::json response;
        romiserial.send("d[1,1]", response);
        int code = response[0];
        int result = response[1];
        
        ASSERT_EQ(0, code);
        ASSERT_EQ(2, result);
}

TEST_F(romiserial_arduino_tests, test_success_string_argument_returning_string)
{
        nlohmann::json response;
        romiserial.send("e[\"he's resting\"]", response);
        int code = response[0];
        std::string result = response[1];
        
        ASSERT_EQ(0, code);
        ASSERT_EQ("he's resting", result);
}

