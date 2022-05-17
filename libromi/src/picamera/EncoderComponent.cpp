/*
  libromi

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  libromi is the base library for the Romi devices.

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
#include <stdexcept>
#include <r.h>
#include "picamera/EncoderComponent.h"

namespace romi::arm {

        EncoderComponent::EncoderComponent(const char* name)
                : Component(name),
                  input_(nullptr),
                  output_(nullptr),
                  pool_(nullptr),
                  buffer_()
        {
                assert_input_and_output();
                input_ = ptr_->input[0];
                output_ = ptr_->output[0];
        }
        
        EncoderComponent::~EncoderComponent()
        {
                check_disable_port(output_);
                if (pool_)
                        mmal_port_pool_destroy(output_, pool_);
        }

        void EncoderComponent::assert_input_and_output()
        {
                if (ptr_->input_num == 0 || ptr_->output_num == 0) {
                        r_err("EncoderComponent::assert_input_and_output() failed");
                        throw std::runtime_error("EncoderComponent::assert_input_and_output");
                }
        }

        rcom::MemBuffer& EncoderComponent::get_buffer()
        {
                return buffer_;
        }
        
        MMAL_PORT_T *EncoderComponent::get_input_port()
        {
                return input_;
        }
}

