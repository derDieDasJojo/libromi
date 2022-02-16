#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "rpc/NavigationAdaptor.h"
#include "rpc/MethodsNavigation.h"
#include "mock_navigation.h"

using namespace std;
using namespace testing;
using namespace romi;

class rpcnavigation_tests : public ::testing::Test
{
protected:
        MockNavigation navigation;
        
	rpcnavigation_tests() : navigation() {
	}

	~rpcnavigation_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
};

TEST_F(rpcnavigation_tests, test_execute_unknown_command_returns_error)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params;
        nlohmann::json result;
        RPCError error;

        adaptor.execute("dummy", params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_move_command_returns_ok)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'distance':1,'speed':0.1}");
        nlohmann::json result;
        RPCError error;

        EXPECT_CALL(navigation, move(1.0, 0.1))
                .WillOnce(Return(true));

        adaptor.execute(MethodsNavigation::move, params, result, error);

        ASSERT_EQ(error.code, 0);
}

TEST_F(rpcnavigation_tests, test_move_command_returns_error_on_missing_parameters_1)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params;
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::move, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_move_command_returns_error_on_missing_parameters_2)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'distance':0}");
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::move, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_move_command_returns_error_on_invalid_parameters_1)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'distance':'foo'}");
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::move, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_move_command_returns_error_on_invalid_parameters_2)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'distance':10,'speed':'foo'}");
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::move, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_move_returns_error_when_move_fails)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'distance':1,'speed':0.1}");
        nlohmann::json result;
        RPCError error;

        EXPECT_CALL(navigation, move(1.0, 0.1))
                .WillOnce(Return(false));

        adaptor.execute(MethodsNavigation::move, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}





TEST_F(rpcnavigation_tests, test_moveat_returns_ok)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'speed':[0.1,0.2]}");
        nlohmann::json result;
        RPCError error;

        EXPECT_CALL(navigation, moveat(0.1, 0.2))
                .WillOnce(Return(true));

        adaptor.execute(MethodsNavigation::moveat, params, result, error);

        ASSERT_EQ(error.code, 0);
}

TEST_F(rpcnavigation_tests, test_moveat_returns_error_on_missing_parameters_1)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'command':'moveat'}");
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::moveat, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_moveat_returns_error_on_invalid_parameters_1)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'speed':'foo'}");
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::moveat, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_moveat_returns_error_on_invalid_parameters_2)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'speed':['foo','bar']}");
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::moveat, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_moveat_returns_error_on_invalid_parameters_3)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'speed':[0.1,'bar']}");
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::moveat, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_moveat_returns_error_on_invalid_parameters_4)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'speed':['foo',0.2]}");
        nlohmann::json result;
        RPCError error;

        adaptor.execute(MethodsNavigation::moveat, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}

TEST_F(rpcnavigation_tests, test_moveat_returns_error_when_moveat_fails)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params = nlohmann::json::parse("{'speed':[0.1,0.2]}");
        nlohmann::json result;
        RPCError error;

        EXPECT_CALL(navigation, moveat(0.1, 0.2))
                .WillOnce(Return(false));

        adaptor.execute(MethodsNavigation::moveat, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}



TEST_F(rpcnavigation_tests, test_stop_returns_ok)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params;
        nlohmann::json result;
        RPCError error;

        EXPECT_CALL(navigation, stop())
                .WillOnce(Return(true));

        adaptor.execute(MethodsNavigation::stop, params, result, error);

        ASSERT_EQ(error.code, 0);
}

TEST_F(rpcnavigation_tests, test_stop_returns_error_when_stop_fails)
{
        NavigationAdaptor adaptor(navigation);

        nlohmann::json params;
        nlohmann::json result;
        RPCError error;

        EXPECT_CALL(navigation, stop())
                .WillOnce(Return(false));

        adaptor.execute(MethodsNavigation::stop, params, result, error);

        ASSERT_NE(error.code, 0);
        ASSERT_NE(error.message.length(), 0);
}
