
#include <iostream>
#include "data_provider/JsonFieldNames.h"
#include "weeder_session/MetaFolder.h"
#include "ClockAccessor.h"


namespace romi {
    MetaFolder::MetaFolder(std::unique_ptr<IIdentityProvider> identityProvider,
                           std::unique_ptr<ILocationProvider> locationProvider)
        : identityProvider_(std::move(identityProvider)), locationProvider_(std::move(locationProvider)),
        path_(), meta_data_() {

        if(nullptr == identityProvider_){
                throw std::invalid_argument("identityProvider");
        }
        if(nullptr == locationProvider_){
                throw std::invalid_argument("locationProvider");
        }

    }

    void MetaFolder::try_create(const std::filesystem::path& path) {
            path_ = path;
            std::filesystem::create_directories(path_);
            std::string identity = identityProvider_->identity();
            JsonCpp identityObjet(identity.c_str());
            meta_data_ = std::make_unique<JsonCpp>();
            json_object_set(meta_data_->ptr(), JsonFieldNames::romi_identity.c_str(), identityObjet.ptr());
            meta_data_->save((path_ / meta_data_filename_).string(), k_json_pretty);
    }

    void MetaFolder::add_file_metadata(const std::string &filename, const std::string &ovservationId) {
            std::string location = locationProvider_->location();
            JsonCpp locationJson(location.c_str());

            JsonCpp newFile;
            json_object_setstr(newFile.ptr(), JsonFieldNames::observation_id.c_str(), ovservationId.c_str());
            json_object_set(newFile.ptr(), JsonFieldNames::location.c_str(), locationJson.ptr());
            json_object_setstr(newFile.ptr(), JsonFieldNames::date_time.c_str(), rpp::ClockAccessor::GetInstance()->datetime_compact_string().c_str());
            json_object_set(meta_data_->ptr(), filename.c_str(), newFile.ptr());
            meta_data_->save((path_ / meta_data_filename_).string(), k_json_pretty);
    }

    void MetaFolder::CheckInput(Image& image) const
    {
            if (meta_data_ == nullptr)
                    throw std::runtime_error("Session not created");
            if (image.data().empty())
                    throw std::runtime_error("Image data empty");
    }

    void MetaFolder::CheckInput(const std::string& string_data) const
    {
            if (meta_data_ == nullptr)
                    throw std::runtime_error("Session not created");
            if (string_data.empty())
                    throw std::runtime_error("String data empty");
    }

    void MetaFolder::try_store_jpg(const std::string &filename, romi::Image &image, const std::string &observationId) {
            CheckInput(image);
            if (!ImageIO::store_jpg(image, (path_/filename).c_str()))
                    throw std::runtime_error(std::string("try_store_jpe failed to write ") + (path_/filename).string());
            add_file_metadata(filename, observationId);;
    }

    void MetaFolder::try_store_png(const std::string &filename, romi::Image &image, const std::string &observationId) {
            CheckInput(image);
            if (!ImageIO::store_png(image, (path_/filename).c_str()))
                    throw std::runtime_error(std::string("try_store_jpe failed to write ") + (path_/filename).string());
            add_file_metadata(filename, observationId);;
    }

    void MetaFolder::try_store_svg(const std::string &filename, const std::string& body, const std::string &observationId) {
            try_store_txt(filename, body, observationId);
    }

    void MetaFolder::try_store_txt(const std::string &filename, const std::string& text,
                                   const std::string &observationId) {
            CheckInput(text);
            FileUtils::TryWriteStringAsFile((path_/filename), text);
            add_file_metadata(filename, observationId);
    }

    void
    MetaFolder::try_store_path(const std::string &filename, romi::Path &weeder_path, const std::string &observationId) {
            std::stringstream ss;
            Path::iterator ptr;
            for (ptr = weeder_path.begin(); ptr < weeder_path.end(); ptr++)  {
                    v3 p = *ptr;
                    ss << p.x() << "\t" << p.y() << "\r\n";
            }
            CheckInput(ss.str());
            FileUtils::TryWriteStringAsFile((path_/filename), ss.str());
            add_file_metadata(filename, observationId);
    }


}
