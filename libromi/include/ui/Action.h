#ifndef ROMI_ROVER_BUILD_AND_TEST_ACTION_H
#define ROMI_ROVER_BUILD_AND_TEST_ACTION_H

namespace romi {

    struct Action {
        // FIXME: This enum is specific to the weeding
        // rover. And this is a generic header for all
        // devices.
        enum ActionType {
            Move, Hoe, Homing, StartRecording, StopRecording
        };

        static constexpr const char *kMoveCommand = "move";
        static constexpr const char *kHoeCommand = "hoe";
        static constexpr const char *kHomingCommand = "homing";
        static constexpr const char *kStartRecordingCommand = "start-recording";
        static constexpr const char *kStopRecordingCommand = "stop-recording";

        ActionType type;
        double params[2];

        Action(ActionType atype) : type(atype) {
            params[0] = 0.0;
            params[1] = 0.0;
        }

        Action(ActionType atype, double param1, double param2) : type(atype) {
            params[0] = param1;
            params[1] = param2;
        }

    };
}
#endif //ROMI_ROVER_BUILD_AND_TEST_ACTION_H
