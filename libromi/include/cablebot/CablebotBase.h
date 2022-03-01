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
#ifndef __ROMI_CABLEBOTBASE_H
#define __ROMI_CABLEBOTBASE_H

#include <stdexcept>
#include <memory>
#include "api/ICNC.h"
#include "RomiSerialClient.h"

namespace romi {
        
        class CablebotBase : public ICNC
        {
        protected:

                static const constexpr double kPrecision = 36.0;
                
                std::unique_ptr<romiserial::IRomiSerialClient> serial_;
                CNCRange range_;
                double diameter_;
                double circumference_;

                void validate_coordinates(double x, double y, double z);
                void validate_speed(double v);
                bool send_command(const char *command);
                int16_t position_to_steps(double x);
                double steps_to_position(double steps);
                bool enable_driver();
                bool disable_driver();
                
        public:
                CablebotBase(std::unique_ptr<romiserial::IRomiSerialClient>& serial);
                virtual ~CablebotBase() = default;

                bool get_range(CNCRange &range) override;

                bool moveto(double x, double y, double z, double relative_speed) override;
                bool moveat(int16_t speed_x, int16_t speed_y, int16_t speed_z) override;
                bool homing() override;
                bool get_position(v3& position) override; 

                bool spindle(double speed) override;
                bool travel(Path &path, double relative_speed) override;
                bool helix(double xc, double yc, double alpha, double z,
                           double relative_speed) override;
                
                // IActivity interface
                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;

                // Power device interface
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;
        };
}

#endif // __ROMI_CABLEBOTBASE_H
