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

#ifndef __ROMI_ALARMCLOCK_H
#define __ROMI_ALARMCLOCK_H

#include <memory>
#include <stdexcept>
#include <vector>
#include <mutex>
#include <thread>
#include <stdint.h>

#include "util/IAlarmClock.h"
#include "util/IAlarmClockListener.h"

namespace romi {

        // struct AlarmTime {
        //         uint8_t hour_;
        //         uint8_t minute_;
                
        //         AlarmTime(uint8_t hour, uint8_t minute)
        //                 : hour_(hour),
        //                   minute_(minute) {
        //                 if (hour_ > 23)
        //                         throw std::runtime_error("AlarmTime: invalid hour");
        //                 if (minute_ > 59)
        //                         throw std::runtime_error("AlarmTime: invalid minute");
        //         }
        // };
        
        class AlarmClock : public IAlarmClock
        {
        protected:
                std::shared_ptr<IAlarmClockListener> listener_;
                // std::vector<AlarmTime> wakeup_times_;
                std::unique_ptr<std::thread> thread_;
                // std::mutex mutex_;
                bool running_;
                uint8_t last_minute_;
                
                // bool matches(AlarmTime& alarm_time, uint8_t hour, uint8_t minute);
                void main_loop();
                void try_check();
                bool check() /*override*/;
                bool check_at(uint8_t hour, uint8_t minute) /*override*/;
                void wakeup(uint8_t hour, uint8_t minute);

        public:
                AlarmClock(std::shared_ptr<IAlarmClockListener>& listener);
                ~AlarmClock() override;

                void set_listener(std::shared_ptr<IAlarmClockListener>& listener) override;
                // void add_wakeup_time(uint8_t hour, uint8_t minute) override;
                // void clear() override;
                void start() override;
                
        };
}

#endif

