#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "camera/CameraIntrinsics.h"

using namespace std;
using namespace testing;
using namespace romi;

class cameraintrinsics_tests : public ::testing::Test
{
public:

        static const constexpr double FX = 1000.0;
        static const constexpr double FY = 2000.0;
        static const constexpr double CX = 960.0;
        static const constexpr double CY = 540.0;
        static const constexpr double TOO_LARGE = 10000000000.0;
        static const constexpr double NEGATIVE = -1.0;
        
protected:
        
	cameraintrinsics_tests() {
        }

	~cameraintrinsics_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {
        }
};

TEST_F(cameraintrinsics_tests, test_constructor_1)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics;
        } catch (...) {
                FAIL() << "Didn't expected an error";
        }

        // Assert
}

TEST_F(cameraintrinsics_tests, test_constructor_2)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(FX, FY, CX, CY);
        } catch (...) {
                FAIL() << "Didn't expected an error";
        }

        // Assert
}

TEST_F(cameraintrinsics_tests, test_constructor_throws_an_exception_1)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(NEGATIVE, FY, CX, CY);
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

TEST_F(cameraintrinsics_tests, test_constructor_throws_an_exception_2)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(TOO_LARGE, FY, CX, CY);
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

TEST_F(cameraintrinsics_tests, test_constructor_throws_an_exception_3)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(FX, NEGATIVE, CX, CY);
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

TEST_F(cameraintrinsics_tests, test_constructor_throws_an_exception_4)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(FX, TOO_LARGE, CX, CY);
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

TEST_F(cameraintrinsics_tests, test_constructor_throws_an_exception_5)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(FX, FY, NEGATIVE, CY);
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

TEST_F(cameraintrinsics_tests, test_constructor_throws_an_exception_6)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(FX, FY, TOO_LARGE, CY);
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

TEST_F(cameraintrinsics_tests, test_constructor_throws_an_exception_7)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(FX, FY, CX, NEGATIVE);
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

TEST_F(cameraintrinsics_tests, test_constructor_throws_an_exception_8)
{
        // Arrange

        // Act
        try {
                CameraIntrinsics intrinsics(FX, FY, CX, TOO_LARGE);
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

TEST_F(cameraintrinsics_tests, test_constructor_values)
{
        // Arrange
        CameraIntrinsics intrinsics(FX, FY, CX, CY);

        // Act
        double fx, fy;
        double cx, cy;
        intrinsics.get_focal_length(fx, fy);
        intrinsics.get_central_point(cx, cy);

        // Assert
        ASSERT_EQ(fx, FX);
        ASSERT_EQ(fy, FY);
        ASSERT_EQ(cx, CX);
        ASSERT_EQ(cy, CY);
}

TEST_F(cameraintrinsics_tests, test_set_focal_length)
{
        // Arrange
        CameraIntrinsics intrinsics;

        // Act
        intrinsics.set_focal_length(FX, FY);

        // Assert
        double fx, fy;
        intrinsics.get_focal_length(fx, fy);
        ASSERT_EQ(fx, FX);
        ASSERT_EQ(fy, FY);
}

TEST_F(cameraintrinsics_tests, test_set_central_point)
{
        // Arrange
        CameraIntrinsics intrinsics;

        // Act
        intrinsics.set_central_point(CX, CY);

        // Assert
        double cx, cy;
        intrinsics.get_central_point(cx, cy);
        ASSERT_EQ(cx, CX);
        ASSERT_EQ(cy, CY);
}
