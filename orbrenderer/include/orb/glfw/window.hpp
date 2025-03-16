#pragma once

#include <orb/box.hpp>
#include <orb/result.hpp>

using VkInstance   = struct VkInstance_T*;
using VkSurfaceKHR = struct VkSurfaceKHR_T*;

namespace orb::glfw
{
    class driver_t;

    class window_t
    {
    public:
        window_t() = default;
        ~window_t();

        window_t(window_t&&) noexcept;
        auto operator=(window_t&&) noexcept -> window_t&;

        window_t(const window_t&)            = delete;
        auto operator=(const window_t&) -> window_t& = delete;

        struct fb_dimensions_t
        {
            int w = 0;
            int h = 0;
        };

        auto create_vk_surface(VkInstance) -> result<VkSurfaceKHR>;

        void destroy();

        [[nodiscard]] auto get_handle() const -> const void* { return m_handle; }
        [[nodiscard]] auto get_handle() -> void* { return m_handle; }

        template <typename T>
        [[nodiscard]] auto get_handle() const -> const T*
        {
            return static_cast<const T*>(m_handle);
        }

        template <typename T>
        [[nodiscard]] auto get_handle() -> T*
        {
            return static_cast<T*>(m_handle);
        }

        [[nodiscard]] auto get_fb_dimensions() const -> fb_dimensions_t;
        [[nodiscard]] auto should_close() const -> bool;
        [[nodiscard]] auto minimized() const -> bool;

    private:
        friend glfw::driver_t;

        void* m_handle = nullptr;
    };
} // namespace orb::glfw
