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
                
                static constexpr const char *kMoveto = "gimbal-moveto";
                static constexpr const char *kGetRange = "gimbal-get-range";
                static constexpr const char *kGetPosition = "gimbal-get-position";

                static constexpr const char *kSpeedParam = "speed";
                static constexpr const char *kMoveXParam = "x";
                static constexpr const char *kMoveYParam = "y";
                static constexpr const char *kMoveZParam = "z";
                static constexpr const char *kPositionX = "x";
                static constexpr const char *kPositionY = "y";
                static constexpr const char *kPositionZ = "z";
        };
}

#endif // __ROMI_METHODS_GIMBAL_H
