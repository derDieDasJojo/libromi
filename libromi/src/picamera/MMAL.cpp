/*
  libromi

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  libromi is the base library for the Romi devices.

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
#include <stdexcept>

#include "picamera/MMAL.h"
#include "util/Logger.h"

namespace romi::arm {

        bool MMAL::status_is_ok(const char *caller_name, MMAL_STATUS_T status)
        {
                bool ok = (status == MMAL_SUCCESS);
                if (!ok) {
                        r_err("%s failed", caller_name);
                }
                return ok;
        }

        void MMAL::assert_status(const char *caller_name, MMAL_STATUS_T status)
        {
                if (!status_is_ok(caller_name, status)) {
                        throw std::runtime_error("Error in CameraComponent");
                }
        }
}
