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

#include <math.h>
#include <r.h>
#include "cablebot/Cablebot.h"

namespace romi {
        
        Cablebot::Cablebot(std::unique_ptr<ICNC>& base,
                           std::unique_ptr<IGimbal>& gimbal,
                           std::unique_ptr<ICamera>& camera)
                : base_(std::move(base)),
                  gimbal_(std::move(gimbal)),
                  camera_(std::move(camera)),
                  camera_range_()
        {
                camera_range_.init(v3(0.0, -90.0, 0.0),
                                   v3(0.0, 90.0, 0.0)); // TODO
        }
        
        bool Cablebot::get_range(CNCRange &range) 
        {
                return base_->get_range(range);
        }

        bool Cablebot::moveto(double x, double y, double z, double relative_speed)
        {
                return base_->moveto(x, y, z, relative_speed);
        }
        
        bool Cablebot::moveat(int16_t speed_x, int16_t speed_y, int16_t speed_z)
        {
                return base_->moveat(speed_x, speed_y, speed_z);
        }
        
        bool Cablebot::lookat(double pan, double tilt)
        {
                validate_orientation(pan, tilt);
                return gimbal_->moveto(tilt);
        }

        void Cablebot::validate_orientation(double pan, double tilt)
        {
                if (!camera_range_.is_inside(pan, tilt, 0.0)) {
                        r_err("Cablebot::validate_orientation: Orientation out of range: "
                                "(%.3f,%.3f)", pan, tilt);
                        throw std::runtime_error("Cablebot::validate_orientation: "
                                                 "Orientation out of range");
                }
        }
        
        bool Cablebot::helix(double xc, double yc, double alpha, double z,
                             double relative_speed)
        {
                (void) xc;
                (void) yc;
                (void) alpha;
                (void) z;
                (void) relative_speed;
                r_err("Cablebot::helix: Not implemented");
                throw std::runtime_error("Cablebot::helix: Not implemented");
        }
                
        bool Cablebot::homing()
        {
                return base_->homing();
        }
        
        bool Cablebot::get_position(v3& position)
        {
                return base_->get_position(position);
        }
        
        bool Cablebot::get_camera_position(double& pan, double& tilt)
        {
                double angle;
                bool success = gimbal_->get_angle(angle);
                if (success) {
                        pan = 0.0;
                        tilt = angle;
                }
                return success;
        }
        
        rpp::MemBuffer& Cablebot::grab_jpeg()
        {
                return camera_->grab_jpeg();
        }
}
