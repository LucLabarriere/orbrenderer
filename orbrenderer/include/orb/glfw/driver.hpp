#pragma once

#include "orb/glfw/window.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <vector>

using VkInstance   = struct VkInstance_T*;
using VkSurfaceKHR = struct VkSurfaceKHR_T*;

namespace orb::glfw
{
    class driver_t
    {
    public:
        driver_t() = default;
        ~driver_t();

        driver_t(const driver_t&)                    = delete;
        auto operator=(const driver_t&) -> driver_t& = delete;

        driver_t(driver_t&&) noexcept;
        auto operator=(driver_t&&) noexcept -> driver_t&;

        static auto create() -> result<box<driver_t>>;

        auto create_window_for_vk() -> result<weak<window_t>>;
        void poll_events();

        void destroy();

    private:
        std::vector<box<window_t>> m_windows;
        bool                       m_initialized = false;
    };
} // namespace orb::glfw
