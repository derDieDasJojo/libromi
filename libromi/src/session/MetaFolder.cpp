
#include <iostream>
#include "data_provider/JsonFieldNames.h"
#include "session/MetaFolder.h"
#include "util/ClockAccessor.h"

namespace romi {
        MetaFolder::MetaFolder(std::shared_ptr<IIdentityProvider> identityProvider,
                           std::shared_ptr<ILocationProvider> locationProvider,
                           std::filesystem::path folder_path)
                : identityProvider_(std::move(identityProvider)),
                  locationProvider_(std::move(locationProvider)),
                  folderPath_(folder_path),
                  meta_data_(),
                  metadata_file_mutex_() {
                
                if (nullptr == identityProvider_){
                        throw std::invalid_argument("identityProvider");
                }
                if (nullptr == locationProvider_){
                        throw std::invalid_argument("locationProvider");
                }
                try_create();
        }

        std::filesystem::path
        MetaFolder::build_filename_with_extension(const std::string& filename,
                                                  const std::string& extension)
        {
                std::filesystem::path filename_extension(filename);
                filename_extension.replace_extension(extension);
                return filename_extension;
        }
        
        void MetaFolder::try_create()
        {
                std::filesystem::create_directories(folderPath_);

                meta_data_ = std::make_unique<nlohmann::json>();
                (*meta_data_)[JsonFieldNames::romi_identity] = identityProvider_->identity();
                SaveMetaData();
        }

        void MetaFolder::add_file_metadata(const std::string &filename,
                                           const std::string &observationId)
        {
                nlohmann::json newFile;
                newFile[JsonFieldNames::observation_id] = observationId;
                newFile[JsonFieldNames::location] = locationProvider_->location();
                newFile[JsonFieldNames::date_time] = romi::ClockAccessor::GetInstance()->datetime_compact_string();
                (*meta_data_)[filename] = newFile;
                SaveMetaData();
        }

        void MetaFolder::CheckInput(Image& image) const
        {
                if (meta_data_ == nullptr)
                        throw std::runtime_error("Session not created");
                if (image.data().empty())
                        throw std::runtime_error("Image data empty");
        }

        void MetaFolder::CheckInput(const std::string& string_data,
                                    bool empty_ok) const
        {
                (void) string_data;
                if (meta_data_ == nullptr)
                        throw std::runtime_error("Session not created");
                if (string_data.empty() && !empty_ok)
                        throw std::runtime_error("String data empty");
        }

        void MetaFolder::CheckInput(rcom::MemBuffer& jpeg) const
        {
                if (meta_data_ == nullptr)
                        throw std::runtime_error("Session not created");
                if (jpeg.data().empty())
                        throw std::runtime_error("Image data empty");
        }

        void MetaFolder::try_store_jpg(const std::string &filename,
                                       romi::Image &image,
                                       const std::string &observationId)
        {
                std::scoped_lock<std::recursive_mutex> scopedLock(metadata_file_mutex_);
                CheckInput(image);
                auto filename_extension = build_filename_with_extension(filename, "jpg");
                if (!ImageIO::store_jpg(image, (folderPath_ / filename_extension).c_str()))
                        throw std::runtime_error(std::string("try_store_jpg failed to write ")
                                                 + filename_extension.string());
                add_file_metadata(filename_extension, observationId);;
        }

        void MetaFolder::try_store_jpg(const std::string &filename,
                                       rcom::MemBuffer& jpeg,
                                       const std::string &observationId)
        {
                std::scoped_lock<std::recursive_mutex> scopedLock(metadata_file_mutex_);
                CheckInput(jpeg);
                auto filename_extension = build_filename_with_extension(filename, "jpg");
                FileUtils::TryWriteVectorAsFile((folderPath_ / filename_extension), jpeg.data());
                add_file_metadata(filename_extension, observationId);;
        }

        void MetaFolder::try_store_png(const std::string &filename,
                                       romi::Image &image,
                                       const std::string &observationId)
        {
                std::scoped_lock<std::recursive_mutex> scopedLock(metadata_file_mutex_);
                CheckInput(image);
                auto filename_extension = build_filename_with_extension(filename, "png");
                if (!ImageIO::store_png(image, (folderPath_ / filename_extension).c_str()))
                        throw std::runtime_error(std::string("try_store_png failed to write ")
                                                 + filename_extension.string());
                add_file_metadata(filename_extension, observationId);;
        }

        void MetaFolder::try_store_svg(const std::string &filename,
                                       const std::string& body,
                                       const std::string &observationId)
        {
                std::scoped_lock<std::recursive_mutex> scopedLock(metadata_file_mutex_);
                CheckInput(body, false);
                auto filename_extension = build_filename_with_extension(filename, "svg");
                FileUtils::TryWriteStringAsFile((folderPath_ / filename_extension), body);
                add_file_metadata(filename_extension, observationId);
        }

        void MetaFolder::try_store_txt(const std::string &filename,
                                       const std::string& text,
                                       const std::string &observationId)
        {
                std::scoped_lock<std::recursive_mutex> scopedLock(metadata_file_mutex_);
                CheckInput(text, true);
                auto filename_extension = build_filename_with_extension(filename, "txt");
                FileUtils::TryWriteStringAsFile((folderPath_ / filename_extension), text);
                add_file_metadata(filename_extension, observationId);
        }

        void MetaFolder::try_store_path(const std::string &filename,
                                        romi::Path &weeder_path,
                                        const std::string &observationId)
        {
                std::scoped_lock<std::recursive_mutex> scopedLock(metadata_file_mutex_);
                std::stringstream ss;
                Path::iterator ptr;
                for (ptr = weeder_path.begin(); ptr < weeder_path.end(); ptr++)  {
                        v3 p = *ptr;
                        ss << p.x() << "\t" << p.y() << "\r\n";
                }
                std::string path_data = ss.str();
                CheckInput(path_data, true);
                auto filename_extension = build_filename_with_extension(filename, "path");
                FileUtils::TryWriteStringAsFile((folderPath_ / filename_extension), path_data);
                add_file_metadata(filename_extension, observationId);
        }

        void MetaFolder::SaveMetaData() const {
            FileUtils::TryWriteStringAsFile((folderPath_ / meta_data_filename_), (*meta_data_).dump(4));
        }


}
