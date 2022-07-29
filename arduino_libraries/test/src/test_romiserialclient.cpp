#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include <vector>
//
#include <RomiSerialClient.h>
#include <RomiSerialErrors.h>
#include "../mock/mock_inputstream.h"
#include "../mock/mock_outputstream.h"

using namespace std;
using namespace testing;
using namespace romiserial;

class romiserialclient_tests : public ::testing::Test
{
protected:
        std::shared_ptr<MockInputStream> in;
        std::shared_ptr<MockOutputStream> out;

        string output_message;
        string expected_message;
        
	romiserialclient_tests()
                : in(std::make_shared<MockInputStream>()),
                  out(std::make_shared<MockOutputStream>()),
                  output_message(),
                  expected_message() {
        }

	~romiserialclient_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}

        void initSerialAvailable(const char *s) {
                InSequence seq;
                size_t len = strlen(s);
                for (size_t i = len; i >= 1; i--) {
                        EXPECT_CALL(*in, available)
                                .WillOnce(Return(i))
                                .RetiresOnSaturation();
                }
        }

        void initSerialRead(const char *s) {
                InSequence seq;
                size_t len = strlen(s);
                for (size_t i = 0; i < len; i++) {
                        EXPECT_CALL(*in, read(_))
                                .WillOnce(DoAll(SetArgReferee<0>(s[i]), Return(1)))
                                .RetiresOnSaturation();
                }
        }
        
        void initInput(const char *s) {
                initSerialAvailable(s);
                initSerialRead(s);
        }

        bool append_output(char s) { 
                output_message += s; 
                return true; 
        }

        void setExpectedOutput(const char *s) {
                expected_message = s;
                EXPECT_CALL(*out, write(_))
                        .WillRepeatedly(Invoke(this, &romiserialclient_tests::append_output));
        }

        
};

TEST_F(romiserialclient_tests, message_without_args)
{
        // Arrange
        std::string client_name("romiserialclient_tests");
        EXPECT_CALL(*in, set_timeout(_));
        RomiSerialClient client(in, out, 255, client_name);
        setExpectedOutput("#a:008e\r\n");
        initInput("#a[0]:00e7\r\n");

        // Act
        nlohmann::json response;
        client.send("a", response);
        
        //Assert
        EXPECT_EQ(expected_message, output_message);
        EXPECT_EQ(true, response.is_array());
        EXPECT_EQ(1, response.size());
        EXPECT_EQ(0, response[0]);
}

TEST_F(romiserialclient_tests, message_with_args)
{
        // Arrange

        EXPECT_CALL(*in, set_timeout(_));
        std::string client_name("romiserialclient_tests");
        RomiSerialClient client(in, out, 255, client_name);
        setExpectedOutput("#a[1,2,3]:00dd\r\n");
        initInput("#a[0]:00e7\r\n");

        // Act
        nlohmann::json response;
        client.send("a[1,2,3]", response);
        
        //Assert
        EXPECT_EQ(expected_message, output_message);
        EXPECT_EQ(true, response.is_array());
        EXPECT_EQ(1, response.size());
        EXPECT_EQ(0, response[0]);
}

TEST_F(romiserialclient_tests, error_reponse)
{
        // Arrange
        EXPECT_CALL(*in, set_timeout(_));
        std::string client_name("romiserialclient_tests");
        RomiSerialClient client(in, out, 255, client_name);
        setExpectedOutput("#a:008e\r\n");
        initInput("#a[1,\"Went to bed early\"]:00f2\r\n");

        // Act
        nlohmann::json response;
        client.send("a", response);
        
        //Assert
        EXPECT_EQ(expected_message, output_message);
        EXPECT_EQ(true, response.is_array());
        EXPECT_EQ(2, response.size());
        EXPECT_EQ(1, response[0]);
        EXPECT_EQ(1, response[1].is_string());
        EXPECT_EQ("Went to bed early", response[1]);
}

TEST_F(romiserialclient_tests, error_reponse_without_message)
{
        // Arrange
        EXPECT_CALL(*in, set_timeout(_));
        std::string client_name("romiserialclient_tests");
        RomiSerialClient client(in, out, 255, client_name);
        setExpectedOutput("#a:008e\r\n");
        initInput("#a[1]:0085\r\n");

        // Act
        nlohmann::json response;
        try{
            client.send("a", response);
        }
        catch (nlohmann::json::exception& ex)
        {
            std::cout << ex.what() << std::endl;
        }

        
        //Assert
        EXPECT_EQ(expected_message, output_message);
        EXPECT_EQ(true, response.is_array());
        EXPECT_EQ(2, response.size());
        EXPECT_EQ(1, response[0]);
        EXPECT_EQ(1, response[1].is_string());
}

TEST_F(romiserialclient_tests, log_message)
{
        // Arrange
        EXPECT_CALL(*in, set_timeout(_));
        std::string client_name("romiserialclient_tests");
        RomiSerialClient client(in, out, 255, client_name);
        setExpectedOutput("#a:008e\r\n");
        initInput("#!LOG MESSAGE:008e\r\n#a[0]:00e7\r\n");

        // Act
        nlohmann::json response;
        client.send("a", response);
        
        //Assert
        EXPECT_EQ(expected_message, output_message);
        EXPECT_EQ(true, response.is_array());
        EXPECT_EQ(1, response.size());
        EXPECT_EQ(0, response[0]);
}

TEST_F(romiserialclient_tests, message_too_long)
{
        // Arrange
        EXPECT_CALL(*in, set_timeout(_));
        std::string client_name("romiserialclient_tests");
        RomiSerialClient client(in, out, 255, client_name);

        // Act
        nlohmann::json response;

        client.send("a[\"0123456789012345678901234567890123456789"
                    "012345678901234567890123456789\"]",
                    response);
        
        //Assert
        EXPECT_EQ(true, response.is_array());
        EXPECT_EQ(2, response.size());
        EXPECT_EQ(kClientTooLong, response[0]);
}

TEST_F(romiserialclient_tests, error_number_has_string_representation)
{
        // Arrange

        // Act

        //Assert
        EXPECT_STREQ("No error", RomiSerialClient::get_error_message(0));
        EXPECT_STREQ("Application error", RomiSerialClient::get_error_message(1));
        EXPECT_STREQ("Unknown error code", RomiSerialClient::get_error_message(-99399));
        
        for (int i = kNoError; i > kLastError; i--) {
  //              EXPECT_EQ("Unknown error code", std::string(RomiSerialClient::get_error_message(i)));
            std::string actual = RomiSerialClient::get_error_message(i);
            ASSERT_FALSE(actual.empty());
        }
}

