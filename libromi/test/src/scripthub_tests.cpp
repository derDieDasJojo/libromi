#include "gtest/gtest.h"
#include "ui/ScriptList.h"
#include "mock_scriptlist.h"
#include "rpc/ScriptHub.h"
#include "rpc/ScriptHubListener.h"
#include <test_utility.h>


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
        MockScriptList mockScriptList;
        auto scriptHubListener = std::make_shared<ScriptHubListener>(mockScriptList);
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

