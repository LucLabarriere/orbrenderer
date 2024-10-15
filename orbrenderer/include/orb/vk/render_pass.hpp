#pragma once

#include "orb/vk/vk_structs.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>
#include <vector>

namespace orb::glfw
{
    struct window_t;
}

namespace orb::vk
{
    struct instance_t;
    struct gpu_t;
    struct device_t;
    struct swapchain_t;

    struct render_pass_t
    {
        VkRenderPass                 handle {};
        weak<device_t>               device;
        VkDescriptorPool             desc_pool {};
        VkCommandPool                cmd_pool {};
        std::vector<VkSemaphore>     semaphores;
        std::vector<VkFence>         fences;
        std::vector<VkCommandBuffer> cmds;
        std::vector<VmaAllocation>   mems;
        std::vector<VkImage>         images;
        std::vector<VkImageView>     views;
        std::vector<VkFramebuffer>   fbs;
    };

    class render_pass_builder_t
    {
    public:
        weak<glfw::window_t> window;
        weak<instance_t>     instance;
        weak<gpu_t>          gpu;
        weak<device_t>       device;
        weak<swapchain_t>    swapchain;
        size_t               fence_count {};
        size_t               semaphore_count {};
        size_t               cmd_count {};
        size_t               framebuffer_count {};

        std::vector<VkDescriptorPoolSize> pool_sizes;

        [[nodiscard]] static auto prepare(
            weak<glfw::window_t>, weak<instance_t>, weak<gpu_t>, weak<device_t>, weak<swapchain_t>)
            -> result<render_pass_builder_t>;

        auto desc_pool_size(desc_types::enum_t type, ui32 count) -> render_pass_builder_t&;
        auto semaphores(size_t count) -> render_pass_builder_t&;
        auto fences(size_t count) -> render_pass_builder_t&;
        auto cmds(size_t count) -> render_pass_builder_t&;
        auto images(size_t count) -> render_pass_builder_t&;
        auto framebuffers(size_t count) -> render_pass_builder_t&;

        [[nodiscard]] auto build() -> result<render_pass_t>;

    private:
        render_pass_builder_t() = default;
    };

    void destroy(render_pass_t& pass);
} // namespace orb::vk
