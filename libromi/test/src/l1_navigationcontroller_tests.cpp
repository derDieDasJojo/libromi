#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Logger.h"
#include "rover/L1NavigationController.h"

using namespace std;
using namespace testing;
using namespace romi;

class l1_navigationcontroller_tests : public ::testing::Test
{
protected:
        
	l1_navigationcontroller_tests() {
	}

	~l1_navigationcontroller_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
};

TEST_F(l1_navigationcontroller_tests, constructor_with_good_values_succeeds)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(2.0);

                // Assert
                
        } catch (const std::exception& e) {
                // Assert
                FAIL() << "Didn't expect an exception";
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_distance_throws_error_1)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(0.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_distance_throws_error_2)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(-0.5);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_distance_throws_error_3)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(100.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, estimate_steering_on_straight_line)
{
        // Arrange
        L1NavigationController ctrl(1.0);

        // Act
        SteeringData direction = ctrl.estimate_steering(0.0, 0.0);

        // Assert
        ASSERT_EQ(direction.type_, kStraight);
        ASSERT_EQ(direction.radius_, 0.0);
}

TEST_F(l1_navigationcontroller_tests, estimate_steering_throws_exception_on_error_too_big)
{
                // Arrange
        L1NavigationController ctrl(1.0);
        
        try {
                // Act
                // cross track error > L -> can't handle
                ctrl.estimate_steering(1.01, 0.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, estimate_steering_returns_positive_radius_on_negative_offset)
{
        // Arrange
        L1NavigationController ctrl(1.0);

        // Act
        SteeringData direction = ctrl.estimate_steering(-0.1, 0.0);

        // Assert
        ASSERT_GT(direction.radius_, 0.0);
}

TEST_F(l1_navigationcontroller_tests, estimate_steering_returns_negative_radius_on_positive_offset)
{
        // Arrange
        L1NavigationController ctrl(1.0);

        // Act
        SteeringData direction = ctrl.estimate_steering(0.1, 0.0);

        // Assert
        ASSERT_LT(direction.radius_, 0.0);
}

TEST_F(l1_navigationcontroller_tests, estimate_steering_returns_positive_radius_on_negative_orientation)
{
        // Arrange
        L1NavigationController ctrl(1.0);

        // Act
        SteeringData direction = ctrl.estimate_steering(0.0, -0.1);

        // Assert
        ASSERT_GT(direction.radius_, 0.0);
}

TEST_F(l1_navigationcontroller_tests, estimate_steering_returns_negative_radius_on_positive_orientation)
{
        // Arrange
        L1NavigationController ctrl(1.0);

        // Act
        SteeringData direction = ctrl.estimate_steering(0.1, 0.1);

        // Assert
        ASSERT_LT(direction.radius_, 0.0);
}


TEST_F(l1_navigationcontroller_tests, estimate_steering_returns_expected_value)
{
        // Arrange
        /*

                /|L'
            R /  |
            /    | L 
          ---------
                  d

         L'² + d² = L²  => L'² = L² - d²       (1)
         (R-|d|)² + L'² = R²                   (2)

         => (R-|d|)² + L² - d² = R²            
         => R² + d² - 2R|d| + L² - d² = R²     (d < 0)
         => -2R|d| + L² = 0        
         => R = L² / (2|d|)                    (R > 0)

        */
        double L = 2.0;
        double d = -0.2;
        double R = L * L / (2.0 * fabs(d));
        L1NavigationController ctrl(L);
                
        // Act
        SteeringData direction = ctrl.estimate_steering(d, 0.0);

        r_debug("radius %f, expected %f", direction.radius_, R);
        
        // Assert
        ASSERT_NEAR(direction.radius_, R, 0.01);
}

TEST_F(l1_navigationcontroller_tests, estimate_steering_returns_expected_value_2)
{
        // Arrange
        double L = 2.0;
        double d = -0.092960;
        double phi = 0.037970;
        double gamma = -atan(d / sqrt(L * L - d * d));
        double theta = phi - gamma;
        double R = -L / (2.0 * sin(theta));
        L1NavigationController ctrl(L);
                
        // Act
        SteeringData direction = ctrl.estimate_steering(d, phi);

        r_debug("radius %f, expected %f", direction.radius_, R);
        
        // Assert
        ASSERT_NEAR(direction.radius_, R, 0.01);
}
