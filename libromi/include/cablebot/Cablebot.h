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
#ifndef __ROMI_CABLEBOT_H
#define __ROMI_CABLEBOT_H

#include <stdexcept>
#include <memory>
#include "api/IImagingDevice.h"
#include "api/ICNC.h"
#include "api/IGimbal.h"
#include "api/ICamera.h"

namespace romi {
        
        class Cablebot : public IImagingDevice
        {
        protected:
                std::unique_ptr<ICNC> base_;
                std::unique_ptr<IGimbal> gimbal_;
                std::unique_ptr<ICamera> camera_;
                CNCRange camera_range_;
                
                void validate_coordinates(double x, double y, double z);
                void validate_speed(double v);
                void validate_orientation(double pan, double tilt);
                
        public:
                Cablebot(std::unique_ptr<ICNC>& base,
                         std::unique_ptr<IGimbal>& gimbal,
                         std::unique_ptr<ICamera>& camera);
                
                virtual ~Cablebot() = default;

                bool get_range(CNCRange &range) override;
                //virtual bool get_camera_range(CameraRange &range) override;

                bool moveto(double x, double y, double z,
                            double relative_speed) override;
                
                bool moveat(int16_t speed_x, int16_t speed_y, int16_t speed_z) override;

                bool lookat(double pan_in_degrees, double tilt_in_degrees) override;
                
                bool helix(double xc, double yc, double alpha, double z,
                           double relative_speed = 0.1) override;
                
                bool homing() override;
                
                bool get_position(v3& position) override; 
                bool get_camera_position(double& pan, double& tilt) override; 

                rpp::MemBuffer& grab_jpeg() override;
        };
}

#endif // __ROMI_CABLEBOT_H
