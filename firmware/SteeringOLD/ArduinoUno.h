/*
  Romi motor controller for brushed motors

  Copyright (C) 2021 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  Azhoo is free software: you can redistribute it and/or modify it
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
#ifndef _STEERING_ARDUINO_UNO_H
#define _STEERING_ARDUINO_UNO_H

#include "IArduino.h"
#include "IncrementalEncoderUno.h"

class ArduinoUno : public IArduino
{
public:
        static const uint8_t kLeftEncoderPinA = 2;
        static const uint8_t kLeftEncoderPinB = 4;
        static const uint8_t kRightEncoderPinA = 3;
        static const uint8_t kRightEncoderPinB = 5;

public:
        
        ArduinoUno();
        ~ArduinoUno() override = default;
        
        void setup();
        
        void init_encoders(uint16_t encoder_steps,
                           int8_t left_increment,
                           int8_t right_increment) override;
        
        IEncoder& left_encoder() override;
        IEncoder& right_encoder() override;

        uint32_t milliseconds() override {
                return millis();
        }
};

#endif // _STEERING_ARDUINO_UNO_H
