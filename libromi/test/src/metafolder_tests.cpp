#include <fstream>
#include <string>
#include <future>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "util/FileUtils.h"
#include "util/ClockAccessor.h"
#include "data_provider/JsonFieldNames.h"
#include "data_provider/RoverIdentityProvider.h"
#include "data_provider/GpsLocationProvider.h"
#include "session/MetaFolder.h"

#include "mock_romidevicedata.h"
#include "mock_softwareversion.h"
#include "mock_clock.h"
#include "mock_gps.h"

using namespace std;
using namespace testing;

uint8_t red_test_image[] = { 0,0,0,  32,0,0,  64,0,0,  96,0,0,
                  32,0,0, 64,0,0,  96,0,0,  128,0,0,
                  64,0,0, 96,0,0,  128,0,0, 160,0,0,
                  96,0,0, 128,0,0, 160,0,0, 192,0,0 };

class metafolder_tests : public ::testing::Test
{
protected:
        
	metafolder_tests() : mockClock_(std::make_shared<romi::MockClock>()), mockGps_(),
                             mockLocationProvider_(),
                             deviceData_(),
                             softwareVersion_(),
                             versionCurrent_("V1.0.1"),
                             versionAlternate_("V1.0.1"),
                             devicetype_("Rover"),
                             devicID_("DEAD-BEEF"),
                             session_path_("./session")
	{
        }

	~metafolder_tests() override = default;

	void SetUp() override {
                EXPECT_CALL(mockGps_, CurrentLocation(_,_))
                        .WillRepeatedly(DoAll(testing::SetArgReferee<0>(0.1),
                                              testing::SetArgReferee<1>(0.2)));
                
                romi::ClockAccessor::SetInstance(mockClock_);
                mockLocationProvider_
                        = std::make_shared<romi::GpsLocationProvider>(mockGps_);
                if (fs::is_directory(session_path_)) {
                        fs::remove_all(session_path_);
                }
        }

	void TearDown() override {
                romi::ClockAccessor::SetInstance(nullptr);
	}

	void SetDeviceIDDataExpectations(const std::string& deviceType,
                                         const std::string& deviceId, int times) {
                EXPECT_CALL(deviceData_, RomiDeviceType)
                        .Times(times)
                        .WillRepeatedly(Return(deviceType));
                EXPECT_CALL(deviceData_, RomiDeviceHardwareId)
                        .Times(times)
                        .WillRepeatedly(Return(deviceId));
        }

        void SetSoftwareVersionDDataExpectations(const std::string& versionCurrent,
                                                 const std::string& versionAlternate) {
                EXPECT_CALL(softwareVersion_, SoftwareVersionCurrent)
                        .WillOnce(Return(versionCurrent));
                EXPECT_CALL(softwareVersion_, SoftwareVersionAlternate)
                        .WillOnce(Return(versionAlternate));
        }


        std::shared_ptr<romi::MockClock> mockClock_;
        MockGps mockGps_;
        std::shared_ptr<romi::ILocationProvider> mockLocationProvider_;
        MockRomiDeviceData deviceData_;
        MockSoftwareVersion softwareVersion_;
        const std::string versionCurrent_;
        const std::string versionAlternate_;
        const std::string devicetype_;
        const std::string devicID_;
        const std::filesystem::path session_path_;
};

TEST_F(metafolder_tests, construct_throws_when_roveridentity_null)
{
      // Arrange
      auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
      std::shared_ptr<romi::RoverIdentityProvider> roverIdentity;

      // Act
      // Assert
      ASSERT_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                mockLocationProvider_,
                                                session_path_),
                   std::invalid_argument);
}

TEST_F(metafolder_tests, construct_throws_when_locationprovider_null)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        std::shared_ptr<romi::GpsLocationProvider> mockLocationProvider_;
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);

        // Act
        // Assert
        ASSERT_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                  mockLocationProvider_,
                                                  session_path_),
                     std::invalid_argument);
}

TEST_F(metafolder_tests, can_construct)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);

        // Act
        // Assert
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_path_));
}

TEST_F(metafolder_tests, construct_creates_folder)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);

        // Act
        // Assert
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_path_));
        ASSERT_TRUE(fs::is_directory(session_path_));
}

