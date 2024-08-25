#pragma once

#include "orbrenderer/vkdriver.hpp"

namespace orb
{
    [[nodiscard]] inline auto initialize() -> result<void>
    {
        auto res = vk::driver::create();
        if (!res)
        {
            return res.error();
        }

        auto vkdriver = res.value();

        vkdriver.loop();
        vkdriver.terminate();

        return {};
    };
} // namespace orb
