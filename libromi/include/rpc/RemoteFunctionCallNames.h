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
#ifndef __ROMI_REMOTEFUNCTIONCALLNAMES_H
#define __ROMI_REMOTEFUNCTIONCALLNAMES_H

namespace romi
{
    class RemoteMessgeTypes
    {
        public:
            static constexpr const char* message_type_request           = "request";
            static constexpr const char* request_type_list              = "list";
            static constexpr const char* request_type_execute           = "execute";
            static constexpr const char* execute_type_id                = "id";
            static constexpr const char* execute_type_start_navigation  = "start_navigation";
            static constexpr const char* execute_type_stop_navigation   = "stop_navigation";
    };

}

#endif // __ROMI_REMOTEFUNCTIONCALLNAMES_H
