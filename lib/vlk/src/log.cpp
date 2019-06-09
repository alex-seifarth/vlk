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
#include <vlk/log.h>
#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

void vlk::set_global_log_level(log_level lv)
{
    namespace logging = boost::log;
    logging::core::get()->set_filter(logging::trivial::severity >= lv);
}
