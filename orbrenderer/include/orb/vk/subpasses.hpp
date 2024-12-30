#pragma once

#include "orb/vk/vk_structs.hpp"

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
            vk::pipeline_bind_points::enum_t bind_point = vk::pipeline_bind_points::graphics;

            std::span<VkAttachmentReference> color_refs;
            std::span<VkAttachmentReference> resolve_refs;
            std::span<VkAttachmentReference> input_refs;
            std::span<VkAttachmentReference> depth_stencil_ref;

            vk::subpass_description_flags::enum_t flags {};
        };

        void add_subpass(subpass_args_t&& args)
        {
            auto& desc = descriptions.emplace_back();

            desc.flags                = args.flags;
            desc.pipelineBindPoint    = args.bind_point;
            desc.colorAttachmentCount = args.color_refs.size();

            desc.pColorAttachments       = args.color_refs.data();
            desc.pResolveAttachments     = args.resolve_refs.data();
            desc.pDepthStencilAttachment = args.depth_stencil_ref.data();

            desc.inputAttachmentCount = args.input_refs.size();
            desc.pInputAttachments    = args.input_refs.data();
        }

        struct dependency_args_t
        {
            ui32 src = vk::subpass_external;
            ui32 dst = 0;

            ui32 src_stage = vk::pipeline_stage_flags::color_attachment_output;
            ui32 dst_stage = vk::pipeline_stage_flags::color_attachment_output;

            ui32 src_access = 0;
            ui32 dst_access = vk::access_flags::color_attachment_write;
        };

        void add_dependency(dependency_args_t&& args)
        {
            auto& dep = dependencies.emplace_back();

            dep.srcSubpass = args.src;
            dep.dstSubpass = args.dst;

            dep.srcStageMask = args.src_stage;
            dep.dstStageMask = args.dst_stage;

            dep.srcAccessMask = args.src_access;
            dep.dstAccessMask = args.dst_access;
        }
    };
} // namespace orb::vk