TEST_F(metafolder_tests, construct_throws_on_fail_create)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);
        fs::path rootPath("/\\");

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);

        // Act
        // Assert
        ASSERT_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                  mockLocationProvider_,
                                                  rootPath),
                     std::runtime_error);
        ASSERT_FALSE(fs::is_directory(session_path_));
}

TEST_F(metafolder_tests, construct_creates_metadata_file)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);

        // Act
        // Assert
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_path_));
        ASSERT_TRUE(fs::is_directory(session_path_));
        ASSERT_TRUE(fs::exists(session_path_ / romi::MetaFolder::meta_data_filename_));
}

TEST_F(metafolder_tests, construct_basic_metadata_file_contain_correct_data)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        // Act
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto identityJson = metaDataJson[JsonFieldNames::romi_identity.c_str()];

        // Assert
        ASSERT_TRUE(metaDataJson.contains(JsonFieldNames::romi_identity));
        ASSERT_TRUE(identityJson.contains(JsonFieldNames::romi_device_type));
        ASSERT_EQ(identityJson[JsonFieldNames::romi_device_type], devicetype_);
        ASSERT_TRUE(identityJson.contains(JsonFieldNames::romi_hardware_id));
        ASSERT_EQ(identityJson[JsonFieldNames::romi_hardware_id], devicID_);
        ASSERT_TRUE(identityJson.contains(JsonFieldNames::software_version_current));
        ASSERT_EQ(identityJson[JsonFieldNames::software_version_current], versionCurrent_);
        ASSERT_TRUE(identityJson.contains(JsonFieldNames::software_version_alternate));
        ASSERT_EQ(identityJson[JsonFieldNames::software_version_alternate],
                  versionAlternate_);
}

TEST_F(metafolder_tests, construct_multiple_times_does_not_leak)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);

        std::filesystem::path session_2 = session_path_ / "multitest";

        // Act
        // Assert
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_2));
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_2 += "1"));
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_2 += "2"));
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_2 += "3"));
}

TEST_F(metafolder_tests, construct_multiple_times_does_not_stack_folders)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);

        std::filesystem::path session_2 = session_path_ / "multitest";

        // Act
        // Assert
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_2));
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_2 += "1"));
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_2 += "2"));
        ASSERT_NO_THROW(romi::MetaFolder meta_folder(roverIdentity,
                                                     mockLocationProvider_,
                                                     session_2 += "3"));
        ASSERT_TRUE(fs::is_directory(session_2));
}

TEST_F(metafolder_tests, store_jpg_no_extension_creates_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .WillOnce(Return(expected));
        
        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("file1");
        std::string filename_jpg("file1.jpg");
        std::string observation("observe");

        // Act
        meta_folder.try_store_jpg(filename, image, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_jpg.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_jpg_wrong_extension_changes_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .WillOnce(Return(expected));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("file1.xxx");
        std::string filename_jpg("file1.jpg");
        std::string observation("observe");

        // Act
        meta_folder.try_store_jpg(filename, image, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_jpg.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_jpg_same_file_rewrites_metadata)
{
        // Arrange
        const int number_files(2);
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .Times(number_files)
                .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                .WillOnce(Return(expected));
        
        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("file1.jpg");
        std::string observation("observe");

        // Act
        meta_folder.try_store_jpg(filename, image, observation);
        meta_folder.try_store_jpg(filename, image, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_jpg_write_error_does_not_write_metadata)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("invalid*/*86.jpg");
        std::string observation("observe");

        // Act
        ASSERT_THROW(meta_folder.try_store_jpg(filename, image, observation),
                     std::runtime_error);

        // Assert
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        ASSERT_THROW(metaDataJson.at(filename), nlohmann::json::exception);
}

TEST_F(metafolder_tests, store_jpg_creates_files_and_correct_meta_data)
{
        // Arrange
        const int number_files(3);
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .Times(number_files)
                        .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                        .WillOnce(Return(std::string("10:25:13-25/01/2020")))
                        .WillOnce(Return(std::string("10:25:24-25/01/2020")));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename1("file1.jpg");
        std::string filename2("file2.jpg");
        std::string filename3("file3.jpg");
        std::string observation("observe");

        // Act
        meta_folder.try_store_jpg(filename1, image, observation);
        meta_folder.try_store_jpg(filename2, image, observation);
        meta_folder.try_store_jpg(filename3, image, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);

        // Assert
        ASSERT_TRUE(metaDataJson.contains(filename1));
        ASSERT_TRUE(metaDataJson.contains(filename2));
        ASSERT_TRUE(metaDataJson.contains(filename3));

        ASSERT_TRUE(fs::exists(session_path_ / filename1));
        ASSERT_TRUE(fs::exists(session_path_ / filename2));
        ASSERT_TRUE(fs::exists(session_path_ / filename3));
}
TEST_F(metafolder_tests, store_jpg_empty_image_throws)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image;
        std::string filename1("file1.jpg");
        std::string observation("observe");

        // Act
        // Assert
        ASSERT_THROW(meta_folder.try_store_jpg(filename1, image, observation),
                     std::runtime_error);
}

///////////////////////////////////////////////////


TEST_F(metafolder_tests, store_png_before_create_throws)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);

        romi::Image image;
        std::string filename("file1.png");
        std::string observation("observe");

        // Act
        // Assert
        ASSERT_THROW(meta_folder.try_store_png(filename, image, observation),
                     std::runtime_error);

}

