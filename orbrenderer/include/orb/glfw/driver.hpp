#pragma once

#include <orb/result.hpp>

#include "orb/glfw/window.hpp"

using VkInstance = struct VkInstance_T*;
using VkSurfaceKHR = struct VkSurfaceKHR_T*;

namespace orb::glfw
{
    class driver_t
    {
    public:
        static auto initialize() -> result<void>;
        static auto create_window_for_vk() -> result<window_t>;
        static auto create_vk_surface(VkInstance, window_t&) -> result<VkSurfaceKHR>;
        static void terminate();
    };
} // namespace orb::vk
