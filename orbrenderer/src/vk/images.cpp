#include "orb/vk/images.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/vk_structs.hpp"

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
        b.m_info.format        = vk::formats::b8g8r8a8_srgb;
        b.m_info.tiling        = vk::image_tilings::optimal;
        b.m_info.initialLayout = vk::image_layouts::undefined;
        b.m_info.usage         = vk::image_usage_flags::color_attachment;
        b.m_info.samples       = vk::sample_count_flags::_1;
        b.m_info.sharingMode   = vk::sharing_modes::exclusive;

        b.m_alloc_info.usage = vk::memory_usages::automatic;
        b.m_alloc_info.flags = vk::memory_flags::dedicated_memory;

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
        VkCommandBuffer cmd, VkImage img, image_layouts::enum_t prev, image_layouts::enum_t next)
    {
        VkImageMemoryBarrier barrier {
            .sType         = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext         = nullptr,
            .srcAccessMask = vk::access_flags::memory_write,
            .dstAccessMask = vk::access_flags::memory_read | vk::access_flags::memory_write,
            .oldLayout     = prev,
            .newLayout     = next,
            .image         = img,
        };

        VkImageAspectFlags aspect = (next == vk::image_layouts::depth_attachment_optimal)
                                      ? vk::image_aspect_flags::depth
                                      : vk::image_aspect_flags::color;

        barrier.subresourceRange = {
            .aspectMask     = aspect,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        vkCmdPipelineBarrier(cmd,
                             vk::pipeline_stage_flags::top_of_pipe,
                             vk::pipeline_stage_flags::top_of_pipe,
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
            .aspectMask     = image_aspect_flags::color,
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
            .aspectMask     = image_aspect_flags::color,
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
            .aspectMask     = image_aspect_flags::color,
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        region.dstSubresource = {
            .aspectMask     = image_aspect_flags::color,
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };

        vkCmdCopyImage(cmd,
                       src,
                       image_layouts::transfer_src_optimal,
                       dst,
                       image_layouts::transfer_dst_optimal,
                       1,
                       &region);
    } // namespace orb::vk

    void destroy(images_t& images)
    {
        for (const auto& [img, alloc] : flux::zip_all_mut(images.handles, images.allocations))
        {
            vmaDestroyImage(images.allocator, img, alloc);
        }
    }

} // namespace orb::vk
