#ifndef ROMI_ROVER_BUILD_AND_TEST_REMOTEFUNCTIONCALLNAMES_H
#define ROMI_ROVER_BUILD_AND_TEST_REMOTEFUNCTIONCALLNAMES_H

namespace romi
{
    class RemoteMessgeTypes
    {
        public:
            static constexpr const char* message_type_request           = "request";
            static constexpr const char* request_type_list              = "list";
            static constexpr const char* request_type_execute           = "execute";
            static constexpr const char* execute_type_id                = "id";
            static constexpr const char* execute_type_start_navigation  = "start_navigation";
            static constexpr const char* execute_type_stop_navigation   = "stop_navigation";
    };

}

#endif //ROMI_ROVER_BUILD_AND_TEST_REMOTEFUNCTIONCALLNAMES_H
