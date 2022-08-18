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

#ifndef __ROMI_ICAMERAINFOIO_H
#define __ROMI_ICAMERAINFOIO_H

#include <memory>
#include <json.hpp>
#include "camera/ICameraInfo.h"

namespace romi {
        
        class ICameraInfoIO
        {
        public:
                virtual ~ICameraInfoIO() = default;

                virtual std::unique_ptr<ICameraInfo> load(nlohmann::json& json) = 0;
                virtual nlohmann::json store(std::unique_ptr<ICameraInfo>& info) = 0;
        };
}

#endif // __ROMI_ICAMERAINFOIO_H
