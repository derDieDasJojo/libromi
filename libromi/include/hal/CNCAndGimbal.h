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
#ifndef __ROMI_CNCANDGIMBAL_H
#define __ROMI_CNCANDGIMBAL_H

#include <memory>
#include "hal/IDisplacementDevice.h"
#include "api/ICNC.h"
#include "api/IGimbal.h"

namespace romi {
        
        class CNCAndGimbal : public IDisplacementDevice
        {
        protected:
                std::unique_ptr<ICNC> cnc_;
                std::unique_ptr<IGimbal> gimbal_;
                
        public:
                CNCAndGimbal(std::unique_ptr<ICNC>& cnc,
                             std::unique_ptr<IGimbal>& gimbal);
                ~CNCAndGimbal() override = default;

                bool get_cnc_range(CNCRange &range) override;
                bool get_gimbal_range(IRange &range) override;
                
                bool moveto(double x, double y, double z,
                            double phi_x, double phi_y, double phi_z,
                            double relative_speed) override;
        };
}

#endif // __ROMCNCANDGIMBAL_H
