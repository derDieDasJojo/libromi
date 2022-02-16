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

#include "api/CNCRange.h"

namespace romi {
        
        CNCRange::CNCRange() : min_(0.0), max_(0.0) {
        }
        
        CNCRange::CNCRange(const double *xmin, const double *xmax)
                : min_(xmin), max_(xmax) {
        }
        
        CNCRange::CNCRange(v3 xmin, v3 xmax)
                : min_(xmin), max_(xmax) {
        }
        
        CNCRange::CNCRange(nlohmann::json& json) : min_(0.0), max_(0.0) {
                init(json);
        }
        
        void CNCRange::init(nlohmann::json& range)
        {
                for (size_t i = 0; i < 3; i++) {
                        min_.set(i, range[i][0]);
                        max_.set(i, range[i][1]);
                }
        }

        v3 CNCRange::dimensions() const
        {
                return max_ - min_;
        }
        
        bool CNCRange::is_inside(double x, double y, double z)
        {
                return ((x >= min_.x()) && (x <= max_.x())
                        && (y >= min_.y()) && (y <= max_.y())
                        && (z >= min_.z()) && (z <= max_.z()));
        }

        bool CNCRange::is_inside(v3 p)
        {
                return is_inside(p.x(), p.y(), p.z());
        }
        
        double CNCRange::error(double x, double y, double z)
        {
                double dx[3] = { 0.0, 0.0, 0.0 };
        
                if (x < min_.x())
                        dx[0] = min_.x() - x;
                else if (x > max_.x())
                        dx[0] = x - max_.x();
                
                if (y < min_.y())
                        dx[1] = min_.y() - y;
                else if (y > max_.y())
                        dx[1] = y - max_.y();
                
                if (z < min_.z())
                        dx[2] = min_.z() - z;
                else if (z > max_.z())
                        dx[2] = z - max_.z();
        
                return vnorm(dx);
        }

        double CNCRange::error(v3 v)
        {
                return error(v.x(), v.y(), v.z());
        }
        
        v3 CNCRange::clamp(v3 p) const
        {
                return p.clamp(min_, max_);
        }
}
