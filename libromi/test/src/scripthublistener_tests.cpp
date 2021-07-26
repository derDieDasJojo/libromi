#include "gtest/gtest.h"
#include "ui/ScriptList.h"
#include "rpc/ScriptHub.h"
#include "rpc/ScriptHubListener.h"
#include <test_utility.h>
#include "WebSocket.mock.h"
#include <FileUtils.h>

////
#include "mock_scriptlist.h"

#include "mock_inputdevice.h"
#include "mock_display.h"
#include "mock_speedcontroller.h"
#include "mock_navigation.h"
#include "mock_eventtimer.h"
#include "mock_menu.h"
#include "mock_scriptengine.h"
#include "mock_notifications.h"
#include "mock_weeder.h"
#include "mock_imager.h"


using namespace std;
using namespace testing;
using testing::_;
using namespace romi;

const std::string test_data_directory = test_utility::getexepath() + "/test_data/";
const std::string test_script_filename = "scripts.json";

class scripthublistener_tests : public ::testing::Test
{
protected:
        
	scripthublistener_tests() : mockWebSocket_(), actual_reply_text_() {
	}

	~scripthublistener_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
public:
    bool capture_send( rpp::MemBuffer& message, rcom::MessageType type)
    {
        (void) type;
        actual_reply_text_ = message;
        return true;
    }

public:
    MockWebSocket mockWebSocket_;
    rpp::MemBuffer actual_reply_text_;
};

TEST_F(scripthublistener_tests, can_construct_scripthublistener)
{


    MockInputDevice mockInputDevice;
    MockDisplay mockDisplay;
    MockSpeedController mockSpeedController;
    MockNavigation mockNavigation;
    MockEventTimer mockEventTimer;
    MockMenu mockMenu;
    MockScriptEngine mockScriptEngine;
    MockNotifications mockNotifications;
    MockWeeder mockWeeder;
    MockImager mockImager;

    romi::Rover rover(mockInputDevice, mockDisplay, mockSpeedController, mockNavigation, mockEventTimer,
                      mockMenu, mockScriptEngine, mockNotifications, mockWeeder, mockImager);
    std::string test_script_path = test_data_directory + test_script_filename;
    ScriptList scripts(test_script_path);

    // Actscripts
    // Assert
    auto scriptHubListener = std::make_shared<ScriptHubListener>(scripts, rover);

}

TEST_F(scripthublistener_tests, onmessage_binary_message_throws)
{
    // Arrange
    MockInputDevice mockInputDevice;
    MockDisplay mockDisplay;
    MockSpeedController mockSpeedController;
    MockNavigation mockNavigation;
    MockEventTimer mockEventTimer;
    MockMenu mockMenu;
    MockScriptEngine mockScriptEngine;
    MockNotifications mockNotifications;
    MockWeeder mockWeeder;
    MockImager mockImager;

    romi::Rover rover(mockInputDevice, mockDisplay, mockSpeedController, mockNavigation, mockEventTimer,
                      mockMenu, mockScriptEngine, mockNotifications, mockWeeder, mockImager);

    std::string test_script_path = test_data_directory + test_script_filename;
    ScriptList scripts(test_script_path);
    auto scriptHubListener = std::make_shared<ScriptHubListener>(scripts, rover);
    MockWebSocket mockWebSocket;

    rpp::MemBuffer request;
    request.printf(R"({"request": "list"})");

    // Act
    // Assert
    ASSERT_THROW(scriptHubListener->onmessage(mockWebSocket, request, rcom::kBinaryMessage), runtime_error);
}


//TEST_F(scripthublistener_tests, onmessage_list_returns_json)
//{
//    // Arrange
//    EXPECT_CALL(mockWebSocket_, send(_,_))
//            .WillOnce(Invoke(this, &scripthublistener_tests::capture_send));
//    std::string test_script_path = test_data_directory + test_script_filename;
//
////    std::string expected_script = FileUtils::TryReadFileAsString(test_script_path);
//
//    std::string saved;
//    ScriptList scripts(test_script_path);
//    auto scriptHubListener = std::make_shared<ScriptHubListener>(scripts);
//
//    rpp::MemBuffer request;
//    request.printf(R"({"request": "list"})");
//    // Act
//    scriptHubListener->onmessage(mockWebSocket_, request, rcom::kTextMessage);
//
////    auto parsed_actual = JsonCpp::parse(actual_reply_text_);
//    std::string actual = actual_reply_text_.tostring();
////    parsed_actual.tostring(actual, k_json_pretty | k_json_sort_keys);
//
//    auto parsed_expected  = JsonCpp::load(test_script_path.c_str());
//    std::string expected;
//    parsed_expected.tostring(actual, k_json_pretty | k_json_sort_keys);
//
//    // Assert
//    ASSERT_EQ(actual, expected);
//}