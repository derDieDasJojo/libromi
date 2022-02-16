#include "gmock/gmock.h"
#include "data_provider/ILocationProvider.h"
#include "json.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class MockLocationProvider : public romi::ILocationProvider
{
public:
        MOCK_METHOD(nlohmann::json, location, (), (override));
        MOCK_METHOD(bool, update_location_estimate, (), (override));
        MOCK_METHOD(romi::v3, coordinates, (), (override));

};
#pragma GCC diagnostic pop
