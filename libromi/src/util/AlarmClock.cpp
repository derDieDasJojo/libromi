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

#include "util/ClockAccessor.h"
#include "util/Logger.h"
#include "util/AlarmClock.h"

namespace romi {

        using SynchronizedCodeBlock = std::lock_guard<std::mutex>;

        AlarmClock::AlarmClock(std::shared_ptr<IAlarmClockListener>& listener)
                : listener_(listener),
                  wakeup_times_(),
                  thread_(),
                  mutex_(),
                  running_(true),
                  last_minute_(60)
        {
        }

        AlarmClock::~AlarmClock()
        {
                running_ = false;
                if (thread_) {
                        thread_->join();
                }
        }

        void AlarmClock::set_listener(std::shared_ptr<IAlarmClockListener>& listener)
        {
                SynchronizedCodeBlock synchronize(mutex_);
                listener_ = listener;
        }

        void AlarmClock::add_wakeup_time(uint8_t hour, uint8_t minute)
        {
                SynchronizedCodeBlock synchronize(mutex_);
                wakeup_times_.emplace_back(hour, minute);
        }
        
        void AlarmClock::clear()
        {
                wakeup_times_.clear();
        }

        void AlarmClock::start()
        {
                SynchronizedCodeBlock synchronize(mutex_);
                if (!thread_) {
                        thread_ = std::make_unique<std::thread>(
                                [this]() {
                                        main_loop();
                                });
                } else {
                        throw std::runtime_error("AlarmClock::start: already started");
                }
        }

        void AlarmClock::main_loop()
        {
                auto clock = romi::ClockAccessor::GetInstance();
                while (running_) {
                        try_check();
                        clock->sleep(1.0);
                }
        }

        void AlarmClock::try_check()
        {
                try {
                        check();
                } catch (std::exception& e) {
                        r_err("AlarmClock::try_check: Exception: %s", e.what());
                }
        }
        
        bool AlarmClock::check()
        {
                auto clock = romi::ClockAccessor::GetInstance();
                Date date = clock->date();
                bool alarm = false;
                if (date.minute_ != last_minute_) {
                        alarm = check_at(date.hour_, date.minute_);
                        last_minute_ = date.minute_;
                }
                return alarm;
        }

        bool AlarmClock::check_at(uint8_t hour, uint8_t minute)
        {
                SynchronizedCodeBlock synchronize(mutex_);
                bool alarm = false;
                for (size_t i = 0; i < wakeup_times_.size(); i++) {
                        if (matches(wakeup_times_[i], hour, minute)) {
                                wakeup(hour, minute);
                                alarm = true;
                        }
                }
                return alarm;
        }

        bool AlarmClock::matches(AlarmTime& alarm_time, uint8_t hour, uint8_t minute)
        {
                return (alarm_time.hour_ == hour
                        && alarm_time.minute_ == minute);
        }

        void AlarmClock::wakeup(uint8_t hour, uint8_t minute)
        {
                if (listener_) {
                        r_info("AlarmClock::wakeup: Calling listener: %02d:%02d",
                               (int) hour, (int) minute);
                        listener_->wakeup(hour, minute);
                }
        }
}


