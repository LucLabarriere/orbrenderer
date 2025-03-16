#include "orb/vk/surface.hpp"

#include "orb/glfw/driver.hpp"

namespace orb::vk
{

    auto surface_builder_t::build() -> result<surface_t>
    {
        auto surface_res = m_window->create_vk_surface(m_instance);

        if (!surface_res)
        {
            return surface_res.error();
        }

        return surface_t {
            .handle = surface_res.value(),
        };
    }
} // namespace orb::vk
