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
} // namespace orb
