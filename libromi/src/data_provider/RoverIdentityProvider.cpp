#include "data_provider/JsonFieldNames.h"
#include "data_provider/RoverIdentityProvider.h"
#include "json.hpp"

namespace romi {

    RoverIdentityProvider::RoverIdentityProvider(IRomiDeviceData &deviceData,
                                                 ISoftwareVersion &softwareVersion)
            : romi_device_type_(),
              romi_hardware_id_(),
              current_software_version_(),
              alternate_software_version_(),
              device_data_(deviceData),
              software_version_(softwareVersion)
    {
            current_software_version_ = software_version_.SoftwareVersionCurrent();
            alternate_software_version_ = software_version_.SoftwareVersionAlternate();
            romi_device_type_ = device_data_.RomiDeviceType();
            romi_hardware_id_ = device_data_.RomiDeviceHardwareId();
    }

    nlohmann::json RoverIdentityProvider::identity() {

        nlohmann::json identity_data_object{
                {JsonFieldNames::romi_device_type.data(),  romi_device_type_},
                {JsonFieldNames::romi_hardware_id.data(), romi_hardware_id_},
                {JsonFieldNames::software_version_current.data(), current_software_version_},
                {JsonFieldNames::software_version_alternate.data(), alternate_software_version_}};

        return identity_data_object;
    }
}
