#pragma once

#include "orb/result.hpp"
#include "orb/vk/vk_types.hpp"

#include <vector>

namespace orb::vk
{
    struct images_t
    {
        std::vector<VkImage>       handles;
        std::vector<VmaAllocation> allocations;
    };

    class images_builder_t
    {
    public:
        [[nodiscard]] static auto prepare() -> result<images_builder_t>;
        [[nodiscard]] auto        build() -> result<images_t>;

    private:
    };
} // namespace orb::vk
