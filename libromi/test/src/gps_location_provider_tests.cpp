#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "data_provider/GpsLocationProvider.h"
#include "mock_gps.h"
#include "json.hpp"

using namespace std;
using namespace testing;

class gps_location_provider_tests : public ::testing::Test
{
protected:
        
	gps_location_provider_tests() {
        }

	~gps_location_provider_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}
};

TEST_F(gps_location_provider_tests, can_construct)
{
        // Arrange
        MockGps gps;
        // Act
        // Assert
        ASSERT_NO_THROW(romi::GpsLocationProvider locationProvider(MockGps));
}

TEST_F(gps_location_provider_tests, will_create_location)
{
        // Arrange
        double latitude = 0.1;
        double longitude = 0.2;

        nlohmann::json expected{
                {"longitude" , 0.200000 },
                {"latitude" , 0.100000 },
        };

       MockGps gps;
        EXPECT_CALL(gps, CurrentLocation(_,_))
                .WillRepeatedly(DoAll(SetArgReferee<0>(latitude),
                                      SetArgReferee<1>(longitude)));
        
        romi::GpsLocationProvider locationProvider(gps);

        // Act
        nlohmann::json actual = locationProvider.location();

        // Assert
        ASSERT_EQ(actual.dump(4), expected.dump(4));
}
