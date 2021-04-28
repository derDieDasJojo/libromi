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
#ifndef __ROMISERIAL_ROMISERIALERRORS_H
#define __ROMISERIAL_ROMISERIALERRORS_H

namespace romiserial {

        enum {
                kNoError = 0,

                // Errors generated by the EnvelopeParser 
                kEnvelopeTooLong = -1,
                kEnvelopeInvalidId = -2,
                kEnvelopeInvalidCrc = -3,
                kEnvelopeCrcMismatch = -4,
                kEnvelopeExpectedEnd = -5,
                kEnvelopeMissingMetadata = -6,
                kEnvelopeInvalidDummyMetadata = -7,
        
                // Errors generated by the MessageParser 
                kUnexpectedChar = -8,
                kVectorTooLong = -9,
                kValueOutOfRange = -10,
                kStringTooLong = -11,
                kInvalidString = -12,
                kTooManyStrings = -13,
                kInvalidOpcode = -14,

                // Errors generated by RomiSerial 
                kDuplicate = -15,
                kUnknownOpcode = -16,
                kBadNumberOfArguments = -17,
                kMissingString = -18,
                kBadString = -19,
                kBadHandler = -20,

                // Errors generated by RomiSerialClient 
                kClientInvalidOpcode = -21,
                kClientTooLong = -22,
                kEmptyResponse = -23,
                kEmptyRequest = -24,
                kConnectionTimeout = -25,        
                kInvalidJson = -26,
                kInvalidResponse = -27,
                kInvalidErrorResponse = -28,
        
                kLastError = -29
        };
}

#endif // __ROMISERIAL_ROMISERIALERRORS_H
