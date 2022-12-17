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
#ifndef __ROMI_CABLEBOTPROGRAM_H
#define __ROMI_CABLEBOTPROGRAM_H

#include <string>
#include "cablebot/ICablebotProgram.h"

namespace romi {

        class CablebotProgram : public ICablebotProgram
        {
        protected:
                static const uint32_t kMaxNameLength = 64;

                uint32_t id_;
                std::string name_;
                uint8_t hour_;
                uint8_t minute_;
                double start_;
                double length_;
                double interval_;
                double tilt_;
                bool enabled_;

        public:
                CablebotProgram(uint32_t id, const std::string& name,
                                uint8_t hour, uint8_t minute,
                                double start, double length, double interval,
                                double tilt, bool enabled);

                ~CablebotProgram() override = default;
                
                uint32_t id() const override;
                const std::string& name() const override;
                void set_name(const std::string& name) override;
                uint8_t hour() const override;
                void set_hour(uint8_t hour) override;
                uint8_t minute() const override;
                void set_minute(uint8_t minute) override;
                double start() const override;
                void set_start(double start) override;
                double length() const override;
                void set_length(double length) override;
                double interval() const override;
                void set_interval(double interval) override;
                double tilt() const override;
                void set_tilt(double tilt) override;
                bool is_enabled() const override;
                void set_enabled(bool value) override;
                void update(ICablebotProgram& new_values) override;
        };
}

#endif // __ROMI_CABLEBOTPROGRAM_H
