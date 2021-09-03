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

#include "ui/RemoteStateInputDevice.h"
#include "rover/EventsAndStates.h"

namespace romi {
        
        RemoteStateInputDevice::RemoteStateInputDevice()  : next_script_(0), nextevent_(0){
        }

        int RemoteStateInputDevice::get_next_script_index(){
            int index = next_script_;
            next_script_ = 0;
            return index;
        }

        void RemoteStateInputDevice::set_next_script_index(long index){
            next_script_ = static_cast<int>(index);
        }

        int RemoteStateInputDevice::get_next_event(){
            int event = nextevent_;
            nextevent_ = 0;
            return event;
        }

        void RemoteStateInputDevice::set_next_event(int event){
            if ((event >=  event_first_event) && (event < event_last_event))
                nextevent_ = event;
        }
}
