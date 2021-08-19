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
#ifndef _IMU_BNO055_H
#define _IMU_BNO055_H

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to SCL pin (analog 5 on Arduino UNO)
   Connect SDA to SDA pin (analog 4 on Arduino UNO)
   Connect VDD to 3-5V DC (depending on your board's logic level)
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
   2015/AUG/27  - Added calibration and system status helpers
*/

#include "Romi_Adafruit_BNO055.h"
#include "IMU.h"

namespace romi {

        class BNO055 : public IMU
        {
        protected:
                Romi_Adafruit_BNO055 bno_;
                unsigned long last_time_;
                uint8_t calib_system_;
                uint8_t calib_gyro_;
                uint8_t calib_accel_;
                uint8_t calib_mag_;

                void wait_update_interval();
                bool check_calibration_status();
                void update_calibration_status();
                void do_calibrate();
                void load_offsets();
                void store_offsets();

        public:
                BNO055();
                virtual ~BNO055() override = default;

                bool setup() override;
                bool calibrate() override;
                bool get_orientation(Orientation& orientation,
                                     double& calibration) override;
        };
}

#endif // _IMU_BNO055_H


