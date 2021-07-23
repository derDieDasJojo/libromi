#ifndef ROMI_ROVER_BUILD_AND_TEST_SCRIPT_H
#define ROMI_ROVER_BUILD_AND_TEST_SCRIPT_H

#include <vector>
#include "Action.h"

namespace romi {
    struct Script {
        std::string id;
        std::string title;
        std::vector<Action> actions;

        Script(const char *id_, const char *title_) : id(id_), title(title_), actions() {
        }

        void append(Action action) {
            actions.push_back(action);
        }
    };
}
#endif //ROMI_ROVER_BUILD_AND_TEST_SCRIPT_H
