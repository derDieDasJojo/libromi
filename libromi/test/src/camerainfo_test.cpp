#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "camera/SimpleRadialDistortion.h"

using namespace std;
using namespace testing;
using namespace romi;

class camerainfo_tests : public ::testing::Test
{
protected:
        
	camerainfo_tests() {
        }

	~camerainfo_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}
};

TEST_F(camerainfo_tests, test_simpleradialdistortion_constructor)
{
        // Arrange

        // Act
        SimpleRadialDistortion distortion(1.0, 2.0);

        // Assert
        ASSERT_EQ(distortion.k1(), 1.0);
        ASSERT_EQ(distortion.k2(), 2.0);
}

TEST_F(camerainfo_tests, test_simpleradialdistortion_type)
{
        // Arrange
        SimpleRadialDistortion distortion;

        // Act
        std::string& type = distortion.get_type();
                
        // Assert
        ASSERT_EQ(type, ICameraDistortion::kSimpleRadialDistortion);
}

TEST_F(camerainfo_tests, test_simpleradialdistortion_set_throws_error_1)
{
        // Arrange
        SimpleRadialDistortion distortion;
        std::vector<double> values{0.0, 0.0, 0.0};
        
        // Act
        try {
                distortion.set(values);
                FAIL() << "Expected std::runtime_error";
        }  catch (std::runtime_error const &re) {
                // Assert
                
        } catch (std::exception const &e) {
                std::cout << e.what() << std::endl;
                FAIL() << "Expected std::runtime_error";
        } catch (...) {
                FAIL() << "Expected std::runtime_error";
        }
}

TEST_F(camerainfo_tests, test_simpleradialdistortion_set_values)
{
        // Arrange
        SimpleRadialDistortion distortion;
        std::vector<double> values{1.0, 2.0};
        
        // Act
        distortion.set(values);

        // Assert
        ASSERT_EQ(distortion.k1(), 1.0);
        ASSERT_EQ(distortion.k2(), 2.0);
}

TEST_F(camerainfo_tests, test_simpleradialdistortion_set_throws_error_2)
{
        // Arrange
        SimpleRadialDistortion distortion;
        std::string name = "unknown";
        
        try {
                // Act
                distortion.set(name, 0.0);
                FAIL() << "Expected std::runtime_error";
        }  catch (std::runtime_error const &re) {
                // Assert
                
        } catch (std::exception const &e) {
                std::cout << e.what() << std::endl;
                FAIL() << "Expected std::runtime_error";
        } catch (...) {
                FAIL() << "Expected std::runtime_error";
        }
}

TEST_F(camerainfo_tests, test_simpleradialdistortion_set)
{
        // Arrange
        SimpleRadialDistortion distortion;
        
        // Act
        distortion.set(ICameraDistortion::kRadialDistortionK1, 1.0);
        distortion.set(ICameraDistortion::kRadialDistortionK2, 2.0);

        // Assert
        ASSERT_EQ(distortion.k1(), 1.0);
        ASSERT_EQ(distortion.k2(), 2.0);
}

TEST_F(camerainfo_tests, test_simpleradialdistortion_get_throws_error)
{
        // Arrange
        SimpleRadialDistortion distortion;
        std::string name = "unknown";
        
        try {
                // Act
                distortion.get(name);
                FAIL() << "Expected std::runtime_error";
        }  catch (std::runtime_error const &re) {
                // Assert
                
        } catch (std::exception const &e) {
                std::cout << e.what() << std::endl;
                FAIL() << "Expected std::runtime_error";
        } catch (...) {
                FAIL() << "Expected std::runtime_error";
        }
}

TEST_F(camerainfo_tests, test_simpleradialdistortion_get_values)
{
        // Arrange
        SimpleRadialDistortion distortion(1.0, 2.0);
        std::vector<double> values{0.0, 0.0, 0.0};
        
        // Act
        distortion.get(values);

        // Assert
        ASSERT_EQ(values.size(), 2);
        ASSERT_EQ(values[0], 1.0);
        ASSERT_EQ(values[1], 2.0);        
}

TEST_F(camerainfo_tests, test_simpleradialdistortion_get)
{
        // Arrange
        SimpleRadialDistortion distortion(1.0, 2.0);
        
        // Act
        double k1 = distortion.get(ICameraDistortion::kRadialDistortionK1);
        double k2 = distortion.get(ICameraDistortion::kRadialDistortionK2);

        // Assert
        ASSERT_EQ(k1, 1.0);
        ASSERT_EQ(k2, 2.0);        
}
