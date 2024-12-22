#include "orb/vk/image.hpp"

namespace orb::vk
{
    void transition_layout(
        VkCommandBuffer cmd, VkImage img, image_layouts::enum_t prev, image_layouts::enum_t next)
    {
        VkImageMemoryBarrier barrier {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        };

        barrier.pNext         = nullptr;
        barrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
        // barrier.srcStageMask  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        // barrier.dstStageMask  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT;

        barrier.oldLayout = prev;
        barrier.newLayout = next;

        VkImageAspectFlags aspectMask = (next == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
                                          ? VK_IMAGE_ASPECT_DEPTH_BIT
                                          : VK_IMAGE_ASPECT_COLOR_BIT;

        barrier.subresourceRange = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };
        barrier.image = img;

        vkCmdPipelineBarrier(cmd,
                             VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
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
        VkImageCopy region {
            .srcSubresource = {
                .aspectMask = image_aspect_flags::color,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
            .srcOffset = {0, 0,0 },
            .dstSubresource = {
                .aspectMask = image_aspect_flags::color,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
            .dstOffset = {0, 0,0 },
            .extent = {src_extent.width, src_extent.height, 1},
        };

        region.srcSubresource.aspectMask     = image_aspect_flags::color;
        region.srcSubresource.baseArrayLayer = 0;
        region.srcSubresource.layerCount     = 1;
        region.srcSubresource.mipLevel       = 0;

        region.dstSubresource.aspectMask     = image_aspect_flags::color;
        region.dstSubresource.baseArrayLayer = 0;
        region.dstSubresource.layerCount     = 1;
        region.dstSubresource.mipLevel       = 0;

        vkCmdCopyImage(cmd,
                       src,
                       image_layouts::transfer_src_optimal,
                       dst,
                       image_layouts::transfer_dst_optimal,
                       1,
                       &region);
    }
} // namespace orb::vk
