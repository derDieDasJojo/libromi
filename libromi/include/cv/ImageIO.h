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

#ifndef __ROMI_IMAGEIO_H
#define __ROMI_IMAGEIO_H

#include <vector>

#include "FileUtils.h"
#include "cv/Image.h"

namespace romi {

        using bytevector = std::vector<uint8_t>;
        const int JPEG_QUALITY_90 = 90;
        class ImageIO
        {

        public:
                static bool store_jpg(Image& image, const char *path);
                static bool store_png(Image& image, const char *path);
                
                static bool load(Image& image, const char *filename);
                static bool load_from_buffer(Image& image, const std::vector<uint8_t>& image_data);

        };
}

#endif // __ROMI_IMAGEIO_H
