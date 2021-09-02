#include "gtest/gtest.h"
#include "ui/ScriptList.h"
#include "rpc/ScriptHub.h"
#include "rpc/ScriptHubListener.h"
#include <test_utility.h>

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
using namespace romi;

const std::string test_data_directory = test_utility::getexepath() + "/test_data/";
const std::string test_script_filename = "scripts.json";

class scripthub_tests : public ::testing::Test
{
protected:
        
	scripthub_tests() {
	}

	~scripthub_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
};

TEST_F(scripthub_tests, can_construct_scripthub)
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
    MockRemoteStateInputDevice mockRemoteStateInputDevice;

    romi::Rover rover(mockInputDevice, mockDisplay, mockSpeedController, mockNavigation, mockEventTimer,
                      mockMenu, mockScriptEngine, mockNotifications, mockWeeder, mockImager, mockRemoteStateInputDevice);

        MockScriptList mockScriptList;
        auto scriptHubListener = std::make_shared<ScriptHubListener>(rover);
        ScriptHub scriptHub(scriptHubListener, 20000);
}

TEST_F(scripthub_tests, successfully_load_simple_scriplist)
{
        std::string test_script_path = test_data_directory + test_script_filename;
        ScriptList scripts(test_script_path);

        ASSERT_EQ(scripts.size(), 5);
}

//TEST_F(scripthub_tests, can_connect)
//{
//    std::string test_script_path = test_data_directory + test_script_filename;
//    ScriptList scripts(test_script_path);
//    auto scriptHubListener = std::make_shared<ScriptHubListener>(scripts);
//    ScriptHub scriptHub(scriptHubListener, 20000);
//    scriptHubListener->onmessage()
//    scriptHub.handle_events();
//    ASSERT_EQ(scripts.size(), 5);
//}

