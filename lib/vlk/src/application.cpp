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
#include <vlk/application.h>
#include <vlk/exception.h>
#include <vlk/final.h>
#include <vlk/log.h>
#include <vlk/util.h>

#include "vulkan-bindings.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <algorithm>
#include <cstring>

#define VLK_VK_LAYER_LUNARG_STANDARD_VALIDATION_NAME  "VK_LAYER_LUNARG_standard_validation"

namespace {

    void stringvec_2_charvec(std::vector<char const*>& target, std::vector<std::string> const& src)
    {
        for (auto const& str : src) {
            auto i = std::find_if(target.begin(), target.end(), [&str](char const* ostr){return 0 == strcmp(ostr, str.c_str());});
            if (i == target.end()) {
                target.emplace_back(str.c_str());
            }
        }
    }

    void append_char_unique(std::vector<const char*>& target, char const* str)
    {
        auto i = std::find_if(target.begin(), target.end(), [&str](char const* ostr){return 0 == strcmp(ostr, str);});
        if (i == target.end()) {
            target.emplace_back(str);
        }
    }

    void setup_queue_create_info(VkDeviceQueueCreateInfo& qci, uint32_t queue_family_index, uint32_t queue_count, float * priority)
    {
        qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qci.pNext = nullptr;
        qci.flags = 0;
        qci.queueFamilyIndex = queue_family_index;
        qci.queueCount = queue_count;
        qci.pQueuePriorities = priority;
    }

}

using namespace vlk;

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

#else
#define DBG_PRINT_GLFW_VERSION()
#define DBG_PRINT_CHAR_VEC(msg, vec)
#define DBG_PRINT_PHYS_DEVICES(msg, vec)
#endif



application::application()
{
    if (GLFW_TRUE != glfwInit()) {
        throw vlk::glfw_exception("GLFW init failed");
    }
    DBG_PRINT_GLFW_VERSION();
}

application::~application()
{
    glfwTerminate();
}

void application::run()
{
    vlk::final fin{[this](){this->cleanup_run();}};
    init_run();
    while(!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
    }
}

void application::cleanup_run() noexcept
{
    _vk_queue_gfx = VK_NULL_HANDLE;
    _vk_queue_pres = VK_NULL_HANDLE;
    if (VK_NULL_HANDLE != _vk_device) {
        vkDestroyDevice(_vk_device, nullptr);
    }
    if (VK_NULL_HANDLE != _vk_surface) {
        vkDestroySurfaceKHR(_vk_instance, _vk_surface, nullptr);
        _vk_surface = VK_NULL_HANDLE;
    }
    if (VK_NULL_HANDLE != _vk_dbg_cbk) {
        vlk::destroyDebugReportCallbackEXT(_vk_instance, _vk_dbg_cbk, nullptr);
        _vk_dbg_cbk = VK_NULL_HANDLE;
    }
    if (VK_NULL_HANDLE != _vk_instance) {
        vkDestroyInstance(_vk_instance, nullptr);
        _vk_instance = VK_NULL_HANDLE;
    }
    if (nullptr != _window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
}

void application::init_run()
{
    create_window();
    create_vk_instance();
    install_validation_report_cbk();
    create_surface();
    create_device();
}

void application::create_window()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window = glfwCreateWindow(static_cast<int>(_window_width), static_cast<int>(_window_height), _app_name.c_str(),
            nullptr, nullptr);
    if (nullptr == _window) {
        throw vlk::glfw_exception{"window creation failed"};
    }
}

