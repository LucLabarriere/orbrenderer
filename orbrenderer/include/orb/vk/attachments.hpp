#pragma once

#include "orb/vk/vk_structs.hpp"
#include "orb/vk/vk_types.hpp"

#include <span>
#include <vector>

namespace orb::vk
{
    struct attachment_t
    {
        VkAttachmentDescription desc {};
        VkAttachmentReference   ref {};
    };

    struct attachments_t
    {
        std::vector<VkAttachmentDescription> descriptions;
        std::vector<VkAttachmentReference>   references;

        struct attachment_args_t
        {
            VkFormat img_format {};

            vk::sample_count_flags::enum_t   samples           = vk::sample_count_flags::_1;
            vk::attachment_load_ops::enum_t  load_ops          = vk::attachment_load_ops::dont_care;
            vk::attachment_store_ops::enum_t store_ops         = vk::attachment_store_ops::store;
            vk::attachment_load_ops::enum_t  stencil_load_ops  = vk::attachment_load_ops::dont_care;
            vk::attachment_store_ops::enum_t stencil_store_ops = vk::attachment_store_ops::dont_care;
            vk::image_layouts::enum_t        initial_layout    = vk::image_layouts::undefined;
            vk::image_layouts::enum_t        final_layout      = vk::image_layouts::present_src_khr;
            vk::image_layouts::enum_t        attachment_layout = vk::image_layouts::color_attachment_optimal;
        };

        void add(attachment_args_t&& args)
        {
            const ui32 index = static_cast<ui32>(descriptions.size());

            auto& desc = descriptions.emplace_back();
            auto& ref  = references.emplace_back();

            desc.format         = args.img_format;
            desc.samples        = args.samples;
            desc.loadOp         = args.load_ops;
            desc.storeOp        = args.store_ops;
            desc.stencilLoadOp  = args.stencil_load_ops;
            desc.stencilStoreOp = args.stencil_store_ops;
            desc.initialLayout  = args.initial_layout;
            desc.finalLayout    = args.final_layout;
            ref.attachment      = index;
            ref.layout          = args.attachment_layout;
        }

        auto spans(size_t offset, size_t count) -> std::tuple<std::span<VkAttachmentDescription>,
                                                              std::span<VkAttachmentReference>>
        {
            return {
                std::span { descriptions }.subspan(offset, count),
                std::span { references }.subspan(offset, count),
            };
        }
    };
} // namespace orb::vk
