#pragma once

#include <array>
#include <cstdio>
#include <cstdlib>
#include <orb/assert.hpp>
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
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <vkhelpers.hpp>

// NOLINTNEXTLINE
#define check_res(var_name, func)                       \
    auto var_name##_res = func;                         \
    if (!var_name##_res) return var_name##_res.error(); \
    auto(var_name) = var_name##_res.value();

namespace orb::vk::details
{
    struct handles_t
    {
        VkAllocationCallbacks*   allocator      = nullptr;
        VkInstance               instance       = VK_NULL_HANDLE;
        VkPhysicalDevice         phy_dev        = VK_NULL_HANDLE;
        VkDevice                 dev            = VK_NULL_HANDLE;
        uint32_t                 queue_fam      = (uint32_t)-1;
        VkQueue                  queue          = VK_NULL_HANDLE;
        VkDebugReportCallbackEXT deb_report     = VK_NULL_HANDLE;
        VkPipelineCache          pipeline_cache = VK_NULL_HANDLE;
        VkDescriptorPool         desc_pool      = VK_NULL_HANDLE;
        GLFWwindow*              window         = nullptr;
        VkSurfaceKHR             surface        = nullptr;

        ImGui_ImplVulkanH_Window main_win_data;
        int                      min_img_count = 2;
        bool                     sc_rebuild    = false;
    };

    inline void glfw_error_callback(int error, const char* description)
    {
        orb::println("GLFW Error {}: {}", error, description);
    }

    inline void check_vk_result(VkResult err)
    {
        if (err == 0)
        {
            return;
        }

        panic("Vulkan error: {}", (i64)err);
    }

    inline auto get_req_instance_exts() -> std::vector<const char*>
    {
        // Disable vulkan get required extensions in dev mode and on windows.
        // This speeds vulkan setup time by ~1.2s
        if constexpr (orb::assert_mode && on_windows)
        {
            return {
                "VK_KHR_surface",
                "VK_KHR_win32_surface",
            };
        }
        else
        {
            std::vector<const char*> extensions;
            uint32_t                 extensions_count = 0;
            const char**             glfw_extensions  = glfwGetRequiredInstanceExtensions(&extensions_count);
            for (uint32_t i = 0; i < extensions_count; i++)
            {
                extensions.push_back(glfw_extensions[i]);
            }
            return extensions;
        }
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
        check_res(gpus, enum_phy_devices(handles.instance));

        for (VkPhysicalDevice& device : gpus)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                return device;
            }
        }

        // Use first GPU (Integrated) is a Discrete one is not available.
        if (gpus.size() > 0)
        {
            return gpus[0];
        }

        return error_t { "No GPU was found" };
    }

    inline auto setup_vk(handles_t& handles, std::vector<const char*> instance_extensions) -> result<void>
    {
        // Create Vulkan Instance
        {
            VkInstanceCreateInfo create_info = {};
            create_info.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

            // Enumerate available extensions
            check_res(properties, enum_instance_ext_props());

            // Enable required extensions
            if (is_ext_avail(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
            {
                instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            }

            if (is_ext_avail(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
            {
                instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
                create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            }

            // Enabling validation layers
            const std::array layers         = { "VK_LAYER_KHRONOS_validation" };
            create_info.enabledLayerCount   = layers.size();
            create_info.ppEnabledLayerNames = layers.data();
            instance_extensions.push_back("VK_EXT_debug_report");

            // Create Vulkan Instance
            create_info.enabledExtensionCount   = (ui32)instance_extensions.size();
            create_info.ppEnabledExtensionNames = instance_extensions.data();
            auto err = vkCreateInstance(&create_info, handles.allocator, &handles.instance);
            check_vk_result(err);

            // Setup the debug report callback
            begin_chrono();
            auto f_vkCreateDebugReportCallbackEXT =
                (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(handles.instance, // NOLINT
                                                                          "vkCreateDebugReportCallbackEXT");
            orbassert(f_vkCreateDebugReportCallbackEXT != nullptr,
                      "Coult not retrieve CreateDebugReportCallback function");
            VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
            debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
            debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT
                                  | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
            debug_report_ci.pfnCallback = debug_report;
            debug_report_ci.pUserData   = nullptr;
            err                         = f_vkCreateDebugReportCallbackEXT(handles.instance,
                                                   &debug_report_ci,
                                                   handles.allocator,
                                                   &handles.deb_report);
            check_vk_result(err);
            print_chrono("- Create debug report callback: {}");
        }

        // Select Physical Device (GPU)
        handles.phy_dev = sel_phy_dev(handles).value();

        // Select graphics queue family
        {
            uint32_t count {};
            vkGetPhysicalDeviceQueueFamilyProperties(handles.phy_dev, &count, nullptr);
            std::vector<VkQueueFamilyProperties> queues(count);
            vkGetPhysicalDeviceQueueFamilyProperties(handles.phy_dev, &count, queues.data());

            for (uint32_t i = 0; i < count; i++)
            {
                if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    handles.queue_fam = i;
                    break;
                }
            }
            orbassert(handles.queue_fam != (uint32_t)-1, "Could not retrieve queue family propoerties");
        }

        // Create Logical Device (with 1 queue)
        {
            ImVector<const char*> device_extensions;
            device_extensions.push_back("VK_KHR_swapchain");

            // Enumerate physical device extension
            uint32_t                           properties_count {};
            std::vector<VkExtensionProperties> properties;
            vkEnumerateDeviceExtensionProperties(handles.phy_dev, nullptr, &properties_count, nullptr);
            properties.resize(properties_count);
            vkEnumerateDeviceExtensionProperties(handles.phy_dev,
                                                 nullptr,
                                                 &properties_count,
                                                 properties.data());
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
            if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
            {
                device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
            }
#endif

            std::array                             queue_priority = { 1.0f };
            std::array<VkDeviceQueueCreateInfo, 1> queue_info {};
            queue_info[0].sType                 = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info[0].queueFamilyIndex      = handles.queue_fam;
            queue_info[0].queueCount            = 1;
            queue_info[0].pQueuePriorities      = queue_priority.data();
            VkDeviceCreateInfo create_info      = {};
            create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            create_info.queueCreateInfoCount    = queue_info.size();
            create_info.pQueueCreateInfos       = queue_info.data();
            create_info.enabledExtensionCount   = (uint32_t)device_extensions.Size;
            create_info.ppEnabledExtensionNames = device_extensions.Data;
            auto err = vkCreateDevice(handles.phy_dev, &create_info, handles.allocator, &handles.dev);
            check_vk_result(err);
            vkGetDeviceQueue(handles.dev, handles.queue_fam, 0, &handles.queue);
        }

        // Create Descriptor Pool
        // The example only requires a single combined image sampler descriptor for the font image and only
        // uses one descriptor set (for that) If you wish to load e.g. additional textures you may need to
        // alter pools sizes.
        {
            std::array<VkDescriptorPoolSize, 1> pool_sizes = {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
            };
            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.flags                      = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets                    = 1;
            pool_info.poolSizeCount              = (uint32_t)pool_sizes.size();
            pool_info.pPoolSizes                 = pool_sizes.data();
            auto err = vkCreateDescriptorPool(handles.dev, &pool_info, handles.allocator, &handles.desc_pool);
            check_vk_result(err);
        }

        return {};
    }

    inline void cleanup_vk(handles_t& handles)
    {
        vkDestroyDescriptorPool(handles.dev, handles.desc_pool, handles.allocator);

        // Remove the debug report callback
        auto f_vkDestroyDebugReportCallbackEXT =
            (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(handles.instance, // NOLINT
                                                                       "vkDestroyDebugReportCallbackEXT");
        f_vkDestroyDebugReportCallbackEXT(handles.instance, handles.deb_report, handles.allocator);

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
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            handles.sc_rebuild = true;
            return;
        }
        check_vk_result(err);

        ImGui_ImplVulkanH_Frame* fd = &handles.main_win_data.Frames[handles.main_win_data.FrameIndex];
        {
            err = vkWaitForFences(handles.dev,
                                  1,
                                  &fd->Fence,
                                  VK_TRUE,
                                  UINT64_MAX); // wait indefinitely instead of periodically checking
            check_vk_result(err);

            err = vkResetFences(handles.dev, 1, &fd->Fence);
            check_vk_result(err);
        }
        {
            err = vkResetCommandPool(handles.dev, fd->CommandPool, 0);
            check_vk_result(err);
            VkCommandBufferBeginInfo info = {};
            info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
            check_vk_result(err);
        }
        {
            VkRenderPassBeginInfo info    = {};
            info.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass               = handles.main_win_data.RenderPass;
            info.framebuffer              = fd->Framebuffer;
            info.renderArea.extent.width  = handles.main_win_data.Width;
            info.renderArea.extent.height = handles.main_win_data.Height;
            info.clearValueCount          = 1;
            info.pClearValues             = &handles.main_win_data.ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo         info       = {};
            info.sType                      = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount         = 1;
            info.pWaitSemaphores            = &image_acquired_semaphore;
            info.pWaitDstStageMask          = &wait_stage;
            info.commandBufferCount         = 1;
            info.pCommandBuffers            = &fd->CommandBuffer;
            info.signalSemaphoreCount       = 1;
            info.pSignalSemaphores          = &render_complete_semaphore;

            err = vkEndCommandBuffer(fd->CommandBuffer);
            check_vk_result(err);
            err = vkQueueSubmit(handles.queue, 1, &info, fd->Fence);
            check_vk_result(err);
        }
    }

    inline void frame_present(handles_t& handles)
    {
        if (handles.sc_rebuild)
        {
            return;
        }
        VkSemaphore render_complete_semaphore =
            handles.main_win_data.FrameSemaphores[handles.main_win_data.SemaphoreIndex]
                .RenderCompleteSemaphore;
        VkPresentInfoKHR info   = {};
        info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores    = &render_complete_semaphore;
        info.swapchainCount     = 1;
        info.pSwapchains        = &handles.main_win_data.Swapchain;
        info.pImageIndices      = &handles.main_win_data.FrameIndex;
        VkResult err            = vkQueuePresentKHR(handles.queue, &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            handles.sc_rebuild = true;
            return;
        }
        check_vk_result(err);
        handles.main_win_data.SemaphoreIndex =
            (handles.main_win_data.SemaphoreIndex + 1)
            % handles.main_win_data.SemaphoreCount; // Now we can use the next set of semaphores
    }

    inline auto vk_supported() -> bool
    {
        if constexpr (orb::assert_mode)
        {
            // Disable vulkan check in dev mode. We should now that vulkan is supported while developping.
            // This speeds up window create time by ~1.5s
            return true;
        }
        else
        {
            return glfwVulkanSupported();
        }
    }

    inline auto init_glfw() -> result<void>
    {
        // Initialize GLFW
        begin_chrono();
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
        {
            return error_t { "Coult not initialize GLFW" };
        }
        print_chrono("- GLFW init time {}");

        return {};
    }

    inline auto create_window() -> result<GLFWwindow*>
    {
        begin_chrono();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);

        if (!window)
        {
            return error_t { "Could not create window" };
        }
        print_chrono("- Window create time {}");

        return window;
    }

    inline auto create_window_surface(handles_t& handles) -> result<VkSurfaceKHR>
    {
        VkSurfaceKHR surface {};
        VkResult     err = glfwCreateWindowSurface(handles.instance, handles.window, nullptr, &surface);
        if (err != 0)
        {
            return error_t { "Coult not create window surface: VkError {}" };
        }
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
        check_res(avail_formats, get_phy_device_surface_formats(pdevice, surface));

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
                if (colorspace == avail.colorSpace && req == avail.format)
                {
                    return avail;
                }
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
        if (res != VK_TRUE)
        {
            return error_t { "Error no WSI support on physical device 0" };
        }

        // Select Surface Format
        const std::array requestSurfaceImageFormat = {
            VK_FORMAT_B8G8R8A8_UNORM,
            VK_FORMAT_R8G8B8A8_UNORM,
            VK_FORMAT_B8G8R8_UNORM,
            VK_FORMAT_R8G8B8_UNORM,
        };

        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
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
        // printf("[vulkan] Selected PresentMode = %d\n", app.g_MainWindowData.PresentMode);

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
