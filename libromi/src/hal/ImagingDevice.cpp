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

#include "hal/ImagingDevice.h"

namespace romi {
        
        ImagingDevice::ImagingDevice(std::shared_ptr<ICamera>& camera,
                                     std::shared_ptr<IDisplacementDevice>& carrier)
                : camera_(camera),
                  carrier_(carrier)
        {
        }

        bool ImagingDevice::pause_activity()
        {
                return carrier_->pause_activity();
        }
        
        bool ImagingDevice::continue_activity()
        {
                return carrier_->continue_activity();
        }
        
        bool ImagingDevice::reset_activity()
        {
                return carrier_->reset_activity();
        }

        bool ImagingDevice::power_up()
        {
                return camera_->power_up()
                        && carrier_->power_up();
        }
        
        bool ImagingDevice::power_down()
        {
                return camera_->power_down()
                        && carrier_->power_down();
        }
        
        bool ImagingDevice::stand_by()
        {
                return camera_->stand_by()
                        && carrier_->stand_by();
        }
        
        bool ImagingDevice::wake_up()
        {
                return camera_->wake_up()
                        && carrier_->wake_up();
        }
}
