#include "orb/vk/render_pass.hpp"

namespace orb::vk
{
    auto render_pass_builder_t::prepare(VkDevice device) -> result<render_pass_builder_t>
    {
        render_pass_builder_t b;
        b.m_device = device;
        return b;
    }

    auto render_pass_builder_t::build(subpasses_t&   subpasses,
                                      attachments_t& attachments) -> result<render_pass_t>
    {
        render_pass_t pass;
        pass.clear_color = m_color;

        pass.device = m_device;

        VkRenderPassCreateInfo info = structs::create::render_pass();
        info.attachmentCount        = attachments.descriptions.size();
        info.pAttachments           = attachments.descriptions.data();
        info.subpassCount           = subpasses.descriptions.size();
        info.pSubpasses             = subpasses.descriptions.data();
        info.dependencyCount        = subpasses.dependencies.size();
        info.pDependencies          = subpasses.dependencies.data();

        if (auto res = vkCreateRenderPass(m_device, &info, nullptr, &pass.handle); res != vkres::ok)
        {
            return error_t { "Failed to create render pass: {}", vkres::get_repr(res) };
        }

        pass.begin_info                   = structs::render_pass_begin();
        pass.begin_info.renderPass        = pass.handle;
        pass.begin_info.clearValueCount   = 1;
        pass.begin_info.renderArea.offset = { .x = 0, .y = 0 };

        return pass;
    }

    void begin(render_pass_t& pass, VkCommandBuffer& cmd)
    {
        vkCmdBeginRenderPass(cmd, &pass.begin_info, VK_SUBPASS_CONTENTS_INLINE);
    }

    void end(render_pass_t& /*pass*/, VkCommandBuffer& cmd)
    {
        vkCmdEndRenderPass(cmd);
    }

    void destroy(render_pass_t& pass)
    {
        vkDestroyRenderPass(pass.device, pass.handle, nullptr);
    }
} // namespace orb::vk
