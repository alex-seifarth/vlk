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

#include <boost/log/trivial.hpp>

#include <vlk/export.h>
#include <ostream>

namespace vlk {

    using log_level = boost::log::trivial::severity_level;
    extern void VLK_EXPORT set_global_log_level(log_level lv);

} // namespace vlk

#define VLK_LOG_TRACE() \
    BOOST_LOG_TRIVIAL(trace)

#define VLK_LOG_DEBUG() \
    BOOST_LOG_TRIVIAL(debug)

#define VLK_LOG_INFO() \
    BOOST_LOG_TRIVIAL(info)

#define VLK_LOG_WARNING() \
    BOOST_LOG_TRIVIAL(warning)

#define VLK_LOG_ERROR() \
    BOOST_LOG_TRIVIAL(error)

#define VLK_LOG_FATAL() \
    BOOST_LOG_TRIVIAL(fatal)
