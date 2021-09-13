/*
  Romi IMU interface

  Copyright (C) 2021 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  Romi IMU is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */
#ifndef _IMU_IMU_H
#define _IMU_IMU_H

#include <stdint.h>

namespace romi {
        
        struct Orientation {
                float x;
                float y;
                float z;

                Orientation() {
                        x = 0.0f;
                        y = 0.0f;
                        z = 0.0f;
                }
        };

        class IMU
        {
        public:
                virtual ~IMU() = default;

                virtual bool setup() = 0;
                virtual bool calibrate() = 0;
                virtual bool get_orientation(Orientation& orientation,
                                             double& calibration) = 0;
        };
}

#endif // _IMU_IMU_H