TEST_F(metafolder_tests, store_png_no_extension_creates_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(expected));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("file1");
        std::string filename_png("file1.png");
        std::string observation("observe");

        // Act
        meta_folder.try_store_png(filename, image, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_png.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_png_wrong_extension_changes_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .WillOnce(Return(expected));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("file1.xxx");
        std::string filename_png("file1.png");
        std::string observation("observe");

        // Act
        meta_folder.try_store_png(filename, image, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_png.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}


TEST_F(metafolder_tests, store_png_same_file_rewrites_metadata)
{
        // Arrange
        const int number_files(2);
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .Times(number_files)
                .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                .WillOnce(Return(expected));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("file1.png");
        std::string observation("observe");

        // Act
        meta_folder.try_store_png(filename, image, observation);
        meta_folder.try_store_png(filename, image, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_png_write_error_does_not_write_metadata)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("invalid*/*86.png");
        std::string observation("observe");

        // Act
        ASSERT_THROW(meta_folder.try_store_png(filename, image, observation),
                     std::runtime_error);

        // Assert
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        ASSERT_THROW(metaDataJson.at(filename.c_str()), nlohmann::json::exception);
}

TEST_F(metafolder_tests, store_png_creates_files_and_correct_meta_data)
{
        // Arrange
        const int number_files(3);
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .Times(number_files)
                .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                .WillOnce(Return(std::string("10:25:13-25/01/2020")))
                .WillOnce(Return(std::string("10:25:24-25/01/2020")));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename1("file1.png");
        std::string filename2("file2.png");
        std::string filename3("file3.png");
        std::string observation("observe");

        // Act
        meta_folder.try_store_png(filename1, image, observation);
        meta_folder.try_store_png(filename2, image, observation);
        meta_folder.try_store_png(filename3, image, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);

        // Assert
        ASSERT_TRUE(metaDataJson.contains(filename1));
        ASSERT_TRUE(metaDataJson.contains(filename2));
        ASSERT_TRUE(metaDataJson.contains(filename3));
        ASSERT_TRUE(fs::exists(session_path_ / filename1));
        ASSERT_TRUE(fs::exists(session_path_ / filename2));
        ASSERT_TRUE(fs::exists(session_path_ / filename3));
}
TEST_F(metafolder_tests, store_png_empty_image_throws)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image;
        std::string filename1("file1.png");
        std::string observation("observe");

        // Act
        // Assert
        ASSERT_THROW(meta_folder.try_store_png(filename1, image, observation),
                     std::runtime_error);
}

///////////////////////////////////////////////////

TEST_F(metafolder_tests, store_svg_no_extension_creates_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(expected));

        std::string svg_body("body");
        std::string filename("file1");
        std::string filename_svg("file1.svg");
        std::string observation("observe");

        // Act
        meta_folder.try_store_svg(filename, svg_body, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_svg.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_svg_wrong_extension_changes_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(expected));

        std::string svg_body("body");
        std::string filename("file1.xxx");
        std::string filename_svg("file1.svg");
        std::string observation("observe");

        // Act
        meta_folder.try_store_svg(filename, svg_body, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_svg.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}


