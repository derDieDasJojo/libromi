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
#include <JsonCpp.h>

namespace romi {

        class ScriptList : public IScriptList
        {
        private:
            std::vector<Script> scripts_;
            JsonCpp json_scripts_;
        protected:
                void load_scripts(const std::string& path);
                void convert_scripts(JsonCpp& scripts);
                void convert_script(JsonCpp& script);
                void convert_script_actions(Script& script, JsonCpp& json_script);
                void convert_action(Script& script, JsonCpp& action);
                void convert_move(Script& script, JsonCpp& action);
                void assure_move_params(double distance, double speed);
                void convert_hoe(Script& script);
                void convert_homing(Script& script);
                void convert_start_recording(Script& script);
                void convert_stop_recording(Script& script);
                
        public:

                explicit ScriptList(const std::string& path);
                explicit ScriptList(JsonCpp& json);
                ~ScriptList() override = default;

                Script& operator[](size_t) override;
                bool empty() const override;
                size_t size() const override;
                std::string json_scripts() const override;
        };
}

#endif // _ROMI_SCRIPT_LIST_H
