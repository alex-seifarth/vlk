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
#include <vlk/phys_device.h>
#include <cassert>


using namespace vlk;

phys_device::phys_device(VkPhysicalDevice dev)
    : device{dev}
    , properties{}
    , features{}
    , queue_family_properties{}
    , extensions{}
{
    assert(VK_NULL_HANDLE != device);

    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);

    uint32_t qf_count{0};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &qf_count, nullptr);
    queue_family_properties.resize(qf_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &qf_count, queue_family_properties.data());

    uint32_t extension_count{0};
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    extensions.resize(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extensions.data());
}

bool phys_device::can_present_on_surface(uint32_t queue_family_idx, VkSurfaceKHR surface) const
{
    assert(queue_family_idx < queue_family_properties.size());
    VkBool32 present{VK_FALSE};
    vkGetPhysicalDeviceSurfaceSupportKHR(device, queue_family_idx, surface, &present);
    return present != VK_FALSE;
}

bool phys_device::supports_extension(std::string const& extension_name) const
{
    for (auto const& ext : extensions) {
        if (extension_name == ext.extensionName) {
            return true;
        }
    }
    return false;
}

