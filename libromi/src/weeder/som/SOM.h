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

#ifndef __ROMI_SOM_H
#define __ROMI_SOM_H

#include "weeder_session/ISession.h"
#include "weeder/IPathPlanner.h"
#include "SelfOrganizedMap.h"
#include "Superpixels.h"

namespace romi {
        
        class SOM : public IPathPlanner
        {
        protected:
                double _alpha;
                double _beta;
                double _epsilon;
                bool _print;

                void assert_settings();

        public:
                explicit SOM(JsonCpp& params);
                ~SOM() override = default;
                
                bool trace_path(ISession &session,
                                Image &mask,
                                double tool_diameter,
                                double meters_to_pixels,
                                Path &path) override;
        };
}

#endif // __ROMI_SOM_H
