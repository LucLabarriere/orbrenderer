#include "orb/glfw/driver.hpp"
#include "orb/glfw/window.hpp"

#include <orb/print_time.hpp>

#include "glfw/glfw_header.hpp"

// Main code
namespace
{
    void glfw_error_callback(int error, const char* description)
    {
        orb::println("GLFW Error {}: {}", error, description);
    }
} // namespace

namespace orb::glfw
{
    auto driver_t::initialize() -> result<void>
    {
        // Initialize GLFW
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) { return error_t { "Coult not initialize GLFW" }; }

        return {};
    }

    void driver_t::poll_events()
    {
        glfwPollEvents();
    }

    void driver_t::terminate()
    {
        glfwTerminate();
    }
} // namespace orb::glfw
