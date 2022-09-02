#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "camera/CameraInfoIO.h"
#include <iostream>

using namespace std;
using namespace testing;
using namespace romi;

class camerainfoio_tests : public ::testing::Test
{
protected:

        static const constexpr double FX = 1000.0;
        static const constexpr double FY = 2000.0;
        static const constexpr double CX = 960.0;
        static const constexpr double CY = 540.0;
        static const constexpr double K1 = 1.0;
        static const constexpr double K2 = 2.0;
        
	camerainfoio_tests() {
        }

	~camerainfoio_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {
        }
};

TEST_F(camerainfoio_tests, test_load_is_doing_it_right)
{
        // Arrange
        nlohmann::json json{
                {CameraInfoIO::kCameraType, ICameraInfo::kPiCameraHQ1},
                {CameraInfoIO::kCameraID, "id"},
                {CameraInfoIO::kCameraName, "name"},
                {CameraInfoIO::kCameraLens, "lens"},
                {CameraInfoIO::kSensor,
                        {
                                {CameraInfoIO::kSensorResolution, {1, 2}},
                                {CameraInfoIO::kSensorDimensions, {1.0, 2.0}}
                        }},
                {CameraInfoIO::kCalibration,
                        {
                                {CameraInfoIO::kCalibrationDate, "date"},
                                {CameraInfoIO::kCalibrationPerson, "person"},
                                {CameraInfoIO::kCalibrationMethod, "method"}
                        }},
                {CameraInfoIO::kIntrinsics,
                        {
                                {CameraInfoIO::kFx, FX},
                                {CameraInfoIO::kFy, FY},
                                {CameraInfoIO::kCx, CX},
                                {CameraInfoIO::kCy, CY}
                        }},
                {CameraInfoIO::kSettings, {
                                {"value1", 1.0},
                                {"value2", 2.0},
                                {"option1", "value"}
                        }},
                {CameraInfoIO::kDistortion,
                        {
                                {CameraInfoIO::kDistortionType,
                                                ICameraDistortion::kSimpleRadialDistortion},
                                {CameraInfoIO::kDistortionValues, {K1, K2}}
                        }}
        };

        //std::cout << json.dump(4) << std::endl;
        
        // Act
        CameraInfoIO loader;
        auto info = loader.load(json);

        // Assert
        ASSERT_STREQ(info->get_id().c_str(), "id");
        ASSERT_STREQ(info->get_type().c_str(), ICameraInfo::kPiCameraHQ1);
        ASSERT_STREQ(info->get_name().c_str(), "name");
        ASSERT_STREQ(info->get_lens().c_str(), "lens");
        ASSERT_EQ(info->get_sensor_resolution().first, 1);
        ASSERT_EQ(info->get_sensor_resolution().second, 2);
        ASSERT_EQ(info->get_sensor_dimensions().first, 1.0);
        ASSERT_EQ(info->get_sensor_dimensions().second, 2.0);
        ASSERT_STREQ(info->get_calibration_date().c_str(), "date");
        ASSERT_STREQ(info->get_calibration_person().c_str(), "person");
        ASSERT_STREQ(info->get_calibration_method().c_str(), "method");

        ICameraIntrinsics& intrinsics = info->get_intrinsics();
        double fx, fy, cx, cy;
        intrinsics.get_focal_length(fx, fy);
        intrinsics.get_central_point(cx, cy);
        ASSERT_EQ(fx, FX);
        ASSERT_EQ(fy, FY);
        ASSERT_EQ(cx, CX);
        ASSERT_EQ(cy, CY);

        ICameraDistortion& distortion = info->get_distortion();
        std::vector<double> values;
        distortion.get(values);
        ASSERT_STREQ(distortion.get_type().c_str(),
                     ICameraDistortion::kSimpleRadialDistortion);
        ASSERT_EQ(values.size(), 2);
        ASSERT_EQ(values[0], K1);
        ASSERT_EQ(values[1], K2);

        ICameraSettings& settings = info->get_settings();
        std::string option;
        settings.get_option("option1", option);
        ASSERT_EQ(settings.get_value("value1"), 1.0);
        ASSERT_EQ(settings.get_value("value2"), 2.0);
        ASSERT_STREQ(option.c_str(), "value");
}

