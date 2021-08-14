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
#ifndef _ROMI_ISCRIPT_LIST_H
#define _ROMI_ISCRIPT_LIST_H

#include <string>
#include "Script.h"

namespace romi {

        class IScriptList
        {
            public:
                virtual ~IScriptList() = default;

                virtual const Script& operator[](size_t) const = 0;
                virtual bool empty() const = 0;
                virtual size_t size() const = 0;
                virtual std::string json_scripts() const = 0;
                virtual long get_id_index(std::string& id ) const = 0;
        };
}

#endif // _IROMI_SCRIPT_LIST_H
