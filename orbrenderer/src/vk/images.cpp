#include "orb/vk/images.hpp"

#include "orb/vk/device.hpp"

#include <orb/flux.hpp>

namespace orb::vk
{
    auto images_builder_t::prepare(VmaAllocator allocator) -> result<images_builder_t>
    {
        images_builder_t b;

        b.m_allocator = allocator;

        b.m_info.extent.width  = 800;
        b.m_info.extent.height = 600;
        b.m_info.extent.depth  = 1;
        b.m_info.mipLevels     = 1;
        b.m_info.arrayLayers   = 1;
        b.m_info.format        = vkenum(format::b8g8r8a8_srgb);
        b.m_info.tiling        = vkenum(image_tiling::optimal);
        b.m_info.initialLayout = vkenum(image_layout::undefined);
        b.m_info.usage         = vkenum(image_usage_flag::color_attachment);
        b.m_info.samples       = vkenum(sample_count_flag::_1);
        b.m_info.sharingMode   = vkenum(sharing_mode::exclusive);

        b.m_alloc_info.usage = vkenum(memory_usage::usage_auto);
        b.m_alloc_info.flags = vkenum(memory_flag::dedicated_memory);

        return std::move(b);
    }

    auto images_builder_t::build() -> result<images_t>
    {
        images_t images;
        images.allocator = m_allocator;
        images.handles.resize(m_count);
        images.allocations.resize(m_count);

        for (const auto& [img, alloc] : flux::zip_all_mut(images.handles, images.allocations))
        {
            vmaCreateImage(m_allocator, &m_info, &m_alloc_info, &img, &alloc, nullptr);
        }

        return images;
    }
    void transition_layout(
        VkCommandBuffer cmd, VkImage img, image_layout prev, image_layout next)
    {
        VkImageMemoryBarrier barrier {
            .sType         = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext         = nullptr,
            .srcAccessMask = vkflag(access_flag::memory_write),
            .dstAccessMask = vkflag(access_flag::memory_read | access_flag::memory_write),
            .oldLayout     = vkenum(prev),
            .newLayout     = vkenum(next),
            .image         = img,
        };

        auto aspect = (next == image_layout::depth_attachment_optimal)
                                      ? image_aspect_flag::depth
                                      : image_aspect_flag::color;

        barrier.subresourceRange = {
            .aspectMask     = vkflag(aspect),
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        vkCmdPipelineBarrier(cmd,
                             vkenum(pipeline_stage_flag::top_of_pipe),
                             vkenum(pipeline_stage_flag::top_of_pipe),
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrier);
    }

    void copy_img(VkCommandBuffer cmd, VkImage src, VkImage dst, VkExtent2D src_extent)
    {
        VkImageCopy region {};

        region.srcSubresource = {
            .aspectMask     = vkflag(image_aspect_flag::color),
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        region.srcOffset = {
            .x = 0,
            .y = 0,
            .z = 0,
        };

        region.dstSubresource = {
            .aspectMask     = vkflag(image_aspect_flag::color),
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        region.dstOffset = {
            .x = 0,
            .y = 0,
            .z = 0,
        };

        region.extent = {
            .width  = src_extent.width,
            .height = src_extent.height,
            .depth  = 1,
        };

        region.srcSubresource = {
            .aspectMask     = vkflag(image_aspect_flag::color),
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        region.dstSubresource = {
            .aspectMask     = vkflag(image_aspect_flag::color),
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        vkCmdCopyImage(cmd,
                       src,
                       vkenum(image_layout::transfer_src_optimal),
                       dst,
                       vkenum(image_layout::transfer_dst_optimal),
                       1,
                       &region);
    } // namespace orb::vk
} // namespace orb::vk
