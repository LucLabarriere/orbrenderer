#include "orb/glfw/window.hpp"

#include "glfw/core.hpp"
#include "orb/vk/core.hpp"

namespace orb::glfw
{
    window_t::~window_t()
    {
        destroy();
    }

    window_t::window_t(window_t&& other) noexcept
    {
        destroy();

        m_handle       = other.m_handle;
        other.m_handle = nullptr;
    }

    auto window_t::operator=(window_t&& other) noexcept -> window_t&
    {
        destroy();

        m_handle       = other.m_handle;
        other.m_handle = nullptr;

        return *this;
    }

    auto window_t::create_vk_surface(VkInstance instance) -> result<VkSurfaceKHR>
    {
        VkSurfaceKHR surface {};

        const auto res = glfwCreateWindowSurface(instance, get_handle<GLFWwindow>(), nullptr, &surface);

        if (res != vk::vkres::ok)
        {
            return error_t { "Could not create the vulkan surface: {}", vk::vkres::get_repr(res) };
        }

        return surface;
    }

    void window_t::destroy()
    {
        if (!m_handle) return;

        glfwDestroyWindow(static_cast<GLFWwindow*>(m_handle));
    }

    auto window_t::get_fb_dimensions() const -> fb_dimensions_t
    {
        fb_dimensions_t dims;
        glfwGetFramebufferSize(static_cast<GLFWwindow*>(m_handle), &dims.w, &dims.h);
        return dims;
    }

    auto window_t::should_close() const -> bool
    {
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
    }

    auto window_t::minimized() const -> bool
    {
        const auto dims = get_fb_dimensions();
        return dims.w == 0 || dims.h == 0;
    }
} // namespace orb::glfw
