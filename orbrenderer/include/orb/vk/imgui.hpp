#pragma once

#include "orb/vk/core.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

using VkCommandBuffer = struct VkCommandBuffer_T*;

namespace orb::glfw
{
    class window_t;
}

namespace orb::vk
{
    struct gpu_t;
    struct instance_t;
    struct device_t;
    struct swapchain_t;
    struct render_pass_t;
    struct desc_pool_t;

    struct imgui_driver_t
    {
        bool m_initialized = false;

        imgui_driver_t() = default;
        ~imgui_driver_t();

        imgui_driver_t(imgui_driver_t&& other) noexcept;
        auto operator=(imgui_driver_t&& other) noexcept -> imgui_driver_t&;

        imgui_driver_t(const imgui_driver_t&)                    = delete;
        auto operator=(const imgui_driver_t&) -> imgui_driver_t& = delete;

        void destroy();

        void new_frame();
        void render();
        void submit_render(VkCommandBuffer cmd);
    };

    class imgui_driver_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<glfw::window_t>,
                                          weak<instance_t>,
                                          weak<gpu_t>,
                                          weak<device_t>,
                                          weak<swapchain_t>,
                                          weak<desc_pool_t>,
                                          weak<render_pass_t>)
            -> imgui_driver_builder_t;

        [[nodiscard]] auto build() -> result<imgui_driver_t>;

        auto config_flag(ImGuiConfigFlags flag) -> imgui_driver_builder_t&
        {
            m_flags |= flag;
            return *this;
        }

        auto dark_theme(bool dark) -> imgui_driver_builder_t&
        {
            m_dark_theme = dark;
            return *this;
        }

    private:
        weak<glfw::window_t> m_window;
        weak<instance_t>     m_instance;
        weak<gpu_t>          m_gpu;
        weak<device_t>       m_device;
        weak<swapchain_t>    m_swapchain;
        weak<desc_pool_t>    m_desc_pool;

        ImGuiConfigFlags          m_flags      = ImGuiConfigFlags_None;
        bool                      m_dark_theme = true;
        ImGui_ImplVulkan_InitInfo m_info {};
    };

} // namespace orb::vk
