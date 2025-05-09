#include "orb/glfw/driver.hpp"

#include "glfw/core.hpp"
#include "orb/glfw/window.hpp"

#include <orb/print_time.hpp>

#include <vulkan/vulkan_core.h>

// Main code
namespace
{
    void glfw_error_callback(int error, const char* description)
    {
        fmt::println("GLFW Error {}: {}", error, description);
    }
} // namespace

namespace orb::glfw
{
    driver_t::~driver_t()
    {
        destroy();
    }

    driver_t::driver_t(driver_t&& other) noexcept
    {
        destroy();

        m_initialized = other.m_initialized;
        m_windows     = std::move(other.m_windows);
    }

    auto driver_t::operator=(driver_t&& other) noexcept -> driver_t&
    {
        destroy();

        m_initialized = other.m_initialized;
        m_windows     = std::move(other.m_windows);

        return *this;
    }

    auto driver_t::create() -> result<box<driver_t>>
    {
        // Initialize GLFW
        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit())
        {
            return error_t { "Coult not initialize GLFW" };
        }

        auto driver           = make_box<driver_t>();
        driver->m_initialized = true;

        return driver;
    }

    auto driver_t::create_window_for_vk() -> result<weak<window_t>>
    {
        auto win = make_box<window_t>();

        if (!glfwVulkanSupported())
        {
            return error_t { "Vulkan not supported!" };
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        GLFWwindow* w = glfwCreateWindow(1280, 720, "OrbRenderer", nullptr, nullptr);

        if (!w)
        {
            return error_t { "Could not create window" };
        }

        win->m_handle = w;
        auto weakptr  = win.getmut();

        m_windows.push_back(std::move(win));

        return weakptr;
    }

    void driver_t::poll_events()
    {
        glfwPollEvents();
    }

    void driver_t::destroy()
    {
        if (!m_initialized) return;

        m_windows.clear();
        glfwTerminate();
        m_initialized = false;
    }
} // namespace orb::glfw
