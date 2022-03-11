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
#ifndef __ROMI_IMAGINGDEVICE_H
#define __ROMI_IMAGINGDEVICE_H

#include <memory>
#include "api/ICamera.h"
#include "api/ICNC.h"
#include "api/IGimbal.h"
#include "api/IActivity.h"
#include "api/IPowerDevice.h"

namespace romi {
        
        class ImagingDevice : public IActivity, public IPowerDevice
        {
        public:

                std::shared_ptr<ICamera> camera_;
                std::shared_ptr<ICNC> cnc_;
                std::shared_ptr<IGimbal> gimbal_;
                
                ImagingDevice(std::shared_ptr<ICamera>& camera,
                              std::shared_ptr<ICNC>& cnc,
                              std::shared_ptr<IGimbal>& gimbal);
                
                virtual ~ImagingDevice() = default;

                // IActivity interface
                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;

                // Power device interface
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;                
        };
}

#endif // __ROMI_IMAGINGDEVICE_H