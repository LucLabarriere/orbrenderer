#pragma once

#include "orb/vk/vk_types.hpp"
#include <orb/result.hpp>

namespace orb::vk
{
    void transition_layout(
        VkCommandBuffer cmd, VkImage img, image_layouts::enum_t prev, image_layouts::enum_t next);

    void copy_img(
        VkCommandBuffer cmd, VkImage src, VkImage dst, VkExtent2D src_extent);
} // namespace orb::vk
