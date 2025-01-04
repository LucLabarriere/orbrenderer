#include "orb/vk/images.hpp"

namespace orb::vk
{
    auto images_builder_t::prepare() -> result<images_builder_t>
    {
        images_builder_t b;
        return std::move(b);
    }

    auto images_builder_t::build() -> result<images_t>
    {
        images_t images;
        return images;
    }
} // namespace orb::vk
