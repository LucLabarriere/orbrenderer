#pragma once

#include "orb/vk/core.hpp"

#include <span>
#include <vector>

namespace orb::vk
{
    struct subpasses_t
    {
        std::vector<VkSubpassDescription> descriptions;
        std::vector<VkSubpassDependency>  dependencies;

        struct subpass_args_t
        {
            pipeline_bind_point bind_point = pipeline_bind_point::graphics;

            std::span<VkAttachmentReference> color_refs;
            std::span<VkAttachmentReference> resolve_refs;
            std::span<VkAttachmentReference> input_refs;
            std::span<VkAttachmentReference> depth_stencil_ref;

            subpass_description_flag flags {};
        };

        void add_subpass(subpass_args_t&& args)
        {
            auto& desc = descriptions.emplace_back();

            desc.flags                = vkenum(args.flags);
            desc.pipelineBindPoint    = vkenum(args.bind_point);
            desc.colorAttachmentCount = args.color_refs.size();

            desc.pColorAttachments       = args.color_refs.data();
            desc.pResolveAttachments     = args.resolve_refs.data();
            desc.pDepthStencilAttachment = args.depth_stencil_ref.data();

            desc.inputAttachmentCount = args.input_refs.size();
            desc.pInputAttachments    = args.input_refs.data();
        }

        struct dependency_args_t
        {
            ui32 src = subpass_external;
            ui32 dst = 0;

            pipeline_stage_flag src_stage = pipeline_stage_flag::color_attachment_output;
            pipeline_stage_flag dst_stage = pipeline_stage_flag::color_attachment_output;

            ui32 src_access = 0;
            access_flag dst_access = access_flag::color_attachment_write;
        };

        void add_dependency(dependency_args_t&& args)
        {
            auto& dep = dependencies.emplace_back();

            dep.srcSubpass = args.src;
            dep.dstSubpass = args.dst;

            dep.srcStageMask = vkenum(args.src_stage);
            dep.dstStageMask = vkenum(args.dst_stage);

            dep.srcAccessMask = args.src_access;
            dep.dstAccessMask = vkenum(args.dst_access);
        }
    };
} // namespace orb::vk
