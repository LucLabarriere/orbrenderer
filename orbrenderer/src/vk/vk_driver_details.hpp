#pragma once

#include <array>
#include <cstdio>
#include <cstdlib>
#include <orb/assert.hpp>
#include <orb/flux.hpp>
#include <orb/platform.hpp>
#include <orb/print.hpp>
#include <orb/print_time.hpp>
#include <orb/result.hpp>
#include <orb/time.hpp>
#include <orb/utility.hpp>
#include <span>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "glfw/glfw_header.hpp"

#include "orb/glfw/window.hpp"
#include "orb/vk/vk_types.hpp"
#include "vk_helpers.hpp"
#include "vk_structs.hpp"

namespace orb::vk::details
{
    struct handles_t
    {
        VkAllocationCallbacks*   allocator      = nullptr;
        VkInstance               instance       = nullptr;
        VkPhysicalDevice         phy_dev        = nullptr;
        VkDevice                 dev            = nullptr;
        ui32                     queue_fam      = UINT32_MAX;
        VkQueue                  queue          = nullptr;
        VkDebugReportCallbackEXT deb_report     = nullptr;
        VkPipelineCache          pipeline_cache = nullptr;
        VkDescriptorPool         desc_pool      = nullptr;
        GLFWwindow*              window         = nullptr;
        VkSurfaceKHR             surface        = nullptr;

        ImGui_ImplVulkanH_Window main_win_data;
        i32                      min_img_count = 2;
        bool                     sc_rebuild    = false;
    };

    inline void check(VkResult err)
    {
        if (err == 0) { return; }

        throw orb::exception("Vulkan error: {}", (i64)err);
    }

    inline auto get_req_instance_exts() -> std::vector<const char*>
    {
        // Disable vulkan get required extensions in dev mode and on windows.
        // This speeds vulkan setup time by ~1.2s
        if constexpr (orb::assert_mode && on_windows)
        {
            return {
                khr_extensions::surface,
                khr_extensions::win32_surface,
            };
        }

        std::vector<const char*> extensions;
        ui32                     extensions_count = 0;
        const char**             glfw_extensions  = glfwGetRequiredInstanceExtensions(&extensions_count);
        extensions.resize(extensions_count);
        for (ui32 i = 0; i < extensions_count; i++)
        {
            extensions.push_back(glfw_extensions[i]);
        }

        return extensions;
    }

    inline VKAPI_ATTR auto VKAPI_CALL debug_report(VkDebugReportFlagsEXT      flags,
                                                   VkDebugReportObjectTypeEXT objectType,
                                                   uint64_t                   object,
                                                   size_t                     location,
                                                   int32_t                    messageCode,
                                                   const char*                pLayerPrefix,
                                                   const char*                pMessage,
                                                   void*                      pUserData) -> VkBool32
    {
        (void)flags;
        (void)object;
        (void)location;
        (void)messageCode;
        (void)pUserData;
        (void)pLayerPrefix;
        orb::println("Vulkan error from {}: {}", (i64)objectType, pMessage);
        return VK_FALSE;
    }

    inline auto is_ext_avail(std::span<VkExtensionProperties> properties, std::string_view extension) -> bool
    {
        for (const VkExtensionProperties& p : properties)
        {
            if (p.extensionName == extension) return true;
        }

        return false;
    }

    inline auto sel_phy_dev(handles_t& handles) -> result<VkPhysicalDevice>
    {
        auto gpus = enum_phy_devices(handles.instance).unwrap();

        for (VkPhysicalDevice& device : gpus)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);

