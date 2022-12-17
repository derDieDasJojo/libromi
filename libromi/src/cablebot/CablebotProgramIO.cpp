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

#include "util/Logger.h"
#include "cablebot/CablebotProgramList.h"
#include "cablebot/CablebotProgram.h"
#include "cablebot/CablebotProgramIO.h"

namespace romi {

        CablebotProgramIO::CablebotProgramIO(std::shared_ptr<IConfigManager>& config,
                                             const std::string& section)
                : config_(config),
                  section_(section)
        {
        }
        
        std::unique_ptr<ICablebotProgramList> CablebotProgramIO::load()
        {
                nlohmann::json json = config_->get_section(section_);
                return from_json(json);;
        }

        std::shared_ptr<ICablebotProgram>
        CablebotProgramIO::load_program(nlohmann::json json)
        {
                std::shared_ptr<ICablebotProgram> program;

                uint32_t id = json[kID];
                std::string name = json[kName];
                uint8_t hour = json[kHour];
                uint8_t minute = json[kMinute];
                double start = json[kStart];
                double length = json[kLength];
                double interval = json[kInterval];
                double tilt = json[kTilt];
                bool enabled = json[kEnabled];

                r_info("CablebotProgramIO: program[id=%d, name='%s']: at %02d:%02d, "
                       "from %.1f m, length %.1f m, every %.2f m (%s)",
                       (int) id, name.c_str(),
                       (int) hour, (int) minute,
                       start, length, interval,
                       enabled? "enabled" : "disabled");
                
                program = std::make_shared<CablebotProgram>(id, name,
                                                            hour, minute,
                                                            start, length, interval,
                                                            tilt, enabled);
                return program;
        }
        
        void CablebotProgramIO::store(ICablebotProgramList& list)
        {
                nlohmann::json json = to_json(list);
                config_->set_section(section_, json);
        }

        nlohmann::json
        CablebotProgramIO::convert(std::shared_ptr<ICablebotProgram>& program)
        {
                return nlohmann::json::object({
                                {kID, program->id()},
                                {kName, program->name()},
                                {kHour, program->hour()},
                                {kMinute, program->minute()},
                                {kStart, program->start()},
                                {kLength, program->length()},
                                {kInterval, program->interval()},
                                {kTilt, program->tilt()},
                                {kEnabled, program->is_enabled()}});
        }

        nlohmann::json CablebotProgramIO::to_json(ICablebotProgramList& list)
        {
                nlohmann::json json = nlohmann::json::array();
                for (size_t i = 0; i < list.count(); i++) {
                        auto program = list.get(i);
                        nlohmann::json json_program = convert(program);
                        json.push_back(json_program);
                }
                return json;
        }

        std::unique_ptr<ICablebotProgramList>
        CablebotProgramIO::from_json(nlohmann::json& json)
        {
                std::unique_ptr<ICablebotProgramList> list;

                if (!json.is_array()) {
                        r_err("CablebotProgramIO::load: expected an array");
                        throw std::runtime_error("CablebotProgramIO::load: "
                                                 "expected an array");
                }
                
                list = std::make_unique<CablebotProgramList>();
                for (size_t i = 0; i < json.size(); i++) {
                        nlohmann::json json_program = json[i];
                        std::shared_ptr<ICablebotProgram> program;
                        program = load_program(json_program);
                        list->insert(program);
                }
                return list;
        }
}
