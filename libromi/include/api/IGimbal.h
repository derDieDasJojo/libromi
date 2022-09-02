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
#ifndef __ROMI_I_GIMBAL_H
#define __ROMI_I_GIMBAL_H

#include "v3.h"
#include "api/IActivity.h"
#include "api/IPowerDevice.h"
#include "api/IRange.h"

namespace romi {
        
        class IGimbal : public IActivity, public IPowerDevice
        {
        public:
                
                virtual ~IGimbal() = default;

                // Angles in degrees.
                virtual bool moveto(double phi_x, double phi_y, double phi_z,
                                    double relative_speed) = 0;
                virtual bool get_position(v3& position) = 0; 
                virtual bool get_range(IRange& range) = 0; 
                virtual bool homing() = 0; 
        };
}

#endif // __ROMI_I_GIMBAL_H
