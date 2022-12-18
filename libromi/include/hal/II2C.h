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
#ifndef __ROMI_II2C_H
#define __ROMI_II2C_H

#include <stdint.h>

namespace romi {
        
        class II2C
        {
        public:
                virtual ~II2C() = default;

                virtual int32_t get(uint8_t register_) = 0;             // throws on error
                virtual void set(uint8_t register_, int32_t value) = 0; // throws on error
        };
}

#endif // __ROMI_II2C_H
