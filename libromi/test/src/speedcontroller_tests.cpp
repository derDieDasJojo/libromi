#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_navigation.h"
#include "rover/SpeedController.h"
#include "rover/SpeedConverter.h"

using namespace std;
using namespace testing;
using namespace romi;

class speedcontroller_tests : public ::testing::Test
{
protected:
        MockNavigation navigation;
        SpeedConverter fast;
        SpeedConverter accurate;
         
	speedcontroller_tests() : navigation(), fast(), accurate() {
	}

	~speedcontroller_tests() override = default;

	void SetUp() override {
                // default values
                fast.use_speed_curve = true;
                fast.speed_curve_exponent = 2.0;
                fast.use_direction_curve = true;
                fast.direction_curve_exponent = 2.0;
                fast.speed_multiplier = 1.0;
                fast.direction_multiplier = 0.4;
                
                accurate.use_speed_curve = true;
                accurate.speed_curve_exponent = 1.0;
                accurate.use_direction_curve = true;
                accurate.direction_curve_exponent = 1.0;
                accurate.speed_multiplier = 1.0;
                accurate.direction_multiplier = 0.4;
	}

	void TearDown() override {
	}
};

TEST_F(speedcontroller_tests, constructor_successfully_parses_json)
{
                nlohmann::json config = nlohmann::json::parse(
                        "{"
                        "    \"user-interface\": {"
                        "        \"speed-controller\": {"
                        "            \"fast\": {"
                        "                \"use-speed-curve\": true,"
                        "                \"speed-curve-exponent\": 2.0,"
                        "                \"use-direction-curve\": true,"
                        "                \"direction-curve-exponent\": 2.0,"
                        "                \"speed-multiplier\": 1.0,"
                        "                \"direction-multiplier\": 0.4"
                        "            },"
                        "            \"accurate\": {"
                        "                \"use-speed-curve\": true,"
                        "                \"speed-curve-exponent\": 1.0,"
                        "                \"use-direction-curve\": true,"
                        "                \"direction-curve-exponent\": 1.0,"
                        "                \"speed-multiplier\": 0.3,"
                        "                \"direction-multiplier\": 0.15"
                        "            }"
                        "        }"
                        "    }"
                        "}");
            ASSERT_NO_THROW(SpeedController controller(navigation, config));

}

TEST_F(speedcontroller_tests, constructor_throws_error_on_missing_fast_keys)
{

                nlohmann::json config = nlohmann::json::parse(
                        "{"
                        "    \"user-interface\": {"
                        "        \"speed-controller\": {"
                        "        }"
                        "    }"
                        "}");
                ASSERT_THROW(SpeedController controller(navigation, config), nlohmann::json::exception);
}

TEST_F(speedcontroller_tests, constructor_throws_error_missing_accurate_key)
{

        nlohmann::json config = nlohmann::json::parse(
                "{"
                "    \"user-interface\": {"
                "        \"speed-controller\": {"
                "            \"fast\": {"
                "                \"use-speed-curve\": true,"
                "                \"speed-curve-exponent\": 2.0,"
                "                \"use-direction-curve\": true,"
                "                \"direction-curve-exponent\": 2.0,"
                "                \"speed-multiplier\": 1.0,"
                "                \"direction-multiplier\": 0.4"
                "            },"
                "            \"accurate\": {}"
                "        }"
                "    }"
                "}");
    ASSERT_THROW(SpeedController controller(navigation, config), nlohmann::json::exception);
}

TEST_F(speedcontroller_tests, constructor_throws_range_error_on_fast_speed_curve_exponent)
{
        fast.speed_curve_exponent = 0.0;

        try {
                SpeedController controller(navigation, fast, accurate);
                FAIL() << "Expected std::range_error";

        } catch (std::range_error const &e) {
                        // NOP
        } catch (...) {
                FAIL() << "Expected std::runtime_error";
        }
}

TEST_F(speedcontroller_tests, constructor_throws_range_error_on_accurate_speed_curve_exponent)
{
        accurate.speed_curve_exponent = 0.0;

                ASSERT_THROW(SpeedController controller(navigation, fast, accurate), std::range_error);
}

