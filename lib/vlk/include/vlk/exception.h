// ================================================================================================
//
// vlk  Vulkan support library to experiment with VULKAN SDK
//
// Copyright (C) 2019 Alexander Seifarth
//
// This program is free software; you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation; either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
// the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program;
// if not, write to the Free Software Foundation,
//          Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//
// ================================================================================================
#pragma once

#include <vlk/util.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <sstream>

namespace vlk {

    class app_exception : public std::runtime_error
    {
    public:
        explicit app_exception(std::string const& msg, int error_code = 0, std::string const& error_descr = {})
            : std::runtime_error{msg}
            , _error_code{error_code}
            , _error_descr{error_descr}
        {}

        int _error_code;
        std::string _error_descr;
        mutable std::string _what{};

        char const* what() const noexcept override
        {
            if (_what.empty()) {
                std::stringstream ss;
                ss << std::runtime_error::what() << " [Error: " << _error_code << " " << _error_descr << "]";
                _what = ss.str();
            }
            return _what.c_str();
        }
    };

    class glfw_exception : public app_exception
    {
    public:
        explicit glfw_exception(std::string const& msg)
            : app_exception(msg)
        {
            char const* description;
            _error_code = glfwGetError(&description);
            _error_descr = description;
        }
    };

    class vulkan_exception : public app_exception
    {
    public:
        explicit vulkan_exception(std::string const& msg, VkResult vulkan_error)
            : app_exception{msg, vulkan_error, vlk::to_string(vulkan_error)}
        {}
    };

} // namespace vlk
