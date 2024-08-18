#pragma once

#include "orbrenderer/vkdriver.hpp"

namespace orb
{
    inline void initialize() {
        vk::create_app();
    };
} // namespace orb
