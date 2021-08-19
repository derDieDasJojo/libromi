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

#include <Arduino.h>
#include "IMUCommands.h"
#include "IMUVersion.h"

using namespace romiserial;

namespace romi {
        
        const static MessageHandler handlers[] = {
                { '?', 0, false, send_info },
                { 'c', 0, false, handle_calibrate },
                { 'o', 0, false, handle_get_orientation },
        };

        static IMU *imu_ = nullptr;
        static IRomiSerial *romi_serial_ = nullptr;
        static char reply_buffer[100];

        void setup_commands(IMU *imu, IRomiSerial *romiSerial)
        {
                imu_ = imu;
                romi_serial_ = romiSerial;
                if (romi_serial_)
                        romi_serial_->set_handlers(handlers,
                                                   sizeof(handlers)
                                                   / sizeof(MessageHandler));
        }

        void handle_commands()
        {
                if (imu_ != nullptr
                    && romi_serial_ != nullptr)
                        romi_serial_->handle_input();
        }

        void send_info(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
        {
                (void) args;
                (void) string_arg;
        
                romiSerial->send("[0,\"IMU\",\"" kIMUVersion "\","
                                 "\"" __DATE__ " " __TIME__ "\"]"); 
        }

        void handle_calibrate(romiserial::IRomiSerial *romiSerial,
                              int16_t *args, const char *string_arg)
        {
                if (imu_->calibrate()) {
                        romiSerial->send_ok();  
                } else {
                        romiSerial->send_error(1, "Failed");  
                }
        }

        void handle_get_orientation(romiserial::IRomiSerial *romiSerial,
                                    int16_t *args, const char *string_arg)
        {
                double calibration;
                Orientation orientation;
                if (imu_->get_orientation(orientation, calibration)) {
                        char x[10];
                        char y[10];
                        char z[10];
                        char c[10];
                        dtostrf(orientation.x, 3, 3, x);
                        dtostrf(orientation.y, 3, 3, y);
                        dtostrf(orientation.z, 3, 3, z);
                        dtostrf(calibration, 3, 3, c);
                        snprintf(reply_buffer, sizeof(reply_buffer),
                                 "[0,%s,%s,%s,%s]", x, y, z, c);
                        romiSerial->send(reply_buffer);                
                } else {
                        romiSerial->send_error(1, "Failed");  
                }
        }
}
