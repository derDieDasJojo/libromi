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
#ifndef __ROMI_CABLEBOTCONTROLLER_H
#define __ROMI_CABLEBOTCONTROLLER_H

#include <memory>
#include "util/IAlarmClock.h"
#include "cablebot/ICablebotProgramList.h"

namespace romi {

        class CablebotController
        {
        protected:
                std::shared_ptr<ICablebotProgramList> programs_;
                std::shared_ptr<IAlarmClock> alarmclock_;

                void program_alarmclock();
                
        public:
                CablebotController(std::shared_ptr<ICablebotProgramList>& programs,
                                   std::shared_ptr<IAlarmClock>& alarmclock);
                ~CablebotController() = default;

        };

}

#endif // __ROMI_CABLEBOTCONTROLLER_H
