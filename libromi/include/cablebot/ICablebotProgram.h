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
#ifndef __ROMI_I_CABLEBOTPROGRAM_H
#define __ROMI_I_CABLEBOTPROGRAM_H

#include <stdint.h>

namespace romi {

        class ICablebotProgram
        {
        public:
                
                virtual ~ICablebotProgram() = default;
                
                virtual uint32_t id() const = 0;
                virtual const std::string& name() const = 0;
                virtual void set_name(const std::string& name) = 0;
                virtual uint8_t hour() const = 0;
                virtual void set_hour(uint8_t hour) = 0;
                virtual uint8_t minute() const = 0;
                virtual void set_minute(uint8_t minute) = 0;
                virtual double start() const = 0;
                virtual void set_start(double start) = 0;
                virtual double length() const = 0;
                virtual void set_length(double length) = 0;
                virtual double interval() const = 0;
                virtual void set_interval(double interval) = 0;
                virtual double tilt() const = 0;
                virtual void set_tilt(double tilt) = 0;
                virtual bool is_enabled() const = 0;
                virtual void set_enabled(bool value) = 0;

                virtual void update(ICablebotProgram& new_values) = 0;
        };
}

#endif // __ROMI_I_CABLEBOTPROGRAM_H
