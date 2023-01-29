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
#ifndef __ROMI_FAKEI2C_H
#define __ROMI_FAKEI2C_H

#include <string>
#include "hal/II2C.h"

namespace romi {
        
        class FakeI2C : public II2C
        {
        protected:
                std::string device_;
                int address_;
                int32_t registers_[256];
                
        public:
                FakeI2C(const char *device, int address);
                ~FakeI2C() override = default;
                
                int32_t get(uint8_t reg) override;
                void set(uint8_t reg, int32_t value) override;

        };
}

#endif // __ROMI_FAKEI2C_H
