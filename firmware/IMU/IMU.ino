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
#include <RomiSerial.h>
#include <ArduinoSerial.h>

#include "BNO055.h"
#include "IMUCommands.h"

romi::BNO055 imu_;
romiserial::ArduinoSerial serial(Serial);
romiserial::RomiSerial romi_serial_(serial, serial);

void setup(void)
{
        Serial.begin(115200);
        Serial.println("Orientation Sensor Test");
        Serial.println("");

        if (!imu_.setup()) {
                Serial.print("imu.begin failed");
                while(1);
        }

        romi::setup_commands(&imu_, &romi_serial_);
}

void loop(void)
{
        romi::handle_commands();
        
        // Orientation orientation;
        // if (imu.get_orientation(orientation)) {
        //         Serial.print("orientation=");
        //         Serial.print(orientation.x, 5);
        //         Serial.print(", ");
        //         Serial.print(orientation.y, 5);
        //         Serial.print(", ");
        //         Serial.print(orientation.z, 5);
        //         Serial.println();
        // }
}
