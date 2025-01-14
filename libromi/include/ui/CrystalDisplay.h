/*
  romi-rover

  Copyright (C) 2019-2020 Sony Computer Science Laboratories
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

#ifndef __ROMI_CRYSTAL_DISPLAY_H
#define __ROMI_CRYSTAL_DISPLAY_H

#include <memory>
#include <IRomiSerialClient.h>
#include "api/IDisplay.h"

namespace romi {
        
        class CrystalDisplay : public IDisplay
        {
        public:
                static constexpr const char *ClassName = "crystal-display";
                static constexpr size_t lines = 2;

        protected:
                std::unique_ptr<romiserial::IRomiSerialClient> _serial;
                        
        public:
                explicit CrystalDisplay(std::unique_ptr<romiserial::IRomiSerialClient>& romi_serial);
                ~CrystalDisplay() override = default;

                bool show(size_t line, const std::string& display_string) override;
                bool clear(size_t line) override;
                
                size_t count_lines() override{
                        return lines;
                }
        };
}
#endif // __ROMI_CRYSTAL_DISPLAY_H
