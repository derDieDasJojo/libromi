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
#include "mock_remotestateinputdevice.h"


using namespace std;
using namespace testing;
using testing::_;
using testing::Return;
using namespace romi;

const std::string test_data_directory = test_utility::getexepath() + "/test_data/";
const std::string test_script_filename = "scripts.json";

class scripthublistener_tests : public ::testing::Test
{
protected:
        
	scripthublistener_tests()
    : actual_reply_text_(),
    mockWebSocket_(), mockInputDevice_(), mockDisplay_(), mockSpeedController_(), mockNavigation_(), mockEventTimer_(),
    mockMenu_(), mockScriptEngine_(), mockNotifications_(), mockWeeder_(), mockImager_(), mockRemoteStateInputDevice_()
    {
	}

	~scripthublistener_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
public:
    bool capture_send( rcom::MemBuffer& message, rcom::MessageType type)
    {
        (void) type;
        actual_reply_text_ = message;
        return true;
    }

public:
    rcom::MemBuffer actual_reply_text_;
    MockWebSocket mockWebSocket_;
    MockInputDevice mockInputDevice_;
    MockDisplay mockDisplay_;
    MockSpeedController mockSpeedController_;
    MockNavigation mockNavigation_;
    MockEventTimer mockEventTimer_;
    MockMenu mockMenu_;
    MockScriptEngine mockScriptEngine_;
    MockNotifications mockNotifications_;
    MockWeeder mockWeeder_;
    MockImager mockImager_;
    MockRemoteStateInputDevice mockRemoteStateInputDevice_;

};

TEST_F(scripthublistener_tests, can_construct_scripthublistener)
{

    romi::Rover rover(mockInputDevice_, mockDisplay_, mockSpeedController_, mockNavigation_, mockEventTimer_,
                      mockMenu_, mockScriptEngine_, mockNotifications_, mockWeeder_, mockImager_, mockRemoteStateInputDevice_);
    std::string test_script_path = test_data_directory + test_script_filename;
    ScriptList scripts(test_script_path);

    // Act
    // Assert
    auto scriptHubListener = std::make_shared<ScriptHubListener>(rover);

}

TEST_F(scripthublistener_tests, onmessage_binary_message_throws)
{
    // Arrange
    romi::Rover rover(mockInputDevice_, mockDisplay_, mockSpeedController_, mockNavigation_, mockEventTimer_,
                      mockMenu_, mockScriptEngine_, mockNotifications_, mockWeeder_, mockImager_, mockRemoteStateInputDevice_);

    std::string test_script_path = test_data_directory + test_script_filename;
    ScriptList scripts(test_script_path);

    auto scriptHubListener = std::make_shared<ScriptHubListener>(rover);
    MockWebSocket mockWebSocket;

    rcom::MemBuffer request;
    request.printf(R"({"request": "list"})");

    // Act
    // Assert
    ASSERT_THROW(scriptHubListener->onmessage(mockWebSocket, request, rcom::kBinaryMessage), runtime_error);
}


TEST_F(scripthublistener_tests, onmessage_list_returns_json)
{
    // Arrange
    std::string test_script_path = test_data_directory + test_script_filename;
    ScriptList scripts(test_script_path);
    EXPECT_CALL(mockScriptEngine_, scriptList())
            .WillOnce(ReturnRef(scripts));
    EXPECT_CALL(mockWebSocket_, send(_,_))
            .WillOnce(Invoke(this, &scripthublistener_tests::capture_send));

    std::string saved;
    romi::Rover rover(mockInputDevice_, mockDisplay_, mockSpeedController_, mockNavigation_, mockEventTimer_,
                      mockMenu_, mockScriptEngine_, mockNotifications_, mockWeeder_, mockImager_, mockRemoteStateInputDevice_);
    auto scriptHubListener = std::make_shared<ScriptHubListener>(rover);

    rcom::MemBuffer request;
    request.printf(R"({"request": "list"})");
    // Act
    scriptHubListener->onmessage(mockWebSocket_, request, rcom::kTextMessage);
    std::string actual = actual_reply_text_.tostring();

    // Assert
    ASSERT_NE(actual.find("move-forward"), std::string::npos);
    ASSERT_NE(actual.find("move-backward"), std::string::npos);
    ASSERT_NE(actual.find("weeding"), std::string::npos);
    ASSERT_NE(actual.find("start-recording"), std::string::npos);
    ASSERT_NE(actual.find("stop-recording"), std::string::npos);
}