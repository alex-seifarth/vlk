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
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <string>
#include <vector>

namespace vlk {

    using extension_list = std::vector<VkExtensionProperties>;
    using layer_list = std::vector<VkLayerProperties>;

    class application
    {
    public:
        application();
        ~application();

        void run();

        //! Sends a debug report message via the Vulkan validation layer.
        //! \throws vlk::app_exception   Thrown when either no Vulkan instance created (e.g. outside run()) or when
        //!                             Vulkan debug validation layer is not active.
        //! \see vkDebugReportMessageEXT
        void vk_debug_report_msg(VkDebugReportFlagsEXT flags,
                                 VkDebugReportObjectTypeEXT objectType,
                                 uint64_t object,
                                 size_t location,
                                 int32_t messageCode,
                                 std::string const& pLayerPrefix,
                                 std::string const& pMessage);

    protected:
        virtual void det_instance_requirements(std::vector<std::string>& required_extensions,
                                               std::vector<std::string>& required_layers);

        virtual phys_device_selection det_physical_device_queue(std::vector<vlk::phys_device> const& available_devices,
                VkSurfaceKHR surface);

        virtual swap_properties_selection det_swap_chain_properties(VkSurfaceCapabilitiesKHR const& capabilities,
                                                                    std::vector<VkSurfaceFormatKHR> const& surface_formats,
                                                                    std::vector<VkPresentModeKHR> const& surface_present_modes,
                                                                    glm::uvec2 window_size);

        virtual void on_vk_debug_msg(VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT object_type,
                uint64_t object,
                size_t location,
                int32_t message_code,
                const char* p_layer_prefix,
                const char* p_message);


    private:
        void init_run();
        void cleanup_run() noexcept;

        void create_window();
        void create_vk_instance();
        void install_validation_report_cbk();
        void create_surface();
        void create_device();
        void create_swap_chain();
        void create_image_views();

        static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_report_cbk(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT,
            uint64_t, size_t, int32_t, const char*, const char*, void*);

        std::vector<vlk::phys_device> get_list_phys_devices();


        bool _vk_enable_validation{true};
        std::string _app_name{};
        uint32_t _window_width{800U};
        uint32_t _window_height{600U};
        GLFWwindow *_window{nullptr};

        VkInstance _vk_instance{VK_NULL_HANDLE};
        VkDebugReportCallbackEXT _vk_dbg_cbk{VK_NULL_HANDLE};
        VkSurfaceKHR _vk_surface{VK_NULL_HANDLE};

        vlk::phys_device_selection _phys_dev_selected{};
        VkDevice _vk_device{VK_NULL_HANDLE};
        VkQueue _vk_queue_gfx{VK_NULL_HANDLE};
        VkQueue _vk_queue_pres{VK_NULL_HANDLE};

        VkSwapchainKHR  _vk_swap_chain{VK_NULL_HANDLE};
        std::vector<VkImage> _vk_swap_chain_images{};
        VkSurfaceFormatKHR _vk_surface_format{};
        VkExtent2D _vk_surface_extent{};
        std::vector<VkImageView> _vk_swap_chain_img_views{};


    };

} // namespace vlk