void application::create_vk_instance()
{
    std::vector<std::string> required_extensions{};
    std::vector<std::string> required_layers{};
    det_instance_requirements(required_extensions, required_layers);

    std::vector<char const*> rexts{};
    char const **glfw_required_extensions;
    uint32_t glfw_extension_count{0};
    glfw_required_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    for (uint32_t i = 0; i < glfw_extension_count; ++i) {
        append_char_unique(rexts, *(glfw_required_extensions + i));
    }
    if (_vk_enable_validation) {
        append_char_unique(rexts, VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }
    stringvec_2_charvec(rexts, required_extensions);
    DBG_PRINT_CHAR_VEC(Required Extensions, rexts);

    std::vector<char const*> rlayr{};
    stringvec_2_charvec(rlayr, required_layers);
    if (_vk_enable_validation) {
        append_char_unique(rlayr, VLK_VK_LAYER_LUNARG_STANDARD_VALIDATION_NAME);
    }
    DBG_PRINT_CHAR_VEC(Required Layers, rlayr);

    VkApplicationInfo ai;
    ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ai.pNext = nullptr;
    ai.pApplicationName = _app_name.c_str();
    ai.applicationVersion = VK_MAKE_VERSION(1U, 0U, 0U);
    ai.pEngineName = "No Engine";
    ai.engineVersion = VK_MAKE_VERSION(1U, 0U, 0U);
    ai.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo ci;
    ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ci.pNext = nullptr;
    ci.flags = 0;
    ci.pApplicationInfo = &ai;
    ci.enabledLayerCount = static_cast<uint32_t>(rlayr.size());
    ci.ppEnabledLayerNames = rlayr.data(),
    ci.enabledExtensionCount = static_cast<uint32_t>(rexts.size());
    ci.ppEnabledExtensionNames = rexts.data();

    auto r = vkCreateInstance(&ci, nullptr, &_vk_instance);
    if (VK_SUCCESS != r) {
        throw vlk::vulkan_exception{"Unable to create Vulkan instance", r};
    }
}

void application::det_instance_requirements([[maybe_unused]] std::vector<std::string>& required_extensions,
                                            [[maybe_unused]] std::vector<std::string>& required_layers)
{}

void application::install_validation_report_cbk()
{
    if (_vk_enable_validation) {
        // this will work only if the layer VK_LAYER_LUNARG_STANDARD_VALIDATION and the extension
        // VK_EXT_DEBUG_REPORT_EXTENSION_NAME are enabled in the Vulkan instance
        VkDebugReportCallbackCreateInfoEXT cbk_create_info;
        cbk_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        cbk_create_info.pNext = nullptr;
        cbk_create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                                VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        cbk_create_info.pfnCallback = &application::vk_debug_report_cbk;
        cbk_create_info.pUserData = static_cast<void*>(this);

        auto r = vlk::createDebugReportCallbackEXT(_vk_instance, &cbk_create_info, nullptr, &_vk_dbg_cbk);
        if (r != VK_SUCCESS) {
            throw vlk::vulkan_exception{"Unable to register validation layer callback", r};
        }
    }
}

VkBool32 application::vk_debug_report_cbk(VkDebugReportFlagsEXT flags,
                                          VkDebugReportObjectTypeEXT object_type,
                                          uint64_t object,
                                          size_t location,
                                          int32_t message_code,
                                          const char* p_layer_prefix,
                                          const char* p_message,
                                          void* p_user_data)
{
    assert(p_user_data);
    auto app = reinterpret_cast<vlk::application*>(p_user_data);
    app->on_vk_debug_msg(flags, object_type, object, location, message_code, p_layer_prefix, p_message);
    return VK_FALSE;
}

void application::on_vk_debug_msg([[maybe_unused]] VkDebugReportFlagsEXT flags,
                                  [[maybe_unused]] VkDebugReportObjectTypeEXT object_type,
                                  [[maybe_unused]] uint64_t object,
                                  [[maybe_unused]] size_t location,
                                  [[maybe_unused]] int32_t message_code,
                                  [[maybe_unused]] const char* p_layer_prefix,
                                  [[maybe_unused]] const char* p_message)
{
    VLK_LOG_ERROR() << "VULKAN " << p_layer_prefix << " - " << p_message;
}

void application::vk_debug_report_msg(VkDebugReportFlagsEXT flags,
                         VkDebugReportObjectTypeEXT objectType,
                         uint64_t object,
                         size_t location,
                         int32_t messageCode,
                         std::string const& pLayerPrefix,
                         std::string const& pMessage)
{
    if (_vk_instance == VK_NULL_HANDLE) {
        throw vlk::app_exception{"Debug messages not possible when no Vulkan instance available"};
    }
    if (!_vk_enable_validation) {
        throw vlk::app_exception{"Debug validation layer not activated"};
    }
    vlk::debugReportMessageEXT(_vk_instance, flags, objectType, object, location, messageCode, pLayerPrefix.c_str(),
            pMessage.c_str());
}

void application::create_surface()
{
    auto r = glfwCreateWindowSurface(_vk_instance, _window, nullptr, &_vk_surface);
    if (VK_SUCCESS != r) {
        throw vlk::vulkan_exception{"Unable to create window surface", r};
    }
    assert(VK_NULL_HANDLE != _vk_surface);
}

std::vector<vlk::phys_device> application::get_list_phys_devices()
{
    if(VK_NULL_HANDLE == _vk_instance) {
        throw vlk::app_exception{"Illegal operation - no instance allocated"};
    }

    uint32_t pd_count{0};
    vkEnumeratePhysicalDevices(_vk_instance, &pd_count, nullptr);
    if (0 == pd_count) {
        return std::vector<vlk::phys_device>{};
    }
    std::vector<VkPhysicalDevice> pds{pd_count};
    vkEnumeratePhysicalDevices(_vk_instance, &pd_count, pds.data());

    std::vector<vlk::phys_device> r{};
    r.reserve(pd_count);
    for (auto const& pd : pds) {
        r.emplace_back(pd);
    }
    return r;
}

void application::create_device()
{
    auto avail_phys_devs = get_list_phys_devices();
    DBG_PRINT_PHYS_DEVICES(Available Physical Devices, avail_phys_devs, _vk_surface);
    auto selected = det_physical_device_queue(avail_phys_devs, _vk_surface);
    if (VK_NULL_HANDLE == selected.device) {
        throw app_exception{"no physical device selected"};
    }
    if (VLK_INVALID_QF_IDX == selected.qfi_graphics || VLK_INVALID_QF_IDX == selected.qfi_presentation) {
        throw app_exception{"no queue family for GFX or presentation found"};
    }

    bool single_queue{selected.qfi_presentation == selected.qfi_graphics};
    float prio_gfx_queue = 1.0f;
    float prio_pres_queue = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> qci{single_queue ? 1U : 2U};
    setup_queue_create_info(qci[0], selected.qfi_graphics, 1, &prio_gfx_queue);
    if (!single_queue) {
        setup_queue_create_info(qci[1], selected.qfi_presentation, 1, &prio_pres_queue);
    }

    VkDeviceCreateInfo ci;
    ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    ci.pNext = nullptr;
    ci.pEnabledFeatures = &selected.features;
    ci.flags = 0;
    ci.enabledLayerCount = 0;
    ci.enabledExtensionCount = 0;
    ci.queueCreateInfoCount = qci.size();
    ci.pQueueCreateInfos = qci.data();

    auto r = vkCreateDevice(selected.device, &ci, nullptr, &_vk_device);
    if (VK_SUCCESS != r) {
        throw vlk::vulkan_exception{"Unable to create logical device", r};
    }
    _phys_dev_selected = selected;
    vkGetDeviceQueue(_vk_device, _phys_dev_selected.qfi_graphics, 0, &_vk_queue_gfx);
    vkGetDeviceQueue(_vk_device, _phys_dev_selected.qfi_presentation, 0, &_vk_queue_pres);
    if (VK_NULL_HANDLE == _vk_queue_pres || _vk_queue_gfx == VK_NULL_HANDLE) {
        throw vlk::vulkan_exception{"Failed to get queue handles", VK_RESULT_MAX_ENUM};
    }
}

vlk::phys_device_selection application::det_physical_device_queue(std::vector<vlk::phys_device> const& available_devices,
        VkSurfaceKHR surface)
{
    // default selection simply searches for the first possible device and queue families for GFX and presentation
    for (auto const& pd : available_devices) {
        vlk::phys_device_selection pds{};
        pds.device = pd.device;

        uint32_t qfidx{0};
        for (auto const &qfp : pd.queue_family_properties) {
            if (VLK_INVALID_QF_IDX == pds.qfi_graphics && (0 != (qfp.queueFlags & VK_QUEUE_GRAPHICS_BIT))) {
                pds.qfi_graphics = qfidx;
            }
            if (VLK_INVALID_QF_IDX == pds.qfi_presentation && pd.can_present_on_surface(qfidx, surface)) {
                pds.qfi_presentation = qfidx;
            }
            if (VLK_INVALID_QF_IDX != pds.qfi_graphics && VLK_INVALID_QF_IDX != pds.qfi_presentation) {
                return pds;
            }
            ++qfidx;
        }
    }
    return vlk::phys_device_selection{};  // nothing selected -> will throw in create_device
}