TEST_F(camerainfoio_tests, test_load_throws_error_for_unknown_type)
{
        // Arrange
        nlohmann::json json{
                {CameraInfoIO::kCameraType, "no-good"},
                {CameraInfoIO::kCameraID, "whatever"},
                {CameraInfoIO::kIntrinsics,
                        {
                                {CameraInfoIO::kFx, FX},
                                {CameraInfoIO::kFy, FY},
                                {CameraInfoIO::kCx, CX},
                                {CameraInfoIO::kCy, CY}
                        }},
                {CameraInfoIO::kSettings, {
                                {"value1", 1.0},
                                {"value2", 2.0},
                                {"option1", "value"}
                        }},
                {CameraInfoIO::kDistortion,
                        {
                                {CameraInfoIO::kDistortionType,
                                                ICameraDistortion::kSimpleRadialDistortion},
                                {CameraInfoIO::kDistortionValues, {K1, K2}}
                        }}
        };

        CameraInfoIO loader;

        // Act
        
        try {
                loader.load(json);
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

TEST_F(camerainfoio_tests, test_load_throws_error_for_unknown_distortion)
{
        // Arrange
        nlohmann::json json{
                {CameraInfoIO::kCameraType, ICameraInfo::kPiCameraHQ1},
                {CameraInfoIO::kCameraID, "whatever"},
                {CameraInfoIO::kIntrinsics,
                        {
                                {CameraInfoIO::kFx, FX},
                                {CameraInfoIO::kFy, FY},
                                {CameraInfoIO::kCx, CX},
                                {CameraInfoIO::kCy, CY}
                        }},
                {CameraInfoIO::kSettings, {
                                {"value1", 1.0},
                                {"value2", 2.0},
                                {"option1", "value"}
                        }},
                {CameraInfoIO::kDistortion,
                        {
                                {CameraInfoIO::kDistortionType, "silly-me"},
                                {CameraInfoIO::kDistortionValues, {K1, K2}}
                        }}
        };

        CameraInfoIO loader;

        // Act
        
        try {
                loader.load(json);
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

TEST_F(camerainfoio_tests, test_store)
{
        // Arrange
        nlohmann::json json{
                {CameraInfoIO::kCameraType, ICameraInfo::kPiCameraHQ1},
                {CameraInfoIO::kCameraID, "id"},
                {CameraInfoIO::kCameraName, "name"},
                {CameraInfoIO::kCameraLens, "lens"},
                {CameraInfoIO::kSensor,
                        {
                                {CameraInfoIO::kSensorResolution, {1, 2}},
                                {CameraInfoIO::kSensorDimensions, {1.0, 2.0}}
                        }},
                {CameraInfoIO::kCalibration,
                        {
                                {CameraInfoIO::kCalibrationDate, "date"},
                                {CameraInfoIO::kCalibrationPerson, "person"},
                                {CameraInfoIO::kCalibrationMethod, "method"}
                        }},
                {CameraInfoIO::kIntrinsics,
                        {
                                {CameraInfoIO::kFx, FX},
                                {CameraInfoIO::kFy, FY},
                                {CameraInfoIO::kCx, CX},
                                {CameraInfoIO::kCy, CY}
                        }},
                {CameraInfoIO::kSettings, {
                                {"value1", 1.0},
                                {"value2", 2.0},
                                {"option1", "value"}
                        }},
                {CameraInfoIO::kDistortion,
                        {
                                {CameraInfoIO::kDistortionType,
                                                ICameraDistortion::kSimpleRadialDistortion},
                                {CameraInfoIO::kDistortionValues, {K1, K2}}
                        }}
        };

        CameraInfoIO loader;
        auto info = loader.load(json);
        
        // Act
        nlohmann::json out = loader.store(info);
        
        // Assert
        bool same = (json == out);
        ASSERT_EQ(same, true);
}

