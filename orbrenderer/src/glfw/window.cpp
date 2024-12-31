#include "orb/glfw/window.hpp"

#include "glfw/glfw_header.hpp"

namespace orb::glfw
{
    void window_t::destroy()
    {
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
