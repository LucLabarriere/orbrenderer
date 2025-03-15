#pragma once

#include "orb/vk/attachments.hpp"
#include "orb/vk/subpasses.hpp"
#include "orb/vk/vk_structs.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

namespace orb::vk
{
    struct render_pass_t
    {
        VkRenderPass          handle = nullptr;
        VkDevice              device = nullptr;
        VkRenderPassBeginInfo begin_info {};
        VkClearValue          clear_color { 0.0f, 0.0f, 0.0f, 1.0f };

        render_pass_t() = default;

        render_pass_t(const render_pass_t&)                    = delete;
        auto operator=(const render_pass_t&) -> render_pass_t& = delete;

        render_pass_t(render_pass_t&& other) noexcept
            : handle(other.handle),
              device(other.device),
              begin_info(other.begin_info),
              clear_color(other.clear_color)
        {
            other.handle = nullptr;
            other.device = nullptr;
        }

        auto operator=(render_pass_t&& other) noexcept -> render_pass_t&
        {
            handle      = other.handle;
            device      = other.device;
            begin_info  = other.begin_info;
            clear_color = other.clear_color;

            other.handle = nullptr;
            other.device = nullptr;

            return *this;
        }

        ~render_pass_t()
        {
            destroy();
        }

        void destroy()
        {
            if (!handle) return;
            vkDestroyRenderPass(device, handle, nullptr);
            handle = nullptr;
        }

        void bind_color()
        {
            begin_info.pClearValues = &clear_color;
        }

        void begin(VkCommandBuffer& cmd)
        {
            vkCmdBeginRenderPass(cmd, &begin_info, VK_SUBPASS_CONTENTS_INLINE);
        }

        void end(VkCommandBuffer& cmd)
        {
            vkCmdEndRenderPass(cmd);
        }
    };

    class render_pass_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(VkDevice device) -> result<render_pass_builder_t>
        {
            render_pass_builder_t b;
            b.m_device = device;
            return b;
        }

        [[nodiscard]] auto build(subpasses_t& subpasses, attachments_t& attachments)
            -> result<box<render_pass_t>>
        {
            auto pass         = make_box<render_pass_t>();
            pass->clear_color = m_color;
            pass->device      = m_device;

            VkRenderPassCreateInfo info = structs::create::render_pass();
            info.attachmentCount        = attachments.descriptions.size();
            info.pAttachments           = attachments.descriptions.data();
            info.subpassCount           = subpasses.descriptions.size();
            info.pSubpasses             = subpasses.descriptions.data();
            info.dependencyCount        = subpasses.dependencies.size();
            info.pDependencies          = subpasses.dependencies.data();

            if (auto res = vkCreateRenderPass(m_device, &info, nullptr, &pass->handle); res != vkres::ok)
            {
                return error_t { "Failed to create render pass: {}", vkres::get_repr(res) };
            }

            pass->begin_info                   = structs::render_pass_begin();
            pass->begin_info.renderPass        = pass->handle;
            pass->begin_info.clearValueCount   = 1;
            pass->begin_info.renderArea.offset = { .x = 0, .y = 0 };

            return pass;
        }

        auto clear_color(VkClearValue&& color) -> render_pass_builder_t&
        {
            m_color = color;
            return *this;
        }

    private:
        VkDevice     m_device = nullptr;
        VkClearValue m_color {};
    };
} // namespace orb::vk
