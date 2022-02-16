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

#include <vector>
#include <stdexcept>
#include <iostream>

#include "oquam/plotter.h"
#include "oquam/print.h"
#include "oquam/is_valid.h"
#include "oquam/Oquam.h"

namespace romi {
        
        Oquam::Oquam(ICNCController& controller,
                     OquamSettings& settings,
                     ISession& session)
                : controller_(controller),
                  settings_(settings),
                  session_(session),
                  mutex_(),
                  store_script_(false),
                  position_changed_(true)
        {
                if (!controller_.configure_homing(settings_.homing_[0],
                                                  settings_.homing_[1],
                                                  settings_.homing_[2])) {
                        throw std::runtime_error("Oquam: configure_homing failed");
                }
        }
                
        bool Oquam::get_range(CNCRange &range)
        {
                range = settings_.range_;
                return true;
        }

        bool Oquam::get_position(int32_t *position) 
        {
                return controller_.get_position(position);
        }
        
        bool Oquam::get_position(v3& position)
        {
                int32_t p[3];
                bool success = get_position(p);
                if (success) {
                        position.set(p[0] / settings_.scale_meters_to_steps_[0],
                                     p[1] / settings_.scale_meters_to_steps_[1],
                                     p[2] / settings_.scale_meters_to_steps_[2]);
                }
                //else std::cout<<"get_position failed"<<std::endl; //test
                return success;
        }

        v3 Oquam::assert_get_position() 
        {
                v3 position;
                if (!get_position(position)) {
                        r_err("Oquam:: get_position failed!");
                        throw std::runtime_error("get_position failed");
                }
                return position;
        }

        bool Oquam::moveat(int16_t speed_x, int16_t speed_y, int16_t speed_z)
        {
            SynchronizedCodeBlock synchronize(mutex_);
            position_changed_ = true;
            store_script_ = false;
            return controller_.moveat(speed_x, speed_y, speed_z);
        }

        bool Oquam::moveto(double x, double y, double z, double relative_speed)
        {
                SynchronizedCodeBlock synchronize(mutex_);
                position_changed_ = true;
                store_script_ = false;
                return moveto_synchronized(x, y, z, relative_speed);
        }

        bool Oquam::moveto_synchronized(double x, double y, double z, double rel_speed)
        {
                bool success = false;
                try {
                        success = do_moveto(x, y, z, rel_speed);
                } catch (std::runtime_error& re) {
                        r_err("Oquam::moveto_synchronized: %s", re.what());
                }
                return success;
        }

        bool Oquam::do_moveto(double x, double y, double z, double rel_speed)
        {
                Path path;
                v3 p = moveto_determine_xyz(x, y, z);
                path.push_back(p);
                return travel_synchronized(path, rel_speed);
        }
        
        v3 Oquam::moveto_determine_xyz(double x, double y, double z)
        {
                v3 p = assert_get_position();
                if (x != UNCHANGED)
                        p.x() = x;
                if (y != UNCHANGED)
                        p.y() = y;
                if (z != UNCHANGED)
                        p.z() = z;
                return p;
        }
        
        bool Oquam::is_zero(int16_t *params)
        {
                return (params[0] == 0)
                        || ((params[1] == 0)
                            && (params[2] == 0)
                            && (params[3] == 0.0));
        }
        
        bool Oquam::spindle(double speed)
        {
                r_info("Oquam::spindle %f", speed);
                position_changed_ = true;
                return controller_.spindle(speed);
        }

        bool Oquam::homing()
        {
                bool homing_result = true;
                SynchronizedCodeBlock synchronize(mutex_);
                if (position_changed_) {
                    homing_result = controller_.homing();
                    position_changed_ = false;
                }
                return homing_result;
        }
                
        bool Oquam::travel(Path &path, double relative_speed)
        {
                SynchronizedCodeBlock synchronize(mutex_);
                position_changed_ = true;
                store_script_ = true;
                return travel_synchronized(path, relative_speed);
        }
        
        bool Oquam::travel_synchronized(Path &path, double relative_speed) 
        {
                bool success = false;
                
                try {
                        do_travel(path, relative_speed); 
                        success = true;
                        
                } catch (std::runtime_error& e) {
                        r_debug("Oquam::travel_synchronized: error: %s", e.what());
                }
                
                return success;
        }

        void Oquam::assert_relative_speed(double relative_speed) 
        {
                if (relative_speed <= 0.0 || relative_speed > 1.0)
                        throw std::runtime_error("Oquam: invalid speed");
        }
        
        void Oquam::assert_in_range(v3 p) 
        {
                if (!settings_.range_.is_inside(p)) { 
                        double e = settings_.range_.error(p);
                        r_warn("Oquam: Point[%d]: out of bounds: "
                               "(%0.6f, %0.6f, %0.6f)"
                               ", range (%0.6f, %0.6f, %0.6f), "
                               "error %.6f",
                               p.x(), p.y(), p.z(),
                               settings_.range_.max_.x(), settings_.range_.max_.y(),
                               settings_.range_.max_.z(), e);
                        if (e > 0.001) { 
                                throw std::runtime_error("Point out of bounds");
                        }
                }
        }
        
