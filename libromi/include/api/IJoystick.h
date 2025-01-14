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
#ifndef _ROMI_I_JOYSTICK_H_
#define _ROMI_I_JOYSTICK_H_
#include <cstdint>
#include <cstddef>

namespace romi {

        struct JoystickEvent
        {
                enum JoystickEventType { kNone, kButton, kAxis };
                
                JoystickEventType type;
                uint8_t number;
        };
        
        class IJoystick
        {
        public:
                virtual ~IJoystick() = default;

                virtual JoystickEvent& get_next_event() = 0;
                virtual size_t get_num_axes() = 0;
                virtual double get_axis(size_t i) = 0;
                virtual size_t get_num_buttons() = 0;
                virtual bool is_button_pressed(size_t i) = 0;
        };
}

#endif // _ROMI_I_JOYSTICK_H_
