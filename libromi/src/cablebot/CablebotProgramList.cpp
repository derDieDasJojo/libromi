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

#include "cablebot/CablebotProgramList.h"

namespace romi {
        
        CablebotProgramList::CablebotProgramList()
                : list_()
        {
        }
        
        size_t CablebotProgramList::count()
        {
                return list_.size();
        }
        
        std::shared_ptr<ICablebotProgram> CablebotProgramList::get(size_t index)
        {
                return list_[index];
        }

        std::shared_ptr<ICablebotProgram> CablebotProgramList::find(uint8_t hour, uint8_t minute)
        {
                std::shared_ptr<ICablebotProgram> result;
                for (size_t i = 0; i < count(); i++) {
                        std::shared_ptr<ICablebotProgram> tmp;
                        tmp = get(i);
                        if (tmp->hour() == hour && tmp->minute() == minute) {
                                result = tmp;
                                break;
                        }
                }
                return result;
        }

        void CablebotProgramList::insert(std::shared_ptr<ICablebotProgram>& program)
        {
                list_.push_back(program);
        }
}
