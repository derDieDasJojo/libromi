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

#include <stdexcept>
#include "cablebot/CablebotProgram.h"
#include "util/Logger.h"

namespace romi {

        CablebotProgram::CablebotProgram(uint32_t id, const std::string& name,
                                         uint8_t hour, uint8_t minute,
                                         double start, double length, double interval,
                                         double tilt, bool enabled)
                : id_(id),
                  name_(),
                  hour_(hour),
                  minute_(minute),
                  start_(start),
                  length_(length),
                  interval_(interval),
                  tilt_(tilt),
                  enabled_(enabled) {
                set_name(name);
        }

        uint32_t CablebotProgram::id() const
        {
                return id_;
        }
        
        const std::string& CablebotProgram::name() const
        {
                return name_;
        }
        
        void CablebotProgram::set_name(const std::string& name)
        {
                if (name.size() > kMaxNameLength) {
                        r_err("CablebotProgram::set_name: Too long (len=%d < %d)",
                              (int) name.size(), (int) kMaxNameLength);
                        throw std::runtime_error("CablebotProgram::set_name: Too long");
                }
                name_ = name;
        }
        
        uint8_t CablebotProgram::hour() const
        {
                return hour_;
        }
        
        void CablebotProgram::set_hour(uint8_t hour)
        {
                hour_ = hour;
        }
        
        uint8_t CablebotProgram::minute() const
        {
                return minute_;
        }
        
        void CablebotProgram::set_minute(uint8_t minute)
        {
                minute_ = minute;
        }
        
        double CablebotProgram::start() const
        {
                return start_;
        }
        
        void CablebotProgram::set_start(double start)
        {
                start_ = start;
        }
        
        double CablebotProgram::length() const
        {
                return length_;
        }
        
        void CablebotProgram::set_length(double length)
        {
                length_ = length;
        }
        
        double CablebotProgram::interval() const
        {
                return interval_;
        }
        
        void CablebotProgram::set_interval(double interval)
        {
                interval_ = interval;
        }
        
        double CablebotProgram::tilt() const
        {
                return tilt_;
        }
        
        void CablebotProgram::set_tilt(double tilt)
        {
                tilt_ = tilt;
        }
        
        bool CablebotProgram::is_enabled() const
        {
                return enabled_;
        }
        
        void CablebotProgram::set_enabled(bool value)
        {
                enabled_ = value;
        }

        void CablebotProgram::update(ICablebotProgram& new_values)
        {
                name_ = new_values.name();
                hour_ = new_values.hour();
                minute_ = new_values.minute();
                start_ = new_values.start();
                length_ = new_values.length();
                interval_ = new_values.interval();
                tilt_ = new_values.tilt();
                enabled_ = new_values.is_enabled();
        }
}
