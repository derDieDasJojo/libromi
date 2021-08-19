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
#ifndef _IMU_IMUCOMMANDS_H
#define _IMU_IMUCOMMANDS_H

#include <IRomiSerial.h>
#include "IMU.h"

namespace romi {
        
        void setup_commands(IMU *imu, romiserial::IRomiSerial *romiSerial);
        void handle_commands();

        void send_info(romiserial::IRomiSerial *romiSerial,
                       int16_t *args, const char *string_arg);
        void handle_calibrate(romiserial::IRomiSerial *romiSerial,
                              int16_t *args, const char *string_arg);
        void handle_get_calibration(romiserial::IRomiSerial *romiSerial,
                                    int16_t *args, const char *string_arg);
        void handle_set_calibration(romiserial::IRomiSerial *romiSerial,
                                    int16_t *args, const char *string_arg);
        void handle_get_orientation(romiserial::IRomiSerial *romiSerial,
                                    int16_t *args, const char *string_arg);
}

#endif // _IMU_IMUCOMMANDS_H
