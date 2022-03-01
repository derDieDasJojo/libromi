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
#ifndef __ROMI_REMOTE_CAMERA_H
#define __ROMI_REMOTE_CAMERA_H

#include <memory>
#include "rpc/RemoteStub.h"
#include "api/ICamera.h"

namespace romi {

        class RemoteCamera : public ICamera, public RemoteStub
        {
        public:
                static constexpr const char *ClassName = "remote-camera";

        protected:
                rpp::MemBuffer output_;

        public:
                RemoteCamera(std::unique_ptr<IRPCClient>& client);
                ~RemoteCamera() override = default;

                bool grab(Image &image) override;
                rpp::MemBuffer& grab_jpeg() override;

                // Power device interface
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;
        };
}

#endif // __ROMI_REMOTE_CAMERA_H