TEST_F(metafolder_tests, store_svg_same_file_rewrites_metadata)
{
        // Arrange
        const int number_files(2);
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .Times(number_files)
                        .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                        .WillOnce(Return(expected));

        std::string filename("file1.svg");
        std::string observation("observe");
        std::string svg_body("body");

        // Act
        meta_folder.try_store_svg(filename, svg_body, observation);
        meta_folder.try_store_svg(filename, svg_body, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_svg_write_error_does_not_write_metadata)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("invalid*/*86.svg");
        std::string observation("observe");
        std::string svg_body("body");

        // Act
        ASSERT_THROW(meta_folder.try_store_svg(filename, svg_body, observation), std::runtime_error);

        // Assert
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        ASSERT_THROW(metaDataJson.at(filename.c_str()), nlohmann::json::exception);
}

TEST_F(metafolder_tests, store_svg_creates_files_and_correct_meta_data)
{
        // Arrange
        const int number_files(3);
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .Times(number_files)
                .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                .WillOnce(Return(std::string("10:25:13-25/01/2020")))
                .WillOnce(Return(std::string("10:25:24-25/01/2020")));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename1("file1.svg");
        std::string filename2("file2.svg");
        std::string filename3("file3.svg");
        std::string observation("observe");
        std::string svg_body("body");

        // Act
        meta_folder.try_store_svg(filename1, svg_body, observation);
        meta_folder.try_store_svg(filename2, svg_body, observation);
        meta_folder.try_store_svg(filename3, svg_body, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);

        // Assert
        ASSERT_TRUE(metaDataJson.contains(filename1));
        ASSERT_TRUE(metaDataJson.contains(filename2));
        ASSERT_TRUE(metaDataJson.contains(filename3));
        ASSERT_TRUE(fs::exists(session_path_ / filename1));
        ASSERT_TRUE(fs::exists(session_path_ / filename2));
        ASSERT_TRUE(fs::exists(session_path_ / filename3));
}

TEST_F(metafolder_tests, store_svg_empty_image_throws)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity
                = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image;
        std::string filename1("file1.svg");
        std::string observation("observe");
        std::string svg_body("");

        // Act
        // Assert
        ASSERT_THROW(meta_folder.try_store_svg(filename1, svg_body, observation),
                     std::runtime_error);
}

///////////////////////////////////////////////////

TEST_F(metafolder_tests, store_txt_no_extension_creates_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(expected));

        std::string txt_body("body");
        std::string filename("file1");
        std::string filename_txt("file1.txt");
        std::string observation("observe");

        // Act
        meta_folder.try_store_txt(filename, txt_body, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_txt.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_txt_wrong_extension_changes_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(expected));

        std::string txt_body("body");
        std::string filename("file1.xxx");
        std::string filename_txt("file1.txt");
        std::string observation("observe");

        // Act
        meta_folder.try_store_txt(filename, txt_body, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_txt.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}


TEST_F(metafolder_tests, store_txt_same_file_rewrites_metadata)
{
        // Arrange
        const int number_files(2);
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .Times(number_files)
                .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                .WillOnce(Return(expected));

        std::string filename("file1.txt");
        std::string observation("observe");
        std::string txt_body("body");

        // Act
        meta_folder.try_store_txt(filename, txt_body, observation);
        meta_folder.try_store_txt(filename, txt_body, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_txt_write_error_does_not_write_metadata)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("invalid*/*86.txt");
        std::string observation("observe");
        std::string txt_body("body");

        // Act
        ASSERT_THROW(meta_folder.try_store_txt(filename, txt_body, observation),
                     std::runtime_error);

        // Assert
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        ASSERT_THROW(metaDataJson.at(filename.c_str()), nlohmann::json::exception);
}

