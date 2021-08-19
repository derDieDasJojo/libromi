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
#include <EEPROM.h>
#include "BNO055.h"

namespace romi {

        static const unsigned long kBNO055SamplerateDelayMS = 100;
        static const uint32_t kBNO055ID = 0xABCD1234;

        BNO055::BNO055() :
                bno_(BNO055_ADDRESS_A, &Wire),
                last_time_(0),
                calib_system_(0),
                calib_gyro_(0),
                calib_accel_(0),
                calib_mag_(0)

        {
        }

        bool BNO055::setup()
        {
                bool success = false;
                if (bno_.begin()) {
                        success = true;
                        delay(1000);
                        bno_.setExtCrystalUse(true);
                }
                
                load_offsets();
                delay(1000);

                double cal;
                Orientation orientation;
                unsigned long start = millis();
                
                while (millis() - start < 2000) {
                        get_orientation(orientation, cal);
                }

                return success;
        }

        bool BNO055::calibrate()
        {
                do_calibrate();
                store_offsets();                
                Serial.print("#!stored:xxxx\r\n");
                return true;
        }

        void BNO055::do_calibrate()
        {
                uint8_t calib_system = 0;
                uint8_t calib_gyro = 0;
                uint8_t calib_accel = 0;
                uint8_t calib_mag = 0;

                while (!bno_.isFullyCalibrated()) {
                        bno_.getCalibration(&calib_system, &calib_gyro,
                                            &calib_accel, &calib_mag);
                        Serial.print("#!sys=");
                        Serial.print(calib_system, DEC);
                        Serial.print(",gyro=");
                        Serial.print(calib_gyro, DEC);
                        Serial.print(",accel=");
                        Serial.print(calib_accel, DEC);
                        Serial.print(",mag=");
                        Serial.print(calib_mag, DEC);
                        Serial.print(":xxxx\r\n");
                        delay(kBNO055SamplerateDelayMS);
                }
        }

        void BNO055::load_offsets()
        {
                adafruit_bno055_offsets_t offsets;
                uint32_t id;
                
                EEPROM.get(0, id);
                if (id == kBNO055ID) {
                        EEPROM.get(4, offsets);
                        bno_.setSensorOffsets(offsets);
                        Serial.print("#!loaded offsets:xxxx\r\n");
                } else {
                        Serial.print("#!no offsets:xxxx\r\n");
                }
        }

        void BNO055::store_offsets()
        {
                adafruit_bno055_offsets_t offsets;
                
                bno_.getSensorOffsets(offsets);
                EEPROM.put(0, kBNO055ID);
                EEPROM.put(4, offsets);
        }

        bool BNO055::get_orientation(Orientation& orientation,
                                     double& calibration)
        {
                wait_update_interval();
                
                Vector3 v = bno_.getEuler();
                orientation.x = v.x;
                orientation.y = v.y;
                orientation.z = v.z;

                update_calibration_status();
                calibration = (double) calib_system_ / 3.0;
                
                return true;
        }

        void BNO055::wait_update_interval()
        {
                unsigned long now = millis();
                while (now - last_time_ < kBNO055SamplerateDelayMS) {
                        delay(1);
                        now = millis();
                }
                last_time_ = now;
        }

        void BNO055::update_calibration_status()
        {
                /* Get the four calibration values (0..3). Any sensor data
                   reporting 0 should be ignored, 3 means "fully
                   calibrated" */
                bno_.getCalibration(&calib_system_, &calib_gyro_,
                                    &calib_accel_, &calib_mag_);
        }

        bool BNO055::check_calibration_status()
        {
                update_calibration_status();
                return (calib_system_ > 0);
        }
}
