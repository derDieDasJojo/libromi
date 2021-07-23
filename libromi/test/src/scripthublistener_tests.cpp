#include "gtest/gtest.h"
#include "ui/ScriptList.h"
#include "mock_scriptlist.h"
#include "rpc/ScriptHub.h"
#include "rpc/ScriptHubListener.h"
#include <test_utility.h>
#include "WebSocket.mock.h"
#include <FileUtils.h>


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
    // Arrange
    std::string test_script_path = test_data_directory + test_script_filename;
    ScriptList scripts(test_script_path);
    // Actscripts
    // Assert
    auto scriptHubListener = std::make_shared<ScriptHubListener>(scripts);

}

TEST_F(scripthublistener_tests, onmessage_binary_message_throws)
{
    // Arrange
    std::string test_script_path = test_data_directory + test_script_filename;
    ScriptList scripts(test_script_path);
    auto scriptHubListener = std::make_shared<ScriptHubListener>(scripts);
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