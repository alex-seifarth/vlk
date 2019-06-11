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

#include <vlk/export.h>
#include <vulkan/vulkan.h>

#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

#define VLK_INVALID_QF_IDX          (std::numeric_limits<uint32_t>::max())

namespace vlk {

    struct VLK_EXPORT phys_device
    {
        explicit phys_device(VkPhysicalDevice dev);

        VkPhysicalDevice device;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        std::vector<VkQueueFamilyProperties> queue_family_properties;

        bool can_present_on_surface(uint32_t queue_family_idx, VkSurfaceKHR surface) const;
    };

    struct VLK_EXPORT phys_device_selection
    {
        VkPhysicalDevice device{VK_NULL_HANDLE};
        VkPhysicalDeviceFeatures features{};
        uint32_t qfi_graphics{VLK_INVALID_QF_IDX};
        uint32_t qfi_presentation{VLK_INVALID_QF_IDX};
    };

} // namespace vlk
