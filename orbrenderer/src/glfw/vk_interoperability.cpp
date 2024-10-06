#include "orb/glfw.hpp"
#include "orb/glfw/window.hpp"

#include "glfw/glfw_header.hpp"

#include <orb/print_time.hpp>

#include <vulkan/vulkan_core.h>

namespace orb::glfw
{
    auto driver_t::create_window_for_vk() -> result<window_t>
    {
        begin_chrono();
        window_t win;

        if (!glfwVulkanSupported()) { return error_t { "Vulkan not supported!" }; }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* w = glfwCreateWindow(1280, 720, "OrbRenderer", nullptr, nullptr);

        if (!w) { return error_t { "Could not create window" }; }

        print_chrono("- Window create time {}");

        win.m_handle = w;
        return win;
    }

    auto driver_t::create_vk_surface(VkInstance instance, window_t& win) -> result<VkSurfaceKHR>
    {
        VkSurfaceKHR surface {};
        const auto   res = glfwCreateWindowSurface(instance, win.get_handle<GLFWwindow>(), nullptr, &surface);
        if (res != VK_SUCCESS) { return error_t { "Could not create the vulkan surface: {}", (i64)res }; }
        return surface;
    }

} // namespace orb::glfw