TEST_F(metafolder_tests, store_txt_creates_files_and_correct_meta_data)
{
        // Arrange
        const int number_files(3);
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .Times(number_files)
                .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                .WillOnce(Return(std::string("10:25:13-25/01/2020")))
                .WillOnce(Return(std::string("10:25:24-25/01/2020")));
        
        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename1("file1.txt");
        std::string filename2("file2.txt");
        std::string filename3("file3.txt");
        std::string observation("observe");
        std::string txt_body("body");

        // Act
        meta_folder.try_store_txt(filename1, txt_body, observation);
        meta_folder.try_store_txt(filename2, txt_body, observation);
        meta_folder.try_store_txt(filename3, txt_body, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);

        // Assert
        ASSERT_TRUE(metaDataJson.contains(filename1));
        ASSERT_TRUE(metaDataJson.contains(filename2));
        ASSERT_TRUE(metaDataJson.contains(filename3));
        ASSERT_TRUE(fs::exists(session_path_ / filename1));
        ASSERT_TRUE(fs::exists(session_path_ / filename2));
        ASSERT_TRUE(fs::exists(session_path_ / filename3));
}

TEST_F(metafolder_tests, store_txt_empty_string_doesnt_throw)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);
        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .WillOnce(Return(std::string("10:25:24-25/01/2020")));

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image;
        std::string filename1("file1.txt");
        std::string observation("observe");
        std::string txt_body;

        // Act
        // Assert
        try {
                meta_folder.try_store_txt(filename1, txt_body, observation);
        } catch (const std::exception& e) {
                FAIL() << "Didn't expected an exception";
        }
}

///////////////////////////////////////////////////

TEST_F(metafolder_tests, store_path_no_extension_creates_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(expected));

        std::string filename("file1");
        std::string filename_txt("file1.path");
        std::string observation("observe");

        double data(0.0);
        romi::Path testPath;
        for (int i = 0; i < 10; i++) {
                data+=0.1;
                testPath.emplace_back(romi::v3(data, data, 0.0));
        }

        // Act
        meta_folder.try_store_path(filename, testPath, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_txt.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}

TEST_F(metafolder_tests, store_path_wrong_extension_changes_extension)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .WillOnce(Return(expected));

        std::string filename("file1.xxx");
        std::string filename_txt("file1.path");
        std::string observation("observe");

        double data(0.0);
        romi::Path testPath;
        for (int i = 0; i < 10; i++) {
                data+=0.1;
                testPath.emplace_back(romi::v3(data, data, 0.0));
        }

        // Act
        meta_folder.try_store_path(filename, testPath, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename_txt.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson[JsonFieldNames::date_time], expected);
}


TEST_F(metafolder_tests, store_path_same_file_rewrites_metadata)
{
        // Arrange
        const int number_files(2);
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .Times(number_files)
                        .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                        .WillOnce(Return(expected));

        std::string filename("file1.path");
        std::string observation("observe");
        std::string path_body("body");

        double data(0.0);
        romi::Path testPath;
        for (int i = 0; i < 10; i++) {
                data+=0.1;
                testPath.emplace_back(romi::v3(data, data, 0.0));
        }

        // Act
        meta_folder.try_store_path(filename, testPath, observation);
        meta_folder.try_store_path(filename, testPath, observation);
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        auto fileJson = metaDataJson[filename.c_str()];

        // Assert
        ASSERT_TRUE(fileJson.contains(JsonFieldNames::date_time));
        ASSERT_EQ(fileJson.at(JsonFieldNames::date_time), expected);
}

TEST_F(metafolder_tests, store_path_write_error_does_not_write_metadata)
{
        // Arrange
        std::string expected("10:25:13-25/01/2020");
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename("invalid*/*86.txt");
        std::string observation("observe");
        std::string path_body("body");

        romi::Path testPath;

        // Act
        ASSERT_THROW(meta_folder.try_store_path(filename, testPath, observation), std::runtime_error);

        // Assert
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        ASSERT_THROW(metaDataJson.at(filename.c_str()), nlohmann::json::exception);
}