        void Oquam::do_travel(Path &path, double relative_speed) 
        {
                assert_relative_speed(relative_speed); 
                
                v3 start_position = assert_get_position();
                SmoothPath script(start_position);
                
                v3 vmax;
                vmax = settings_.vmax_ * relative_speed;
                
                double speed_ms = norm(settings_.vmax_ * relative_speed);
                
                convert_path_to_script(path, speed_ms, script);
                convert_script(script, vmax);
                store_script(script);
                check_script(script, vmax); 
                execute_script(script);
                wait_end_of_script(script); 
        }

        void Oquam::convert_path_to_script(Path &path, double speed, SmoothPath& script) 
        {
                for (size_t i = 0; i < path.size(); i++) {
                        assert_in_range(path[i]); 
                        script.moveto(path[i], speed);
                }
        }

        void Oquam::convert_script(SmoothPath& script, v3& vmax) 
        {
                script.convert(vmax.values(),
                               settings_.amax_.values(),
                               settings_.path_max_deviation_,
                               settings_.path_slice_duration_,
                               settings_.path_max_slice_duration_); 
        }

        void Oquam::store_script(SmoothPath& script) 
        {
                if (store_script_) {
                        store_script_svg(script);
                        store_script_json(script);
                        store_script_ = false;
                }
        }

        void Oquam::store_script_svg(SmoothPath &script)
        {
                rcom::MemBuffer svg = plot_to_mem(script, settings_.range_,
                                            settings_.vmax_.values(),
                                            settings_.amax_.values());
                if (svg.size() > 0) {
                        session_.store_svg("oquam.svg", svg.tostring());
                } else {
                        r_warn("Oquam::store_script: plot failed");
                }
        }

        void Oquam::store_script_json(SmoothPath &script)
        {
                rcom::MemBuffer text;
                print(script, text);
                session_.store_txt("oquam.json",text.tostring());
        }

        void Oquam::check_script(SmoothPath& script, v3& vmax) 
        {
                if (!is_valid(script, 
                              settings_.range_, vmax.values(),
                              settings_.amax_.values())) {
                        r_err("Oquam::convert_script: generated script is invalid");
                        throw std::runtime_error("is_valid(script) failed");
                }
        }
        
        void Oquam::execute_script(SmoothPath& script) 
        {
                if (script.count_slices() > 0) {
                        Section& section = script.get_slice(0);
                
                        // Initialize the start position
                        int32_t pos_steps[3];
                        convert_position_to_steps(section.p0, pos_steps); 
                        for (size_t k = 0; k < script.count_slices(); k++) {
                                execute_move(script.get_slice(k), pos_steps);
                        }
                }
        }

        void Oquam::execute_move(Section& section, int32_t *pos_steps)
        {
                int32_t p1[3];
                convert_position_to_steps(section.p1, p1); 

                int16_t params[4];
                params[0] = (int16_t) (1000.0 * section.duration);
                params[1] = (int16_t) (p1[0] - pos_steps[0]);
                params[2] = (int16_t) (p1[1] - pos_steps[1]);
                params[3] = (int16_t) (p1[2] - pos_steps[2]);
                
                if (!is_zero(params)) {
                        
                        assert_move(params);
                        
                        // Update the current position
                        pos_steps[0] = p1[0];
                        pos_steps[1] = p1[1];
                        pos_steps[2] = p1[2];
                }
        }
        
        void Oquam::assert_move(int16_t *params)
        {
                if (!controller_.move(params[0], params[1], params[2], params[3])) {
                        r_err("Oquam: move failed");
                        throw std::runtime_error("Oquam: move failed");
                }
        }

        void Oquam::convert_position_to_steps(double *position, int32_t *steps) 
        {
                double *scale = settings_.scale_meters_to_steps_;
                steps[0] = (int32_t) (position[0] * scale[0]);
                steps[1] = (int32_t) (position[1] * scale[1]);
                steps[2] = (int32_t) (position[2] * scale[2]);
        }

        void Oquam::wait_end_of_script(SmoothPath& script) 
        {
                double duration = script.get_duration();
                double timeout = 60.0 + 1.5 * duration;
                assert_synchronize(timeout);
        }

        void Oquam::assert_synchronize(double timeout)
        {
                if (!controller_.synchronize(timeout)) {
                        r_err("Oquam: synchronize failed");
                        throw std::runtime_error("Oquam: synchronize failed");
                }
        }

        bool Oquam::pause_activity()
        {
                return controller_.pause_activity();
        }
        
        bool Oquam::continue_activity()
        {
                return controller_.continue_activity();
        }
        
        bool Oquam::reset_activity()
        {
                return controller_.reset_activity();
        }

        bool Oquam::enable_driver()
        {
                SynchronizedCodeBlock synchronize(mutex_);
                return controller_.enable();
        }

        bool Oquam::disable_driver()
        {
                SynchronizedCodeBlock synchronize(mutex_);
                return controller_.disable();
        }

        bool Oquam::power_up()
        {
                return enable_driver() && homing();
        }
        
        bool Oquam::power_down()
        {
                return disable_driver();
        }
        
        bool Oquam::stand_by()
        {
                return disable_driver();
        }
        
        bool Oquam::wake_up()
        {
                return enable_driver();
        }
}
