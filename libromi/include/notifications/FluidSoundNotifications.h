/*
  romi-rover

  Copyright (C) 2019-2020 Sony Computer Science Laboratories
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

#ifndef _ROMI_FLUID_SOUND_NOTIFICATION_H
#define _ROMI_FLUID_SOUND_NOTIFICATION_H

#include <json.hpp>
#include <fluidsynth.h>
#include <map>

#include "api/INotifications.h"

namespace romi {

        struct Sound
        {
                int preset;
                int volume;

                Sound(int preset, int volume) : preset(preset), volume(volume){
                }
        };
              
        
        class FluidSoundNotifications : public INotifications
        {
        protected:
                fluid_settings_t *_settings;
                fluid_synth_t *_synth;
                fluid_audio_driver_t *_adriver;
                int _sfont_id;
                std::map<std::string, Sound> _sounds;

                int32_t add_sound(const std::string& notification, nlohmann::json& sound);

                void init_settings();
                void start_synth();
                void load_sound_font(const char *soundfont);
                void clean_up();
                void add_sound(const std::string& name, int preset, int volume);
                void add_sounds(nlohmann::json& config);
                void try_add_sounds(nlohmann::json& config);
                Sound& get_sound(const char *name);

                void play(const char *name);

        public:
                FluidSoundNotifications(const std::string& soundfont, nlohmann::json& sounds);
                FluidSoundNotifications(const FluidSoundNotifications&) = delete;
                FluidSoundNotifications& operator=(const FluidSoundNotifications&) = delete;
                ~FluidSoundNotifications() override;
                        
                void notify(const char *name) override;
                void stop(const char *name) override;
                void reset() override;
        };
}

#endif // _ROMI_FLUID_SOUND_NOTIFICATION_H
