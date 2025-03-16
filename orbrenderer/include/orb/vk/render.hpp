#pragma once

#include "orb/vk/core.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <span>

namespace orb::vk
{
    struct render_pass_t;
    struct swapchain_t;

    struct render_info_t
    {
        VkDevice               device {};
        VkRenderPass           pass {};
        VkFramebuffer          fb {};
        VkCommandBuffer        cmd {};
        VkQueue                queue {};
        VkFence                wait_fence {};
        std::span<VkSemaphore> wait_semaphores;
        std::span<VkSemaphore> signal_semaphores;
        weak<swapchain_t>      swapchain;
        VkClearValue           clear_color = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
    };

    [[nodiscard]] auto render_begin(render_info_t&) -> result<void>;
    [[nodiscard]] auto render_end(render_info_t&) -> result<void>;

    void describe(const render_info_t& info);
} // namespace orb::vk
