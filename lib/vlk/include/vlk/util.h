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
#include <vlk/phys_device.h>

#include <vulkan/vulkan.h>
#include <cassert>
#include <string>

namespace vlk {

    //! Translate VkResult enumeration values to string.
    std::string VLK_EXPORT to_string(VkResult r);

    //! Translate VkPhyiscalDeviceType value to string.
    std::string VLK_EXPORT to_string(VkPhysicalDeviceType dt);

    std::string VLK_EXPORT to_string(VkFormat f);
    std::string VLK_EXPORT to_string(VkColorSpaceKHR cs);
    std::string VLK_EXPORT to_string(VkPresentModeKHR pm);

    //! Logs the properties of a physical device from VULKAN.
    void VLK_EXPORT log_phys_device(vlk::phys_device const& pd, VkSurfaceKHR surface, std::string const& prefix = {});

    void VLK_EXPORT log(vlk::swap_properties_selection const& sps, std::string const& prefix = {});

    //! returns value clamped to range [limit_low, limit_high]
    template<typename Tp>
    inline Tp const& clamp_range(Tp const& value, Tp const& limit_low, Tp const& limit_high)
    {
        assert(limit_low <= limit_high);
        if (value < limit_low) return limit_low;
        if (value > limit_high) return limit_high;
        return value;
    }

} // namespace vlk
