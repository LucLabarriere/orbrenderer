#pragma once

#include "orb/vk/core.hpp"

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
    struct surface_t;

    struct swapchain_t
    {
        VkSwapchainCreateInfoKHR info = structs::create::swapchain();
        VkSurfaceCapabilitiesKHR cap {};
        weak<device_t>           device {};
        weak<gpu_t>              gpu {};
        weak<glfw::window_t>     window {};
        VkSwapchainKHR           handle {};
        VkSurfaceKHR             surface {};
        VkInstance               instance {};
        VkSurfaceFormatKHR       format { .colorSpace = vkenum(color_space::srgb_nonlinear_khr) };
        VkCommandBuffer          cmd {};
        VkPresentModeKHR         present_mode {};
        VkExtent2D               extent {};
        std::vector<VkImage>     images;

        ui32 width {};
        ui32 height {};
        ui32 img_count {};

        swapchain_t() = default;

        swapchain_t(const swapchain_t&)                    = delete;
        auto operator=(const swapchain_t&) -> swapchain_t& = delete;

        swapchain_t(swapchain_t&& other) noexcept
        {
            if (handle or surface) destroy();

            info         = other.info;
            cap          = other.cap;
            device       = other.device;
            gpu          = other.gpu;
            window       = other.window;
            handle       = other.handle;
            surface      = other.surface;
            instance     = other.instance;
            format       = other.format;
            cmd          = other.cmd;
            present_mode = other.present_mode;
            extent       = other.extent;
            images       = std::move(other.images);

            other.handle  = nullptr;
            other.surface = nullptr;
        }

        auto operator=(swapchain_t&& other) noexcept -> swapchain_t&
        {
            destroy();

            info         = other.info;
            cap          = other.cap;
            device       = other.device;
            gpu          = other.gpu;
            window       = other.window;
            handle       = other.handle;
            surface      = other.surface;
            instance     = other.instance;
            format       = other.format;
            cmd          = other.cmd;
            present_mode = other.present_mode;
            extent       = other.extent;
            images       = std::move(other.images);

            other.handle  = nullptr;
            other.surface = nullptr;

            return *this;
        }

        ~swapchain_t()
        {
            destroy();
        }

        void destroy();

        auto rebuild() -> result<void>;
    };

    class swapchain_builder_t
    {
    public:
        weak<glfw::window_t> window;
        weak<instance_t>     instance;
        weak<gpu_t>          gpu;
        weak<device_t>       device;
        weak<surface_t>      surface;

        box<swapchain_t> sc {};

        ui32 present_qf_index {};
        ui32 semaphore_count {};

        std::vector<format>       formats;
        std::vector<present_mode> present_modes;

        [[nodiscard]] static auto prepare(weak<instance_t>     instance,
                                          weak<gpu_t>          gpu,
                                          weak<device_t>       device,
                                          weak<glfw::window_t> window,
                                          weak<surface_t>      surface)
            -> result<swapchain_builder_t>;

        auto fb_dimensions(i32 w, i32 h) -> swapchain_builder_t&;
        auto fb_dimensions_from_window() -> swapchain_builder_t&;
        auto present_queue_family_index(ui32 index) -> swapchain_builder_t&;

        auto format(format v) -> swapchain_builder_t&
        {
            formats.push_back(v);
            return *this;
        }
        auto color_space(color_space v) -> swapchain_builder_t&
        {
            sc->format.colorSpace = vkenum(v);
            return *this;
        }
        auto present_mode(present_mode v) -> swapchain_builder_t&
        {
            present_modes.push_back(v);
            return *this;
        }
        auto min_img_count(ui32 count) -> swapchain_builder_t&
        {
            sc->info.minImageCount = count;
            return *this;
        }
        auto usage(image_usage_flag flag) -> swapchain_builder_t&
        {
            sc->info.imageUsage |= vkenum(flag);
            return *this;
        }

        [[nodiscard]] auto build() -> result<box<swapchain_t>>;
    };

    void copy_to_swapchain(swapchain_t&    sc,
                           VkCommandBuffer cmd,
                           VkImage         src,
                           VkExtent2D      src_size,
                           ui32            frame_index);

    struct img_res_t
    {
        std::variant<ui32, vkres::enum_t> content {};

        [[nodiscard]] auto img_index() const -> ui32 { return std::get<ui32>(content); };
        [[nodiscard]] auto is_error() const -> bool { return content.index() == 1; };
        [[nodiscard]] auto is_valid() const -> bool { return content.index() == 0; };
        [[nodiscard]] auto error() const -> vkres::enum_t { return std::get<vkres::enum_t>(content); }
        [[nodiscard]] auto require_sc_rebuild() const -> bool
        {
            if (is_valid()) return false;

            const auto res = std::get<vkres::enum_t>(content);

            if (res == vkres::err_out_of_date_khr || res == vkres::suboptimal_khr) { return true; }

            throw orb::exception("Error encountered during vkAcquireNextImageKHR: {}", vkres::get_repr(res));
        }
    };

    [[nodiscard]] auto acquire_img(swapchain_t&,
                                   VkSemaphore  = VK_NULL_HANDLE,
                                   VkFence      = VK_NULL_HANDLE,
                                   ui64 timeout = UINT64_MAX) -> img_res_t;

    [[nodiscard]] auto present_img(swapchain_t&,
                                   VkQueue,
                                   std::span<VkSemaphore>,
                                   ui32 frame_index) -> img_res_t;

    class present_helper_t
    {
    public:
        [[nodiscard]] static auto prepare() -> present_helper_t { return {}; }

        auto swapchain(swapchain_t& swapchain) -> present_helper_t&
        {
            m_info.pSwapchains    = &swapchain.handle;
            m_info.swapchainCount = 1;
            return *this;
        }

        auto img_index(ui32& index) -> present_helper_t&
        {
            m_info.pImageIndices  = &index;
            m_info.swapchainCount = 1;
            return *this;
        }

        auto wait_semaphores(std::span<VkSemaphore> semaphores) -> present_helper_t&
        {
            m_info.pWaitSemaphores    = semaphores.data();
            m_info.waitSemaphoreCount = semaphores.size();
            return *this;
        }

        auto wait_semaphore(VkSemaphore& semaphore) -> present_helper_t&
        {
            m_info.pWaitSemaphores    = &semaphore;
            m_info.waitSemaphoreCount = 1;
            return *this;
        }

        auto present(VkQueue queue) -> img_res_t
        {
            img_res_t  res;
            const auto r = vkQueuePresentKHR(queue, &m_info);

            if (r == vkres::ok)
            {
                res.content.emplace<0>();
                return res;
            }

            res.content.emplace<1>(r);
            return res;
        }

    private:
        VkPresentInfoKHR m_info = structs::present();
    };
} // namespace orb::vk
