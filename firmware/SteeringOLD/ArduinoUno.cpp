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
#include <Arduino.h>
#include "ArduinoUno.h"

static IncrementalEncoderUno left_encoder_;
static IncrementalEncoderUno right_encoder_;

void setup_pin_change_interrupt(byte pin)
{
        *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
        PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
        PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

ArduinoUno::ArduinoUno()
{
}

// Interrupt callback for the left and right encoders
static void update_left_encoder()
{
        left_encoder_.update();
}

static void update_right_encoder()
{
        right_encoder_.update();
}

void ArduinoUno::setup()
{
}

void ArduinoUno::init_encoders(uint16_t encoder_steps,
                               int8_t left_increment,
                               int8_t right_increment)
{
        left_encoder_.init(encoder_steps,
                           left_increment, 
                           kLeftEncoderPinA,
                           kLeftEncoderPinB,
                           update_left_encoder);
        setup_pin_change_interrupt(A0);
        
        right_encoder_.init(encoder_steps,
                            right_increment, 
                            kRightEncoderPinA,
                            kRightEncoderPinB,
                            update_right_encoder);
        setup_pin_change_interrupt(A1);
}

IEncoder& ArduinoUno::left_encoder()
{
        return left_encoder_;
}

IEncoder& ArduinoUno::right_encoder()
{
        return right_encoder_;
}

ISR(PCINT1_vect) // handle pin change interrupt for A0 to A5 here
{
        if (digitalRead(A0)) {
                left_encoder_.set_index();
        }
        if (digitalRead(A1)) {
                right_encoder_.set_index();
        }
}  

