/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#include <stdexcept>
#include <Logger.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "ui/ScriptList.h"

namespace romi {
                
        ScriptList::ScriptList(const std::string& path) : scripts_(), json_scripts_()
        {
                try {
                        load_scripts(path);
                } catch (nlohmann::json::exception& je) {
                        r_err("ScriptList: JSON error while "
                              "loading scripts: %s", je.what());
                        throw std::runtime_error("Invalid script");
                        
                } catch (std::runtime_error& re) {
                        r_err("ScriptList: Error while converting "
                              "scripts: %s", re.what());
                        throw std::runtime_error("Invalid script");
                }
        }
                
        ScriptList::ScriptList(nlohmann::json& json) : scripts_(), json_scripts_()
        {
                try {
                        
                        convert_scripts(json);
                
                } catch (nlohmann::json::exception& je) {
                        r_err("ScriptList: JSON error while "
                              "loading scripts: %s", je.what());
                        throw std::runtime_error("Invalid script");
                        
                } catch (std::runtime_error& re) {
                        r_err("ScriptList: Error while converting "
                              "scripts: %s", re.what());
                        throw std::runtime_error("Invalid script");
                }
        }
        
        void ScriptList::load_scripts(const std::string& path)
        {
            // TBD: REFACTOR ADD JSON LOAD UTILITY FUNCTION or USE FILEUTILS
            std::ifstream jsoninput(path.c_str());
            jsoninput >> json_scripts_;
            convert_scripts(json_scripts_);
        }
        
        void ScriptList::convert_scripts(nlohmann::json& scripts)
        {
                for (size_t index = 0; index < scripts.size(); index++) {
                        nlohmann::json script = scripts.at(index);
                        convert_script(script);
                }                        
        }
        
        void ScriptList::convert_script(nlohmann::json& script)
        {
                std::string id = script["id"];
                std::string title = script["title"];
                
                scripts_.emplace_back(id, title);
                convert_script_actions(scripts_.back(), script);
        }

        void ScriptList::convert_script_actions(Script& script,
                                                nlohmann::json& json_script)
        {
                nlohmann::json actions = json_script.at("actions");
                for (size_t i = 0; i < actions.size(); i++) {
                        nlohmann::json action = actions.at(i);
                        convert_action(script, action);
                }
        }

        void ScriptList::convert_action(Script& script, nlohmann::json& action)
        {
                std::string type = action["action"];
                if (type == Action::kMoveCommand)
                        convert_move(script, action);
                else if (type == Action::kHoeCommand)
                        convert_hoe(script);
                else if (type == Action::kHomingCommand)
                        convert_homing(script);
                else if (type == Action::kStartRecordingCommand)
                        convert_start_recording(script);
                else if (type == Action::kStopRecordingCommand)
                        convert_stop_recording(script);
        }
        
        void ScriptList::convert_move(Script& script, nlohmann::json& action)
        {
                double distance = (double) action["distance"];
                double speed = (double) action["speed"];
                
                assure_move_params(distance, speed);
                script.append(Action(Action::Move, distance, speed));
        }

        void ScriptList::assure_move_params(double distance, double speed)
        {
                if (fabs(distance) > 100.0) {
                        r_err("ScriptList: invalid distance for move: "
                              "%0.2f", distance);
                        throw std::runtime_error("Invalid distance");
                }
                if (fabs(speed) > 1.0) {
                        r_err("ScriptList: invalid speed for move: "
                              "%0.2f", speed);
                        throw std::runtime_error("Invalid speed");
                }
        }
        
        void ScriptList::convert_hoe(Script& script)
        {
                script.append(Action(Action::Hoe));
        }
        
        void ScriptList::convert_homing(Script& script)
        {
                script.append(Action(Action::Homing));
        }
        
        void ScriptList::convert_start_recording(Script& script)
        {
                script.append(Action(Action::StartRecording));
        }
        
        void ScriptList::convert_stop_recording(Script& script)
        {
                script.append(Action(Action::StopRecording));
        }

        std::string ScriptList::json_scripts() const {
            return json_scripts_.dump(4);
        }

        const Script &ScriptList::operator[](size_t index) const {
            return scripts_[index];
        }

        bool ScriptList::empty() const {
                return scripts_.empty();
        }

        size_t ScriptList::size() const {
                return scripts_.size();
        }

    long ScriptList::get_id_index(std::string &id) const {

       auto itr = std::find_if(scripts_.begin(), scripts_.end(), [&id](const auto& element) { return element.id == id; });
       long index = -1;
        if (itr != scripts_.cend()) {
            index = std::distance(scripts_.begin(), itr);
        }
        return index;
    }
}