TEST_F(metafolder_tests, store_path_creates_files_and_correct_meta_data)
{
        // Arrange
        const int number_files(3);
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_, softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .Times(number_files)
                .WillOnce(Return(std::string("10:25:02-25/01/2020")))
                .WillOnce(Return(std::string("10:25:13-25/01/2020")))
                .WillOnce(Return(std::string("10:25:24-25/01/2020")));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);
        std::string filename1("file1.path");
        std::string filename2("file2.path");
        std::string filename3("file3.path");
        std::string observation("observe");
        std::string path_body("body");

        double data(0.0);
        romi::Path testPath;
        for (int i = 0; i < 10; i++) {
                data+=0.1;
                testPath.emplace_back(romi::v3(data, data, 0.0));
        }

        // Act
        meta_folder.try_store_path(filename1, testPath, observation);
        meta_folder.try_store_path(filename2, testPath, observation);
        meta_folder.try_store_path(filename3, testPath, observation);

        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);

        // Assert
        ASSERT_TRUE(metaDataJson.contains(filename1));
        ASSERT_TRUE(metaDataJson.contains(filename2));
        ASSERT_TRUE(metaDataJson.contains(filename3));
        ASSERT_TRUE(fs::exists(session_path_ / filename1));
        ASSERT_TRUE(fs::exists(session_path_ / filename2));
        ASSERT_TRUE(fs::exists(session_path_ / filename3));
}

TEST_F(metafolder_tests, store_empty_path_doesnt_throw)
{
        // Arrange
        EXPECT_CALL(*mockClock_, datetime_compact_string)
                .Times(AnyNumber())
                .WillRepeatedly(Return(std::string("10:25:02-25/01/2020")));
        
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        romi::Image image;
        std::string filename1("file1.txt");
        std::string observation("observe");

        romi::Path testPath;

        // Act
        // Assert
        try {
                meta_folder.try_store_path(filename1, testPath, observation);
        } catch (const std::exception& e) {
                FAIL() << "Didn't expected an exception";
        }                
}

const int max_files = 20;

int Write_multiple_thread_png(romi::MetaFolder& MetaFolder, romi::Image& image)
{
        int file_index = 0;
        std::string filename("file");
        std::string observation_id("png_observer");
        for (file_index = 0; file_index < 20; file_index++) {
                MetaFolder.try_store_png((filename + to_string(file_index)),
                                         image, observation_id);
        }
        return 0;
}

int Write_multiple_thread_jpg(romi::MetaFolder& MetaFolder, romi::Image& image)
{
        int file_index = 0;
        std::string filename("file");
        std::string observation_id("jpg_observer");
        for (file_index = 0; file_index < 20; file_index++) {
                MetaFolder.try_store_jpg((filename + to_string(file_index)),
                                         image, observation_id);
        }
        return 0;
}

TEST_F(metafolder_tests, store_multiple_threads_does_not_corrupt_metafolder)
{
        // Arrange
        SetDeviceIDDataExpectations(devicetype_, devicID_, 1);
        SetSoftwareVersionDDataExpectations(versionCurrent_, versionAlternate_);

        auto mockLocationProvider_ = std::make_shared<romi::GpsLocationProvider>(mockGps_);
        auto roverIdentity = std::make_shared<romi::RoverIdentityProvider>(deviceData_,
                                                                           softwareVersion_);
        romi::MetaFolder meta_folder(roverIdentity, mockLocationProvider_, session_path_);
        fs::path meta_data_filename = session_path_ / romi::MetaFolder::meta_data_filename_;

        EXPECT_CALL(*mockClock_, datetime_compact_string)
                        .Times(AnyNumber())
                        .WillRepeatedly(Return(std::string("10:25:02-25/01/2020")));

        romi::Image image(romi::Image::RGB, red_test_image, 4, 4);

        auto png_future = std::thread(Write_multiple_thread_png, std::ref(meta_folder),
                                      std::ref(image));
        auto jpg_future = std::thread(Write_multiple_thread_jpg, std::ref(meta_folder),
                                      std::ref(image));

        // Act
        jpg_future.join();
        png_future.join();

        // Assert
        std::ifstream ifs(meta_data_filename);
        nlohmann::json metaDataJson = nlohmann::json::parse(ifs);
        ASSERT_NO_THROW(auto file0Json = metaDataJson["file0.jpg"]);
        ASSERT_NO_THROW(auto file0Json = metaDataJson["file19.jpg"]);
        ASSERT_NO_THROW(auto file0Json = metaDataJson["file0.png"]);
        ASSERT_NO_THROW(auto file0Json = metaDataJson["file19.png"]);
}


