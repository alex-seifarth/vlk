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

#include <vlk/log.h>
#include <vlk/util.h>

#ifndef NDEBUG
#define DBG_PRINT_GLFW_VERSION() \
    { \
        int mjr, mnr, rev; \
        glfwGetVersion(&mjr, &mnr, &rev); \
        VLK_LOG_DEBUG() << "Initialised GLFW " << mjr << "." << mnr << "." << rev; \
    } \
    void(0)

#define DBG_PRINT_CHAR_VEC(msg, vec) \
    { \
        VLK_LOG_DEBUG() << #msg; \
        for (auto const& s : vec) { \
            VLK_LOG_DEBUG() << "  - " << s; \
        } \
    } \
    void(0)

#define DBG_PRINT_PHYS_DEVICES(msg, vec, surface) \
    { \
        VLK_LOG_DEBUG() << #msg; \
        for (auto const& pd : vec) { \
            vlk::log_phys_device(pd, surface, " - "); \
        }\
    } \
    void(0)

#define DBG_PRINT_DEVICE_EXTENSIONS(msg, vec) \
    { \
        VLK_LOG_DEBUG() << #msg; \
        for (auto const& e : vec) { \
            VLK_LOG_DEBUG() << "  - " << e; \
        } \
    } \
    void(0)

#define DBG_PRINT_SWAP_CHAIN_PROPERTIES(msg, sps) \
    { \
        VLK_LOG_DEBUG() << #msg; \
        vlk::log(sps, "  - "); \
    } \
    void()

#else
#define DBG_PRINT_GLFW_VERSION()
#define DBG_PRINT_CHAR_VEC(msg, vec)
#define DBG_PRINT_PHYS_DEVICES(msg, vec)
#define DBG_PRINT_DEVICE_EXTENSIONS(msg, vec)
#define DBG_PRINT_SWAP_CHAIN_PROPERTIES(msg, sps)
#endif

namespace vlk
{

} // namespace vlk
