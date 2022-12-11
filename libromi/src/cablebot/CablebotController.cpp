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
#include "cablebot/CablebotController.h"

namespace romi {

        CablebotController::CablebotController(std::shared_ptr<ICablebotProgramList>& programs,
                                               std::shared_ptr<IAlarmClock>& alarmclock)
                : programs_(programs),
                  alarmclock_(alarmclock)
        {
                program_alarmclock();
                alarmclock_->start();
        }

        void CablebotController::program_alarmclock()
        {
                alarmclock_->clear();
                for (size_t i = 0; i < programs_->count(); i++) {
                        auto program = programs_->get(i);
                        alarmclock_->add_wakeup_time(program->hour(), program->minute());
                }
        }
}
