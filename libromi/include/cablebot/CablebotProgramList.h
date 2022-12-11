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
#ifndef __ROMI_CABLEBOTPROGRAMLIST_H
#define __ROMI_CABLEBOTPROGRAMLIST_H

#include <vector>
#include "cablebot/ICablebotProgramList.h"

namespace romi {

        class CablebotProgramList : public ICablebotProgramList
        {
        protected:
                std::vector<std::shared_ptr<ICablebotProgram>> list_;
                
        public:
                CablebotProgramList();
                ~CablebotProgramList() override = default;
                
                size_t count() override;
                std::shared_ptr<ICablebotProgram> get(size_t index) override;
                std::shared_ptr<ICablebotProgram> find(uint8_t hour, uint8_t minute) override;
                void insert(std::shared_ptr<ICablebotProgram>& program) override;
        };        

}

#endif // __ROMI_CABLEBOTPROGRAM_H
