#pragma once

#include "orb/vk/vk_types.hpp"
#include "orb/vk/vma.hpp"

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
                .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT
                       | VK_DEBUG_REPORT_WARNING_BIT_EXT
                       | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
                .pUserData = nullptr,
            };
        }

        [[nodiscard]] inline auto debug_utils() -> VkDebugUtilsMessengerCreateInfoEXT
        {
            return {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,

                .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,

                .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

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

        [[nodiscard]] inline auto image() -> VkImageCreateInfo
        {
            return {
                .sType     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .imageType = image_types::_2d,
            };
        }

        [[nodiscard]] inline auto image_view() -> VkImageViewCreateInfo
        {
            return {
                .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .viewType   = image_view_types::type_2d,
                .components = {
                               .r = component_swizzles::r,
                               .g = component_swizzles::g,
                               .b = component_swizzles::b,
                               .a = component_swizzles::a,
                               },
                .subresourceRange = { image_aspect_flags::color, 0, 1, 0, 1 },
            };
        }

        [[nodiscard]] inline auto framebuffer() -> VkFramebufferCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            };
        }

        [[nodiscard]] inline auto descriptor_pool() -> VkDescriptorPoolCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            };
        }

        [[nodiscard]] inline auto render_pass() -> VkRenderPassCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            };
        }

        [[nodiscard]] inline auto semaphore() -> VkSemaphoreCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            };
        }

        [[nodiscard]] inline auto fence() -> VkFenceCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                .flags = VK_FENCE_CREATE_SIGNALED_BIT,
            };
        }

        [[nodiscard]] inline auto cmd_buffer() -> VkCommandBufferAllocateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            };
        }

        [[nodiscard]] inline auto cmd_pool() -> VkCommandPoolCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            };
        }

        [[nodiscard]] inline auto allocator() -> VmaAllocatorCreateInfo
        {
            return {};
        }

        [[nodiscard]] inline auto allocation() -> VmaAllocationCreateInfo
        {
            return {};
        }

        [[nodiscard]] inline auto shader_module() -> VkShaderModuleCreateInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            };
        }
    } // namespace create

    [[nodiscard]] inline auto cmd_buffer_begin() -> VkCommandBufferBeginInfo
    {
        return {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        };
    }

    [[nodiscard]] inline auto one_time_cmd_buffer_begin() -> VkCommandBufferBeginInfo
    {
        return {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
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
