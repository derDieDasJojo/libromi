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
#ifndef _ROMI_SCRIPT_LIST_H
#define _ROMI_SCRIPT_LIST_H

#include "IScriptList.h"
#include "Script.h"
#include <string>
#include <vector>
#include <json.hpp>

namespace romi {

        class ScriptList : public IScriptList
        {
        private:
            std::vector<Script> scripts_;
            nlohmann::json json_scripts_;
        protected:
                void load_scripts(const std::string& path);
                void convert_scripts(nlohmann::json& scripts);
                void convert_script(nlohmann::json& script);
                void convert_script_actions(Script& script, nlohmann::json& json_script);
                void convert_action(Script& script, nlohmann::json& action);
                void convert_move(Script& script, nlohmann::json& action);
                void assure_move_params(double distance, double speed);
                void convert_hoe(Script& script);
                void convert_homing(Script& script);
                void convert_start_recording(Script& script);
                void convert_stop_recording(Script& script);
                
        public:

                explicit ScriptList(const std::string& path);
                explicit ScriptList(nlohmann::json& json);
                ~ScriptList() override = default;

                const Script& operator[](size_t) const override;
                bool empty() const override;
                size_t size() const override;
                std::string json_scripts() const override;
                long get_id_index(std::string& id ) const override;
        };
}

#endif // _ROMI_SCRIPT_LIST_H
