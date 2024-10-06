#pragma once

#include "orb/vk/vk_types.hpp"

#include <vulkan/vulkan_core.h>

namespace orb::vk::structs
{
    namespace create
    {
        [[nodiscard]] inline auto instance() -> VkInstanceCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext = nullptr,
            };
        }

        [[nodiscard]] inline auto debug_report_callback() -> VkDebugReportCallbackCreateInfoEXT
        {
            return {
                .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
                .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT   //
                       | VK_DEBUG_REPORT_WARNING_BIT_EXT //
                       | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
                .pUserData = nullptr,
            };
        }

        [[nodiscard]] inline auto device_queue() -> VkDeviceQueueCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            };
        }

        [[nodiscard]] inline auto device() -> VkDeviceCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            };
        }

        [[nodiscard]] inline auto desc_pool() -> VkDescriptorPoolCreateInfo
        {
            return {
                .sType   = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                .maxSets = 1,
            };
        }

        [[nodiscard]] inline auto swapchain() -> VkSwapchainCreateInfoKHR
        {
            return {
                .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            };
        }

        [[nodiscard]] inline auto image_view() -> VkImageViewCreateInfo
        {
            return {
                .sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .viewType     = image_view_types::type_2d,
                .components = {
                    .r = component_swizzles::r,
                    .g = component_swizzles::g,
                    .b = component_swizzles::b,
                    .a = component_swizzles::a,
                },
                .subresourceRange = { image_aspect_flag::color, 0, 1, 0, 1 },
            };
        }

    } // namespace create

    [[nodiscard]] inline auto cmd_buffer_begin() -> VkCommandBufferBeginInfo
    {
        return {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        };
    }

    [[nodiscard]] inline auto render_pass_begin() -> VkRenderPassBeginInfo
    {
        return {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        };
    }

    [[nodiscard]] inline auto submit() -> VkSubmitInfo
    {
        return {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        };
    }

    [[nodiscard]] inline auto present() -> VkPresentInfoKHR
    {
        return {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        };
    }
} // namespace orb::vk::structs
