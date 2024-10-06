#include "orb/glfw/driver.hpp"
#include "orb/glfw/window.hpp"

#include <orb/print_time.hpp>

#include "glfw/glfw_header.hpp"

// Main code
namespace orb::glfw
{
    inline void glfw_error_callback(int error, const char* description)
    {
        orb::println("GLFW Error {}: {}", error, description);
    }

    auto driver_t::initialize() -> result<void>
    {
        // Initialize GLFW
        begin_chrono();

        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) { return error_t { "Coult not initialize GLFW" }; }

        print_chrono("- GLFW init time {}");

        return {};
    }

    void driver_t::terminate()
    {
        glfwTerminate();
    }
} // namespace orb::glfw
