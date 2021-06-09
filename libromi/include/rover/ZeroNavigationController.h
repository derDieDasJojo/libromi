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
#ifndef __ROMI_ZERO_NAVIGATION_CONTROLLER_H
#define __ROMI_ZERO_NAVIGATION_CONTROLLER_H

#include "rover/INavigationController.h"

namespace romi {
        
        class ZeroNavigationController : public INavigationController
        {
        public:
                ~ZeroNavigationController() override = default;
                
                double estimate_correction(double cross_track_error,
                                           double orientation_error) override
                {
                        (void) cross_track_error;
                        (void) orientation_error;
                        return 0.0;
                }
        };
}

#endif // __ROMI_ZERO_NAVIGATION_CONTROLLER_H
