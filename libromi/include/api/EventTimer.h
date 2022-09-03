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
#ifndef _ROMI_DEFAULT_EVENT_TIMER_H
#define _ROMI_DEFAULT_EVENT_TIMER_H

#include "util/ClockAccessor.h"
#include "api/IEventTimer.h"

namespace romi {
        
        class EventTimer : public IEventTimer
        {
        protected:
                int _event;
                double _event_time;
                std::shared_ptr<romi::IClock> _clock;

                bool has_timed_out();

        public:
                explicit EventTimer(int event)
                        : _event(event),
                          _event_time(0),
                          _clock(romi::ClockAccessor::GetInstance()){}
                ~EventTimer() override = default;

                int get_next_event() override;
                void set_timeout(double timeout) override;
                void reset() override;
        };
}

#endif // _ROMI_DEFAULT_EVENT_TIMER_H
