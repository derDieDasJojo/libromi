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
#ifndef __ROMI_FAKE_CNC_H
#define __ROMI_FAKE_CNC_H

#include "api/ICNC.h"

namespace romi {
        
        class FakeCNC : public ICNC
        {
        public:
                static constexpr const char *ClassName = "fake-cnc";

        protected:
                CNCRange _range;
                
        public:
                explicit FakeCNC(nlohmann::json& range) : _range(range){

                }
                
                ~FakeCNC() override = default;
                
                bool get_range(CNCRange &range) override {
                        range = _range;
                        return true;
                }
                
                bool moveto(__attribute__((unused))double x, __attribute__((unused))double y, __attribute__((unused))double z, __attribute__((unused))double relative_speed) override {
                        return true;
                }
                
                bool spindle(__attribute__((unused))double speed) override {
                        return true;
                }
                
                bool travel(__attribute__((unused))Path &path, __attribute__((unused))double relative_speed) override {
                        return true;
                }
                
                bool homing() override{
                        return true;
                }
                
                bool pause_activity() override {
                        return true;
                }
                
                bool continue_activity() override {
                        return true;
                }
                
                bool reset_activity() override {
                        return true;
                }
                
                bool power_up() override {
                        return true;
                }
                
                bool power_down() override {
                        return true;
                }

                bool stand_by() override {
                        return true;
                }

                bool wake_up() override {
                        return true;
                }
        };
}

#endif // __ROMI_FAKE_CNC_H
