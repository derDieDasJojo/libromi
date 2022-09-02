#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "camera/SimpleRadialDistortion.h"

using namespace std;
using namespace testing;
using namespace romi;

class simpleradialdistortion_tests : public ::testing::Test
{
protected:

        static const constexpr double K1 = 1.0;
        static const constexpr double K2 = 2.0;
        
	simpleradialdistortion_tests() {
        }

	~simpleradialdistortion_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}
};

TEST_F(simpleradialdistortion_tests, test_constructor)
{
        // Arrange

        // Act
        SimpleRadialDistortion distortion(K1, K2);

        // Assert
        ASSERT_EQ(distortion.k1(), K1);
        ASSERT_EQ(distortion.k2(), K2);
}

TEST_F(simpleradialdistortion_tests, test_constructor_with_vector)
{
        // Arrange
        std::vector<double> values{K1, K2};
        // Act
        SimpleRadialDistortion distortion(values);

        // Assert
        ASSERT_EQ(distortion.k1(), K1);
        ASSERT_EQ(distortion.k2(), K2);
}

TEST_F(simpleradialdistortion_tests, test_get_type)
{
        // Arrange
        SimpleRadialDistortion distortion;

        // Act
        std::string& type = distortion.get_type();
                
        // Assert
        ASSERT_EQ(type, ICameraDistortion::kSimpleRadialDistortion);
}

TEST_F(simpleradialdistortion_tests, test_set_throws_error_1)
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

TEST_F(simpleradialdistortion_tests, test_set_values)
{
        // Arrange
        SimpleRadialDistortion distortion;
        std::vector<double> values{K1, K2};
        
        // Act
        distortion.set(values);

        // Assert
        ASSERT_EQ(distortion.k1(), K1);
        ASSERT_EQ(distortion.k2(), K2);
}

TEST_F(simpleradialdistortion_tests, test_set_throws_error_2)
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

TEST_F(simpleradialdistortion_tests, test_set)
{
        // Arrange
        SimpleRadialDistortion distortion;
        
        // Act
        distortion.set(SimpleRadialDistortion::kK1, K1);
        distortion.set(SimpleRadialDistortion::kK2, K2);

        // Assert
        ASSERT_EQ(distortion.k1(), K1);
        ASSERT_EQ(distortion.k2(), K2);
}

TEST_F(simpleradialdistortion_tests, test_get_throws_error)
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

TEST_F(simpleradialdistortion_tests, test_get_values)
{
        // Arrange
        SimpleRadialDistortion distortion(K1, K2);
        std::vector<double> values{0.0, 0.0, 0.0};
        
        // Act
        distortion.get(values);

        // Assert
        ASSERT_EQ(values.size(), 2);
        ASSERT_EQ(values[0], K1);
        ASSERT_EQ(values[1], K2);        
}

TEST_F(simpleradialdistortion_tests, test_get)
{
        // Arrange
        SimpleRadialDistortion distortion(K1, K2);
        
        // Act
        double k1 = distortion.get(SimpleRadialDistortion::kK1);
        double k2 = distortion.get(SimpleRadialDistortion::kK2);

        // Assert
        ASSERT_EQ(k1, K1);
        ASSERT_EQ(k2, K2);        
}