            if (properties.deviceType == phy_device_types::discrete) { return device; }
        }

        // Use first GPU (Integrated) is a Discrete one is not available.
        if (gpus.size() > 0) { return gpus[0]; }

        return error_t { "No GPU was found" };
    }

    inline auto setup_vk(handles_t& handles, std::vector<const char*> instance_extensions) -> result<void>
    {
        // Create Vulkan Instance
        {
            auto create_info = structs::create::instance();
            auto properties  = enum_instance_ext_props().unwrap();

            // Enable required extensions
            if (is_ext_avail(properties, khr_extensions::device_properties_2))
            {
                instance_extensions.push_back(khr_extensions::device_properties_2);
            }

            if (is_ext_avail(properties, khr_extensions::portability_enumeration))
            {
                instance_extensions.push_back(khr_extensions::portability_enumeration);
                create_info.flags |= instance_create_bits::portability;
            }

            // Enabling validation layers
            constexpr std::array layers     = { validation_layers::validation };
            create_info.enabledLayerCount   = layers.size();
            create_info.ppEnabledLayerNames = layers.data();
            instance_extensions.push_back(extensions::debug_report);

            // Create Vulkan Instance
            create_info.enabledExtensionCount   = (ui32)instance_extensions.size();
            create_info.ppEnabledExtensionNames = instance_extensions.data();

            check(vkCreateInstance(&create_info, handles.allocator, &handles.instance));
        }

        // Setup the debug report callback
        {
            begin_chrono();
            auto create_deb_callback_fn = proc_addresses::create_deb_callback(handles.instance);
            auto debug_report_ci        = structs::create::debug_report_callback();
            debug_report_ci.pfnCallback = debug_report;

            check(create_deb_callback_fn(handles.instance,
                                         &debug_report_ci,
                                         handles.allocator,
                                         &handles.deb_report));

            print_chrono("- Create debug report callback: {}");
        }

        // Select Physical Device (GPU)
        {
            auto gpus = enum_phy_devices(handles.instance).unwrap();

            for (VkPhysicalDevice& device : gpus)
            {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(device, &properties);

                if (properties.deviceType == phy_device_types::discrete)
                {
                    handles.phy_dev = device;
                    break;
                }
            }

            // Use first GPU (Integrated) is a Discrete one is not available.
            if (gpus.size() > 0) { handles.phy_dev = gpus.front(); }
            else { return error_t { "No GPU was found" }; }
        }

        // Select graphics queue family
        {
        ui32 count {};
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
        std::vector<VkQueueFamilyProperties> queues(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queues.data());
            auto properties = enum_queue_family_properties(handles.phy_dev).unwrap();

            for (const auto [i, prop] : flux::enumerate(properties))
            {
                if (prop.queueFlags & queue_families::graphics)
                {
                    handles.queue_fam = i;
                    break;
                }
            }
            orbassert(handles.queue_fam != UINT32_MAX, "Could not retrieve queue family propoerties");
        }

        // Create Logical Device (with 1 queue)
        {
            std::vector<const char*> device_extensions;
            device_extensions.push_back(khr_extensions::swapchain);

            // Enumerate physical device extension
            auto properties = enum_device_extension_properties(handles.phy_dev).unwrap();

            constexpr std::array queue_priority = { 1.0f };

            std::array queue_info { structs::create::device_queue() };
            queue_info[0].queueFamilyIndex = handles.queue_fam;
            queue_info[0].queueCount       = queue_priority.size();
            queue_info[0].pQueuePriorities = queue_priority.data();

            auto create_info                    = structs::create::device();
            create_info.queueCreateInfoCount    = queue_info.size();
            create_info.pQueueCreateInfos       = queue_info.data();
            create_info.enabledExtensionCount   = device_extensions.size();
            create_info.ppEnabledExtensionNames = device_extensions.data();

            check(vkCreateDevice(handles.phy_dev, &create_info, handles.allocator, &handles.dev));
            vkGetDeviceQueue(handles.dev, handles.queue_fam, 0, &handles.queue);
        }

        {
            constexpr std::array<VkDescriptorPoolSize, 1> pool_sizes = {
                { desc_types::combined_image_sampler, 1 },
            };
            auto pool_info          = structs::create::desc_pool();
            pool_info.flags         = desc_types::combined_image_sampler;
            pool_info.maxSets       = 1;
            pool_info.poolSizeCount = pool_sizes.size();
            pool_info.pPoolSizes    = pool_sizes.data();
            check(vkCreateDescriptorPool(handles.dev, &pool_info, handles.allocator, &handles.desc_pool));
        }

        return {};
    }

    inline void cleanup_vk(handles_t& handles)
    {
        vkDestroyDescriptorPool(handles.dev, handles.desc_pool, handles.allocator);

        // Remove the debug report callback
        auto destroy_deb_callback_fn = proc_addresses::destroy_deb_callback(handles.instance);
        destroy_deb_callback_fn(handles.instance, handles.deb_report, handles.allocator);

        vkDestroyDevice(handles.dev, handles.allocator);
        vkDestroyInstance(handles.instance, handles.allocator);
    }

    inline void cleanup_vk_win(handles_t& handles)
    {
        ImGui_ImplVulkanH_DestroyWindow(handles.instance,
                                        handles.dev,
                                        &handles.main_win_data,
                                        handles.allocator);
    }

    inline void frame_render(handles_t& handles, ImDrawData* draw_data)
    {
        VkResult err {};

        VkSemaphore image_acquired_semaphore =
            handles.main_win_data.FrameSemaphores[handles.main_win_data.SemaphoreIndex]
                .ImageAcquiredSemaphore;
        VkSemaphore render_complete_semaphore =
            handles.main_win_data.FrameSemaphores[handles.main_win_data.SemaphoreIndex]
                .RenderCompleteSemaphore;

        err = vkAcquireNextImageKHR(handles.dev,
                                    handles.main_win_data.Swapchain,
                                    UINT64_MAX,
                                    image_acquired_semaphore,
                                    VK_NULL_HANDLE,
                                    &handles.main_win_data.FrameIndex);
        if (err == vkres::err_out_of_date_khr || err == vkres::suboptimal_khr)
        {
            handles.sc_rebuild = true;
            return;
        }

        check(err);

        ImGui_ImplVulkanH_Frame* fd = &handles.main_win_data.Frames[handles.main_win_data.FrameIndex];
        {
            check(vkWaitForFences(handles.dev, 1, &fd->Fence, true, UINT64_MAX));
            check(vkResetFences(handles.dev, 1, &fd->Fence));
        }
        {
            check(vkResetCommandPool(handles.dev, fd->CommandPool, 0));
            auto info = structs::cmd_buffer_begin();

            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            check(vkBeginCommandBuffer(fd->CommandBuffer, &info));
        }
        {
            auto begin_render_pass                     = structs::render_pass_begin();
            begin_render_pass.renderPass               = handles.main_win_data.RenderPass;
            begin_render_pass.framebuffer              = fd->Framebuffer;
            begin_render_pass.renderArea.extent.width  = handles.main_win_data.Width;
            begin_render_pass.renderArea.extent.height = handles.main_win_data.Height;
            begin_render_pass.clearValueCount          = 1;
            begin_render_pass.pClearValues             = &handles.main_win_data.ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer, &begin_render_pass, VK_SUBPASS_CONTENTS_INLINE);
        }

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            constexpr VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            auto submit                 = structs::submit();
            submit.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit.waitSemaphoreCount   = 1;
            submit.pWaitSemaphores      = &image_acquired_semaphore;
            submit.pWaitDstStageMask    = &wait_stage;
            submit.commandBufferCount   = 1;
            submit.pCommandBuffers      = &fd->CommandBuffer;
            submit.signalSemaphoreCount = 1;
            submit.pSignalSemaphores    = &render_complete_semaphore;

            check(vkEndCommandBuffer(fd->CommandBuffer));
            check(vkQueueSubmit(handles.queue, 1, &submit, fd->Fence));
        }
    }

    inline void frame_present(handles_t& handles)
    {
        if (handles.sc_rebuild) { return; }

        VkSemaphore render_complete_semaphore =
            handles.main_win_data.FrameSemaphores[handles.main_win_data.SemaphoreIndex]
                .RenderCompleteSemaphore;

        auto present               = structs::present();
        present.waitSemaphoreCount = 1;
        present.pWaitSemaphores    = &render_complete_semaphore;
        present.swapchainCount     = 1;
        present.pSwapchains        = &handles.main_win_data.Swapchain;
        present.pImageIndices      = &handles.main_win_data.FrameIndex;

        VkResult err = vkQueuePresentKHR(handles.queue, &present);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            handles.sc_rebuild = true;
            return;
        }
        check(err);

        handles.main_win_data.SemaphoreIndex =
            (handles.main_win_data.SemaphoreIndex + 1)
            % handles.main_win_data.SemaphoreCount; // Now we can use the next set of semaphores
    }

    inline auto vk_supported() -> bool
    {
        if constexpr (orb::assert_mode)
        {
            // Disable vulkan check in dev mode.
            // We should know that vulkan is supported while developping.
            // This speeds up window create time by ~1.5s
            return true;
        }

        return glfwVulkanSupported();
    }

    inline auto create_window_surface(handles_t& handles) -> VkSurfaceKHR
    {
        VkSurfaceKHR surface {};
        check(glfwCreateWindowSurface(handles.instance, handles.window, nullptr, &surface));

        return surface;
    }

    inline auto get_fb_size(handles_t& handles) -> std::tuple<int, int>
    {
        int w {}, h {};
        glfwGetFramebufferSize(handles.window, &w, &h);
        return { w, h };
    }

    inline auto sel_win_surface_format(VkPhysicalDevice    pdevice,
                                       VkSurfaceKHR        surface,
                                       std::span<VkFormat> req_formats,
                                       VkColorSpaceKHR     colorspace) -> result<VkSurfaceFormatKHR>
    {
        auto avail_formats = get_phy_device_surface_formats(pdevice, surface).unwrap();

        // First check if only one format, VK_FORMAT_UNDEFINED, is available, which would imply that any
        // format is available
        if (avail_formats.size() == 1)
        {
            auto       format           = avail_formats.back();
            const bool any_format_valid = format.format != VK_FORMAT_UNDEFINED;

            return any_format_valid ? format
                                    : VkSurfaceFormatKHR {
                                          .format     = req_formats.back(),
                                          .colorSpace = colorspace,
                                      };
        }

        // Request several formats, the first found will be used
        for (auto req : req_formats)
        {
            for (auto avail : avail_formats)
            {
                if (colorspace == avail.colorSpace && req == avail.format) { return avail; }
            }
        }

        // If none of the requested image formats could be found, use the first available
        println("None of the request image formats were found. Using another one");
        return avail_formats.front();
    }

    inline auto setup_vk_win(handles_t& handles) -> result<void>
    {
        handles.main_win_data.Surface = handles.surface;
        const auto [width, height]    = get_fb_size(handles);

        // Check for WSI support
        VkBool32 res {};
        vkGetPhysicalDeviceSurfaceSupportKHR(handles.phy_dev,
                                             handles.queue_fam,
                                             handles.main_win_data.Surface,
                                             &res);
        if (res != VK_TRUE) { return error_t { "Error no WSI support on physical device 0" }; }

        // Select Surface Format
        constexpr std::array requestSurfaceImageFormat = {
            VK_FORMAT_B8G8R8A8_UNORM,
            VK_FORMAT_R8G8B8A8_UNORM,
            VK_FORMAT_B8G8R8_UNORM,
            VK_FORMAT_R8G8B8_UNORM,
        };

        constexpr VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        handles.main_win_data.SurfaceFormat =
            ImGui_ImplVulkanH_SelectSurfaceFormat(handles.phy_dev,
                                                  handles.main_win_data.Surface,
                                                  requestSurfaceImageFormat.data(),
                                                  requestSurfaceImageFormat.size(),
                                                  requestSurfaceColorSpace);

        // Select Present Mode
        const std::array present_modes = {
            VK_PRESENT_MODE_MAILBOX_KHR,
            VK_PRESENT_MODE_IMMEDIATE_KHR,
            VK_PRESENT_MODE_FIFO_KHR,
        };
        handles.main_win_data.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(handles.phy_dev,
                                                                                handles.main_win_data.Surface,
                                                                                present_modes.data(),
                                                                                present_modes.size());

        // Create SwapChain, RenderPass, Framebuffer, etc.
        orbassert(handles.min_img_count >= 2, "Swapchain invalid");
        ImGui_ImplVulkanH_CreateOrResizeWindow(handles.instance,
                                               handles.phy_dev,
                                               handles.dev,
                                               &handles.main_win_data,
                                               handles.queue_fam,
                                               handles.allocator,
                                               width,
                                               height,
                                               handles.min_img_count);

        return {};
    }

} // namespace orb::vk::details
