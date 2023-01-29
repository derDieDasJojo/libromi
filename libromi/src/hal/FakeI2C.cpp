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
#include "util/ClockAccessor.h"
#include "hal/FakeI2C.h"

namespace romi {

        FakeI2C::FakeI2C(rcom::ILinux& linux, const char *device, int address)
                : device_(device),
                  address_(address)
        {
                for (int i = 0; i < 256; i++)
                        registers_[i] = 0;
        }
                
        int32_t FakeI2C::get(uint8_t r)
        {
                return registers_[r];
        }
        
        void FakeI2C::set(uint8_t r, int32_t value)
        {
                registers_[r] = value;
        }
}
