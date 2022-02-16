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

#ifndef __ROMI_CNC_RANGE_H
#define __ROMI_CNC_RANGE_H

#include "json.hpp"
#include "v3.h"
#include "IRange.h"

namespace romi {

        class CNCRange : public IRange
        {
            public:
                v3 min_;
                v3 max_;
            public:
                CNCRange();
                explicit CNCRange(nlohmann::json &range);
                CNCRange(const double *min, const double *max);
                CNCRange(v3 min, v3 max);

                void init(nlohmann::json &range) override;

                v3 dimensions() const override;

                bool is_inside(double x, double y, double z) override;
                bool is_inside(v3 p) override;

                // Computes the distance of a point that lies outside
                // the range to the border of the range.
                double error(double x, double y, double z) override;
                double error(v3 p) override;

                v3 clamp(v3 p) const override;
        };
}

#endif // __ROMI_CNC_RANGE_H
