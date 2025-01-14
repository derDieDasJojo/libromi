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
#include "util/StringUtils.h"
#include "ui/CrystalDisplay.h"

namespace romi {
        
        constexpr size_t screen_width = 32;
        
        CrystalDisplay::CrystalDisplay(std::unique_ptr<romiserial::IRomiSerialClient>& romi_serial)
                : _serial()
        {
                _serial = std::move(romi_serial);
        }

        bool CrystalDisplay::show(size_t line, const std::string& display_string)
        {
                bool success = false;
                if (line < count_lines() && display_string.length() <= screen_width) {
                        char buffer[64];
                        nlohmann::json response;
                        
                        StringUtils::rprintf(buffer, 64, "S[%d,\"%s\"]", line, display_string.c_str());
                        
                        _serial->send(buffer, response);

                        return response[0] == 0;
                        
                } else {
                        r_warn("CrystalDisplay::show: Invalid line number or string "
                                "(line=%d, s='%s')", line, display_string.c_str());
                }
                return success;
        }
        
        bool CrystalDisplay::clear(size_t line)
        {
                bool success = false;
                if (line < count_lines()) {
                        char buffer[64];
                        nlohmann::json response;
                        
                        StringUtils::rprintf(buffer, 64, "C[%d]", line);
                        
                        _serial->send(buffer, response);

                        return response[0] == 0;
                        
                } else {
                        r_warn("CrystalDisplay::show: Invalid line number "
                                "(line=%d)", line);
                }
                return success;
        }
}
