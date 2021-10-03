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
#ifndef __ROMI_METHODS_GIMBAL_H
#define __ROMI_METHODS_GIMBAL_H

namespace romi {
        
        class MethodsGimbal
        {
        public:
                
                static constexpr const char *get_angle = "gimbal-get-angle";
                static constexpr const char *set_angle = "gimbal-set-angle";
                static constexpr const char *moveto = "gimbal-moveto";
                static constexpr const char *moveat = "gimbal-moveat";

                static constexpr const char *angle_param = "angle";
                static constexpr const char *angle_result = "angle";
                static constexpr const char *rps_param = "rps";
        };
}

#endif // __ROMI_METHODS_GIMBAL_H
