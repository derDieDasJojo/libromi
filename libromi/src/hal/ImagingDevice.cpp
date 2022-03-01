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
                                     std::shared_ptr<ICNC>& cnc,
                                     std::shared_ptr<IGimbal>& gimbal)
                : camera_(camera),
                  cnc_(cnc),
                  gimbal_(gimbal)
        {
        }

        bool ImagingDevice::pause_activity()
        {
                return cnc_->pause_activity()
                        && gimbal_->pause_activity();
        }
        
        bool ImagingDevice::continue_activity()
        {
                return cnc_->continue_activity()
                        && gimbal_->continue_activity();
        }
        
        bool ImagingDevice::reset_activity()
        {
                return cnc_->reset_activity()
                        && gimbal_->reset_activity();
        }

        bool ImagingDevice::power_up()
        {
                return camera_->power_up()
                        && cnc_->power_up()
                        && gimbal_->power_up();
        }
        
        bool ImagingDevice::power_down()
        {
                return camera_->power_down()
                        && cnc_->power_down()
                        && gimbal_->power_down();
        }
        
        bool ImagingDevice::stand_by()
        {
                return camera_->stand_by()
                        && cnc_->stand_by()
                        && gimbal_->stand_by();
        }
        
        bool ImagingDevice::wake_up()
        {
                return camera_->wake_up()
                        && cnc_->wake_up()
                        && gimbal_->wake_up();
        }
}
