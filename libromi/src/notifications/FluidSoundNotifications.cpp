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

#include <string>
#include <stdexcept>
#include <iostream>
#include "util/Logger.h"
#include "notifications/FluidSoundNotifications.h"

#if (FLUIDSYNTH_VERSION_MAJOR == 1)
typedef unsigned int fluid_int;
#else
typedef int fluid_int;
#endif

namespace romi {

        FluidSoundNotifications::FluidSoundNotifications(const std::string& soundfont,
                                                         nlohmann::json& sounds)
                : _settings(nullptr), _synth(nullptr), _adriver(nullptr), _sfont_id(0), _sounds()
        {
                try {
                        init_settings();
                        start_synth();
                        load_sound_font(soundfont.c_str());
                        try_add_sounds(sounds);
                 
                } catch (std::runtime_error& e) {
                        r_err("FluidSoundNotification: init failed: %s", e.what());
                        clean_up();
                        throw e;
                }
        }

        FluidSoundNotifications::~FluidSoundNotifications()
        {
                clean_up();
        }
        
        void FluidSoundNotifications::clean_up()
        {
                if (_adriver)
                        delete_fluid_audio_driver(_adriver);
                if (_synth)
                        delete_fluid_synth(_synth);
                if (_settings)
                        delete_fluid_settings(_settings);
        }

        void FluidSoundNotifications::init_settings()
        {
                _settings = new_fluid_settings();
                fluid_settings_setstr(_settings, "audio.driver", "alsa");
                fluid_settings_setint(_settings, "audio.period-size", 512);
                fluid_settings_setint(_settings, "audio.periods", 4);
                fluid_settings_setnum(_settings, "synth.gain", 4.0f);
        }

        void FluidSoundNotifications::start_synth()
        {
                _synth = new_fluid_synth(_settings);
                _adriver = new_fluid_audio_driver(_settings, _synth);
        }

        void FluidSoundNotifications::load_sound_font(const char *soundfont)
        {
                r_info("FluidSoundNotification: loading soundfont %s", soundfont);
                
                _sfont_id = fluid_synth_sfload(_synth, soundfont, 1);
                if (_sfont_id == FLUID_FAILED) {
                        r_err("Failed to load the soundfont: %s", soundfont);
                        throw std::runtime_error("Failed to load the soundfont");
                }
        }

        void FluidSoundNotifications::try_add_sounds(nlohmann::json& sounds)
        {
                try {
                        add_sounds(sounds);
                        
                } catch (nlohmann::json::exception& je) {
                        r_err("FluidSoundNotification: Failed to get "
                              "user-interface.fluid-sounds.sounds: %s", je.what());
                        throw je;
                }
        }
        
        void FluidSoundNotifications::add_sounds(nlohmann::json& sounds)
        {
            for (auto& el : sounds.items())
            {
                std::cout << "key: " << el.key() << ", value:" << el.value() << '\n';
                add_sounds(el.value());
            }
        }
        
        int32_t FluidSoundNotifications::add_sound(const std::string& notification, nlohmann::json& sound)
        {
            if (sound.is_object())
            {
                int preset = sound["preset"];
                int volume = sound["volume"];
                add_sound(notification, preset, volume);

            } else {
                r_err("FluidSoundNotification: Value '%s' in list of sounds is not valid", sound.dump().c_str());
            }
                return 0;
        }

        void FluidSoundNotifications::add_sound(const std::string& notification,
                                                int preset, int volume)
        {
                r_info("FluidSoundNotification: %s -> preset %d", notification.c_str(), preset);
                _sounds.insert(std::pair<std::string, Sound>(notification, Sound(preset, volume)));
        }
                
        Sound& FluidSoundNotifications::get_sound(const char *name)
        {
                return _sounds.at(name);
        }

        void FluidSoundNotifications::notify(const char *name)
        {
                try {
                        play(name);
                } catch (...) {
                        r_warn("FluidSoundNotification: Failed to play '%s'", name);
                }
        }

        void FluidSoundNotifications::play(const char *name)
        {
                Sound& sound = get_sound(name);
                r_debug("FluidSoundNotification: play %s, preset %d, volume %d",
                        name, sound.preset, sound.volume);
                fluid_synth_program_select(_synth, 0, (fluid_int)_sfont_id,
                                           0, (fluid_int)sound.preset); // TODO
                fluid_synth_noteon(_synth, 0, 60, sound.volume);
        }
        
        void FluidSoundNotifications::stop(__attribute__((unused)) const char *name)
        {
                fluid_synth_noteoff(_synth, 0, 60);
        }

        void FluidSoundNotifications::reset()
        {
                for (int i = 0; i < 16; i++)
                        fluid_synth_all_notes_off(_synth, 0);
        }
}
