#pragma once

#include "orb/vk/core.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

namespace orb::glfw
{
    class window_t;
}

namespace orb::vk
{
    struct surface_t
    {
        VkSurfaceKHR handle = nullptr;
    };

    class surface_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(VkInstance           instance,
                                          weak<glfw::window_t> window)
            -> surface_builder_t
        {
            surface_builder_t d;

            d.m_instance = instance;
            d.m_window   = window;

            return d;
        }

        [[nodiscard]] auto build() -> result<surface_t>;

    private:
        VkInstance           m_instance = nullptr;
        weak<glfw::window_t> m_window;
    };
} // namespace orb::vk
