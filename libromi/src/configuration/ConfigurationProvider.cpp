#include <Logger.h>
#include <stdexcept>
#include "configuration/ConfigurationProvider.h"

namespace romi {

    std::string get_brush_motor_device_in_config(nlohmann::json &config)
    {
            std::string brush_motor_device;
            try {
                    brush_motor_device = config["ports"]["brush-motor-driver"]["port"];

            } catch (nlohmann::json::exception& je) {
                    r_warn("Failed to get the value for "
                           "ports.brush_motor.port: %s", je.what());
                    throw std::runtime_error("No brush_motor device specified");
            }
            return brush_motor_device;
    }

    std::string get_brush_motor_device(romi::IOptions &options, nlohmann::json &config)
    {
            std::string brush_motor_device = options.get_value(romi::RoverOptions::navigation_device);
            if (brush_motor_device.empty()) {
                    brush_motor_device = get_brush_motor_device_in_config(config);
            }
            return brush_motor_device;
    }

    std::string get_sound_font_in_config(nlohmann::json &config)
    {
            try {
                    return config["user-interface"]["fluid-sounds"][RoverOptions::soundfont];

            } catch (nlohmann::json::exception& je) {
                    r_err("FluidSoundNotification: Failed to read the config: %s",
                          je.what());
                    throw std::runtime_error("No soundfont in the config file");
            }
    }

    std::string get_sound_font_file(romi::IOptions &options, nlohmann::json &config)
    {
            std::string file = options.get_value(RoverOptions::soundfont);
            if (file.empty())
                    file = get_sound_font_in_config(config);
            return file;
    }

    std::string get_script_file_in_config(nlohmann::json &config)
    {
            try {
                    return config["user-interface"]["script-engine"][RoverOptions::script];

            } catch (nlohmann::json::exception& je) {
                    r_err("Failed to read script file in config: %s",
                          je.what());
                    throw std::runtime_error("No script file in the config file");
            }
    }

    std::string get_script_file(romi::IOptions &options, nlohmann::json &config)
    {
            std::string file = options.get_value(romi::RoverOptions::script);
            if (file.empty()) {
                    file = get_script_file_in_config(config);
            }
            return file;
    }

    std::string get_session_directory(romi::IOptions &options, nlohmann::json &config)
    {
            (void) config;
            std::string dir = options.get_value(romi::RoverOptions::session_directory);
            if (dir.empty()) {
                    // TODO: to be finalized: get seesion dir from config
                    // file, add the current date to the path, and create
                    // a new directory.
                    dir = ".";
            }
            return dir;
    }

    std::string get_camera_image(romi::IOptions& options, nlohmann::json& config)
    {
            std::string path = options.get_value(romi::RoverOptions::camera_image);
            if (path.empty()) {
                    path = config["weeder"]["file-camera"]["image"];
            }
            return path;
    }

    std::string get_camera_device_in_config(nlohmann::json& config)
    {
            try {
                    return config["ports"]["usb-camera"]["port"];

            } catch (nlohmann::json::exception& je) {
                    r_err("get_camera_device_in_config: Failed to get value "
                          "of ports.usb-camera.port");
                    throw std::runtime_error("Missing device name for camera in config");
            }
    }

    std::string get_camera_device(romi::IOptions& options, nlohmann::json& config)
    {
            std::string device = options.get_value(romi::RoverOptions::camera_device);
            if (device.empty())
                    device = get_camera_device_in_config(config);
            return device;
    }

    std::string get_camera_classname_in_config(nlohmann::json& config)
    {
            try {
                    return config["weeder"]["camera-classname"];

            } catch (nlohmann::json::exception& je) {
                    r_err("get_camera_classname_in_config: Failed to get value "
                          "of weeder.camera-classname");
                    throw std::runtime_error("Missing classname for camera in config");
            }
    }

    std::string get_camera_classname(romi::IOptions& options, nlohmann::json& config)
    {
            std::string device = options.get_value(romi::RoverOptions::camera_classname);
            if (device.empty())
                    device = get_camera_classname_in_config(config);
            return device;
    }
}