TEST_F(speedcontroller_tests, constructor_throws_range_error_on_config_fast_speed_curve_exponent)
{
                nlohmann::json config = nlohmann::json::parse(
                        "{"
                        "    \"user-interface\": {"
                        "        \"speed-controller\": {"
                        "            \"fast\": {"
                        "                \"use-speed-curve\": true,"
                        "                \"speed-curve-exponent\": 0.0,"  // Bad
                        "                \"use-direction-curve\": true,"
                        "                \"direction-curve-exponent\": 2.0,"
                        "                \"speed-multiplier\": 1.0,"
                        "                \"direction-multiplier\": 0.4"
                        "            },"
                        "            \"accurate\": {"
                        "                \"use-speed-curve\": true,"
                        "                \"speed-curve-exponent\": 1.0,"
                        "                \"use-direction-curve\": true,"
                        "                \"direction-curve-exponent\": 1.0,"
                        "                \"speed-multiplier\": 0.3,"
                        "                \"direction-multiplier\": 0.15"
                        "            }"
                        "        }"
                        "    }"
                        "}");
                ASSERT_THROW(SpeedController controller(navigation, config), std::range_error);
}

TEST_F(speedcontroller_tests, constructor_throws_range_error_on_config_accurate_speed_multiplier)
{
                nlohmann::json config = nlohmann::json::parse(
                        "{"
                        "    \"user-interface\": {"
                        "        \"speed-controller\": {"
                        "            \"fast\": {"
                        "                \"use-speed-curve\": true,"
                        "                \"speed-curve-exponent\": 1.0,"
                        "                \"use-direction-curve\": true,"
                        "                \"direction-curve-exponent\": 2.0,"
                        "                \"speed-multiplier\": 1.0,"
                        "                \"direction-multiplier\": 0.4"
                        "            },"
                        "            \"accurate\": {"
                        "                \"use-speed-curve\": true,"
                        "                \"speed-curve-exponent\": 1.0,"
                        "                \"use-direction-curve\": true,"
                        "                \"direction-curve-exponent\": 1.0,"
                        "                \"speed-multiplier\": 0.0,"    // Bad
                        "                \"direction-multiplier\": 0.15"
                        "            }"
                        "        }"
                        "    }"
                        "}");
            ASSERT_THROW(SpeedController controller(navigation, config), std::range_error);
}

TEST_F(speedcontroller_tests, stop_calls_navigation_stop)
{
        SpeedController controller(navigation, fast, accurate);
        
        EXPECT_CALL(navigation, stop())
                .WillOnce(Return(true));
        
        bool success = controller.stop();

        ASSERT_EQ(success, true);
}

TEST_F(speedcontroller_tests, stop_returns_false_when_navigation_stop_fails)
{
        SpeedController controller(navigation, fast, accurate);
        
        EXPECT_CALL(navigation, stop())
                .WillOnce(Return(false));
        
        bool success = controller.stop();

        ASSERT_EQ(success, false);
}

TEST_F(speedcontroller_tests, drive_at_generates_expected_speeds)
{
        fast.use_speed_curve = false;
        fast.use_direction_curve = false;
        fast.speed_multiplier = 1.0;
        fast.direction_multiplier = 1.0;
        
        SpeedController controller(navigation, fast, accurate);
        
        // Speed 1 and direction 0: straight forward: left wheel (1)
        // and right wheel (1).
        EXPECT_CALL(navigation, moveat(1.0, 1.0))
                .WillOnce(Return(true));
        
        bool success = controller.drive_at(1.0, 0.0);

        ASSERT_EQ(success, true);
}

TEST_F(speedcontroller_tests, drive_accurately_at_generates_expected_speeds)
{
        accurate.use_speed_curve = false;
        accurate.use_direction_curve = false;
        accurate.speed_multiplier = 0.5;
        accurate.direction_multiplier = 0.5;
        
        SpeedController controller(navigation, fast, accurate);
        
        // Speed 1 and direction 0: straight forward: left wheel (1)
        // and right wheel (1).
        EXPECT_CALL(navigation, moveat(0.5, 0.5))
                .WillOnce(Return(true));
        
        bool success = controller.drive_accurately_at(1.0, 0.0);

        ASSERT_EQ(success, true);
}

TEST_F(speedcontroller_tests, spin_generates_inverse_speeds)
{
        accurate.use_speed_curve = false;
        accurate.use_direction_curve = false;
        accurate.speed_multiplier = 1.0;
        accurate.direction_multiplier = 1.0;
        
        SpeedController controller(navigation, fast, accurate);
        
        EXPECT_CALL(navigation, moveat(1.0, -1.0))
                .WillOnce(Return(true));
        
        bool success = controller.spin(1.0);

        ASSERT_EQ(success, true);
}
