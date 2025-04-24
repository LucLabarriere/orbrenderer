#pragma once

#include "orb/vk/core.hpp"

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

            sample_count_flag   samples           = sample_count_flag::_1;
            attachment_load_op  load_ops          = attachment_load_op::dont_care;
            attachment_store_op store_ops         = attachment_store_op::store;
            attachment_load_op  stencil_load_ops  = attachment_load_op::dont_care;
            attachment_store_op stencil_store_ops = attachment_store_op::dont_care;
            image_layout        initial_layout    = image_layout::undefined;
            image_layout        final_layout      = image_layout::present_src_khr;
            image_layout        attachment_layout = image_layout::color_attachment_optimal;
        };

        void add(attachment_args_t&& args)
        {
            const ui32 index = static_cast<ui32>(descriptions.size());

            auto& desc = descriptions.emplace_back();
            auto& ref  = references.emplace_back();

            desc.format         = args.img_format;
            desc.samples        = vkenum(args.samples);
            desc.loadOp         = vkenum(args.load_ops);
            desc.storeOp        = vkenum(args.store_ops);
            desc.stencilLoadOp  = vkenum(args.stencil_load_ops);
            desc.stencilStoreOp = vkenum(args.stencil_store_ops);
            desc.initialLayout  = vkenum(args.initial_layout);
            desc.finalLayout    = vkenum(args.final_layout);
            ref.attachment      = index;
            ref.layout          = vkenum(args.attachment_layout);
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
