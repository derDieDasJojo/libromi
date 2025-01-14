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


#include "util/Logger.h"
#include "ui/UIEventMapper.h"

namespace romi {

        int UIEventMapper::map_axis(IJoystick& joystick, JoystickEvent& event)
        {
                int retval = 0;
                        
                if (event.number == axis_direction) {
                        retval = event_direction;
                                        
                } else if (event.number == axis_backward_speed) {
                        retval = event_backward_speed;
                                        
                } else if (event.number == axis_forward_speed) {
                        retval = event_forward_speed;
                        
                } else if (event.number == axis_menu_navigation) {
                        if (joystick.get_axis(event.number) < 0)
                                retval = event_previous_menu;
                        else if (joystick.get_axis(event.number) > 0)
                                retval = event_next_menu;
                } 
 
                return retval;
        }

        int UIEventMapper::map_button(IJoystick& joystick, JoystickEvent& event)
        {
                int retval = 0;
                        
                if (event.number == button_forward_mode) {
                        if (joystick.is_button_pressed(button_forward_mode)) {
                                retval = event_forward_start;
                        } else {
                                retval = event_forward_stop;
                        }
                                
                } else if (event.number == button_backward_mode) {
                        if (joystick.is_button_pressed(button_backward_mode)) {
                                retval = event_backward_start;
                        } else {
                                retval = event_backward_stop;
                        }
                                
                } else if (event.number == button_spin_mode) {
                        if (joystick.is_button_pressed(button_spin_mode)) {
                                retval = event_spinning_start;
                        } else {
                                retval = event_spinning_stop;
                        }
                                
                } else if (event.number == button_accurate_forward) {
                        if (joystick.is_button_pressed(button_accurate_forward)) {
                                retval = event_accurate_forward_start;
                        } else {
                                retval = event_accurate_forward_stop;
                        }
                                
                } else if (event.number == button_accurate_backward) {
                        if (joystick.is_button_pressed(button_accurate_backward)) {
                                retval = event_accurate_backward_start;
                        } else {
                                retval = event_accurate_backward_stop;
                        }
                } else if (event.number == button_navigation_mode) {
                        if (joystick.is_button_pressed(button_navigation_mode)) {
                                retval = event_navigation_mode_pressed;
                        } else {
                                retval = event_navigation_mode_released;
                        }
                } else if (event.number == button_menu_mode) {
                        r_debug("button_menu_mode");
                        if (joystick.is_button_pressed(button_menu_mode)) {
                                r_debug("event_menu_mode_pressed");
                                retval = event_menu_mode_pressed;
                        } else {
                                r_debug("event_menu_mode_released");
                                retval = event_menu_mode_released;
                        }
                } else if (event.number == button_select) {
                        if (joystick.is_button_pressed(button_select)) {
                                retval = event_select;
                        }
                } else if (event.number == button_stop) {
                        if (joystick.is_button_pressed(button_stop)) {
                                retval = event_stop;
                        }
                } 
                        
                return retval;
        }
                
        int UIEventMapper::map(IJoystick& joystick, JoystickEvent& event)
        {
                int retval = 0;
                        
                if (event.type == JoystickEvent::kAxis) {
                        retval = map_axis(joystick, event);
                                
                } else if (event.type == JoystickEvent::kButton) {
                        retval = map_button(joystick, event);
                }
                        
                return retval;
        }
}
