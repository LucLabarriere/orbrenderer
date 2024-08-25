#include <iostream>

#include <orbrenderer/engine.hpp>

auto main() -> int
{
    auto init_res = orb::initialize();
    if (!init_res)
    {
        orb::println("{}", init_res.error().get_message());
        return 1;
    }

    return 0;
}
