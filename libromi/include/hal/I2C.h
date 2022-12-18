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
#ifndef __ROMI_I2C_H
#define __ROMI_I2C_H

#include <string>
#include <rcom/ILinux.h>
#include "hal/II2C.h"

namespace romi {
        
        class I2C : public II2C
        {
        protected:
                static const uint8_t kBufferLength = 5;
                static const uint8_t kCheckSumIndex = 4;
                
                rcom::ILinux& linux_;
                std::string device_;
                int address_;
                int fd_;
                uint8_t buffer_[kBufferLength];
                
        public:
                I2C(rcom::ILinux& linux, const char *device, int address);
                ~I2C() override;
                
                int32_t get(uint8_t register_) override;
                void set(uint8_t register_, int32_t value) override;

        protected:
                uint8_t compute_checksum();
                void assure_checksum(uint8_t sum);
                void set_checksum(uint8_t sum);
                void to_buffer(int32_t value);
                int32_t from_buffer();
                void send(uint8_t register_);
                void receive(uint8_t register_);
                void sleep();
        };
}

#endif // __ROMI_I2C_H
