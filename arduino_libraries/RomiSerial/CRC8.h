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
#ifndef __CRC8_H
#define __CRC8_H

#include <stdint.h>
#include <string.h>

class CRC8
{
protected:
        uint8_t _crc;
        uint8_t _table[256];
        
public:
        
        CRC8(uint8_t poly = 0x07) : _crc(0){
                init_table(poly);
        }
        
        void init_table(uint8_t poly = 0x07) {
                uint8_t crc;                
                for (uint16_t i = 0; i <= UINT8_MAX; i++) {
                        crc = (uint8_t)i;
                        for (uint8_t j = 0; j < 8; j++)
                            crc = (uint8_t)(crc << 1) ^ (uint8_t )((crc & 0x80) ? poly : 0);
                        _table[i] = crc;
                }
        }
        
        void start(uint8_t start_value = 0) {
                _crc = start_value;
        }

        void update(const char c) {
            update(uint8_t(c));
        }

        void update(uint8_t c) {
                _crc = _table[_crc ^ c];
        }
        
        void update(const char *s, size_t len) {
                while (len--) {
                        update((uint8_t) *s++);
                }
        }
        
        void update(const char *s) {
                update(s, strlen(s));
        }
        
        uint8_t finalize() {
                return _crc;
        }
        
        uint8_t get() {
                return _crc;
        }
        
        uint8_t compute(const char *s, size_t len) {
                start();
                for (size_t i = 0; i < len; i++)
                        update((uint8_t)s[i]);
                return finalize();
        }
};

#endif
