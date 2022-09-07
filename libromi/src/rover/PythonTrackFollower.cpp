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

#include "util/Logger.h"
#include "util/ClockAccessor.h"
#include "rover/PythonTrackFollower.h"

namespace romi {

        PythonTrackFollower::PythonTrackFollower(ICamera& camera,
                                                 std::unique_ptr<rcom::IRPCClient>& rpc,
                                                 const std::string& function_name,
                                                 double pixels_per_meter,
                                                 ISession& session)
                : camera_(camera),
                  rpc_(),
                  function_name_(function_name),
                  pixels_per_meter_(pixels_per_meter),
                  session_(session),
                  cross_track_error_(0.0),
                  orientation_error_(0.0),
                  image_counter_(0)
        {
                if (pixels_per_meter_ <= 0.0) {
                        r_err("Invalid value for pixels_per_meter: %f", pixels_per_meter);
                        throw std::runtime_error("Invalid value for pixels_per_meter");
                }
                //rpc_ = romi::RcomClient::create("python", 30);
                rpc_ = std::move(rpc);
        }
        
        double PythonTrackFollower::get_cross_track_error()
        {
                return cross_track_error_;
        }
        
        double PythonTrackFollower::get_orientation_error()
        {
                return orientation_error_;
        }
        
        bool PythonTrackFollower::update_error_estimate()
        {
                bool success = false;
                try {
                        try_update();
                        success = true;
                } catch (const std::exception& e) {
                        r_err("PythonTrackFollower::update_error_estimate: caught: %s",
                              e.what());
                }
                return success;
        }
        
        void PythonTrackFollower::try_update()
        {
                auto clock = romi::ClockAccessor::GetInstance();
                double start_time = clock->time();

                auto filename = get_image_name();

                auto image = grab_image();
                r_debug("PythonTrackFollower: grab: %.6f s", clock->time()-start_time);
                
                store_image(image, filename);
                r_debug("PythonTrackFollower: store: %.6f s", clock->time()-start_time);
                
                auto path = get_image_path(filename);
                nlohmann::json response = send_python_request(path);
                r_debug("PythonTrackFollower: python: %.6f s", clock->time()-start_time);
                
                parse_response(response);
                r_debug("PythonTrackFollower: parse: %.6f s", clock->time()-start_time);
        }

        rcom::MemBuffer& PythonTrackFollower::grab_image()
        {
                image_counter_++;
                return camera_.grab_jpeg();
        }

        std::string PythonTrackFollower::get_image_name()
        {
                char filename[64];
                snprintf(filename, sizeof(filename), "%s-%06zu.jpg",
                         kImageName, image_counter_);
                return std::string(filename);
        }

        std::string PythonTrackFollower::get_image_path(std::string& name)
        {
                std::filesystem::path dir = session_.current_path();
                std::filesystem::path path = dir /= name;
                return path.string();
        }

        void PythonTrackFollower::store_image(rcom::MemBuffer& jpg, std::string& name)
        {
                if (!session_.store_jpg(name, jpg)) {
                        r_err("PythonTrackFollower::store_image: store_jpg failed");
                        throw std::runtime_error("Failed to save the image");
                }
        }

        nlohmann::json PythonTrackFollower::send_python_request(const std::string& path)
        {
                nlohmann::json response;
                rcom::RPCError error;
                
                nlohmann::json params;
                params["path"] = path;


                rpc_->execute(function_name_, params, response, error);
                
                if (error.code != 0) {
                        r_warn("Failed to call Python: %s", error.message.c_str());
                        throw std::runtime_error("Failed to call Python");
                        
                } else if (response["error"]["code"] != 0) {
                        std::string message = response["error"]["message"];
                        r_warn("Failed to call Python: %s", message.c_str());
                        throw std::runtime_error("Failed to call Python");
                }

                return response;
        }
                
        void PythonTrackFollower::parse_response(nlohmann::json& response)
        {
                cross_track_error_ = response["result"][kCrossTrackErrorKey];
                cross_track_error_ /= pixels_per_meter_;
                orientation_error_ = response["result"][kOrientationErrorKey];
                r_debug("cross_track_error %f, orientation_error %f",
                        cross_track_error_, orientation_error_);
        }
}
