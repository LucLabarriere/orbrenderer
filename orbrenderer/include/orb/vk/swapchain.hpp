#pragma once

#include "orb/vk/vk_structs.hpp"
#include "orb/vk/vk_types.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>
#include <orb/utility.hpp>

#include <span>
#include <vector>

namespace orb::glfw
{
    class window_t;
}

namespace orb::vk
{
    struct instance_t;
    struct gpu_t;
    struct device_t;

    struct swapchain_t
    {
        VkSwapchainKHR     handle {};
        VkDevice           device {};
        VkInstance         instance {};
        VkSurfaceKHR       surface {};
        VkSurfaceFormatKHR format {
            .colorSpace = color_spaces::srgb_nonlinear_khr,
        };
        VkCommandBuffer          cmd {};
        VkPresentModeKHR         present_mode {};
        std::vector<VkImage>     images;
        std::vector<VkImageView> views;
        std::vector<VkSemaphore> semaphores;

        ui32 width {};
        ui32 height {};
        ui32 img_count {};
        ui32 min_img_count {};
    };

    class swapchain_builder_t
    {
    public:
        weak<glfw::window_t> window;
        weak<instance_t>     instance;
        weak<gpu_t>          gpu;
        weak<device_t>       device;

        swapchain_t sc {};

        ui32 present_qf_index {};
        ui32 semaphore_count {};

        std::vector<formats::enum_t>       formats;
        std::vector<present_modes::enum_t> present_modes;

        [[nodiscard]] static auto prepare(
            weak<instance_t> instance, weak<gpu_t> gpu, weak<device_t> device, weak<glfw::window_t> window)
            -> result<swapchain_builder_t>;

        auto fb_dimensions(i32 w, i32 h) -> swapchain_builder_t&;
        auto fb_dimensions_from_window() -> swapchain_builder_t&;
        auto present_queue_family_index(ui32 index) -> swapchain_builder_t&;

        auto format(formats::enum_t v) -> swapchain_builder_t&
        {
            formats.push_back(v);
            return *this;
        }
        auto color_space(color_spaces::enum_t v) -> swapchain_builder_t&
        {
            sc.format.colorSpace = v;
            return *this;
        }
        auto present_mode(present_modes::enum_t v) -> swapchain_builder_t&
        {
            present_modes.push_back(v);
            return *this;
        }
        auto min_img_count(ui32 count) -> swapchain_builder_t&
        {
            sc.min_img_count = count;
            return *this;
        }

        auto semaphores(ui32 count) -> swapchain_builder_t&
        {
            semaphore_count = count;
            return *this;
        }

        [[nodiscard]] auto build() -> result<swapchain_t>;
    };

    void copy_to_swapchain(
        swapchain_t& sc, VkCommandBuffer cmd, VkImage src, VkExtent2D src_size, ui32 frame_index);

    struct img_res_t
    {
        std::variant<ui32, vkres::enum_t> content {};

        [[nodiscard]] auto is_error() const -> bool { return content.index() == 1; };
        [[nodiscard]] auto is_valid() const -> bool { return content.index() == 0; };
        [[nodiscard]] auto error() const -> vkres::enum_t { return std::get<vkres::enum_t>(content); }
        [[nodiscard]] auto require_sc_rebuild() const -> bool
        {
            println("is_valid()={} is_error()={}", is_valid(), is_error());
            if (is_valid()) return false;

            const auto res = std::get<vkres::enum_t>(content);
            if (res == vkres::err_out_of_date_khr || res == vkres::suboptimal_khr) { return true; }

            throw orb::exception("Error encountered during vkAcquireNextImageKHR: {}", vkres::get_repr(res));
        }
    };

    [[nodiscard]] auto acquire_next_img(swapchain_t&, VkSemaphore, VkFence, ui64 timeout = UINT64_MAX)
        -> img_res_t;

    [[nodiscard]] auto present_img(swapchain_t&, VkQueue, std::span<VkSemaphore>, ui32 frame_index) -> img_res_t;

    void destroy(swapchain_t& swapchain);
} // namespace orb::vk
