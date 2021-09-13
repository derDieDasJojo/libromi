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

#ifndef __IRANGE_H
#define __IRANGE_H

#include "JsonCpp.h"
#include "v3.h"

namespace romi {
        
        class IRange
        {
        public:
                virtual ~IRange() = default;
                virtual void init(JsonCpp &range)=0;
                
                virtual v3 dimensions() const =0;
                
                virtual bool is_inside(double x, double y, double z)=0;
                virtual bool is_inside(v3 p)=0;
                
                // Computes the distance of a point that lies outside
                // the range to the border of the range.
                virtual double error(double x, double y, double z)=0;
                virtual double error(v3 p)=0;

                virtual v3 clamp(v3 p) const=0;
        };
}

#endif // __IRANGE_H