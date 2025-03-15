#pragma once

#include "orb/vk/device.hpp"
#include "orb/vk/vk_structs.hpp"

#include <orb/box.hpp>
#include <orb/flux.hpp>
#include <orb/result.hpp>

#include <vector>

namespace orb::vk
{
    struct device_t;

    struct framebuffers_t
    {
        std::vector<VkFramebuffer> handles;

        VkDevice device = nullptr;

        framebuffers_t() = default;

        framebuffers_t(const framebuffers_t&)                    = delete;
        auto operator=(const framebuffers_t&) -> framebuffers_t& = delete;

        framebuffers_t(framebuffers_t&& other) noexcept
        {
            destroy();

            handles = std::move(other.handles);
            device  = other.device;
        }

        auto operator=(framebuffers_t&& other) noexcept -> framebuffers_t&
        {
            destroy();

            handles = std::move(other.handles);
            device  = other.device;

            return *this;
        }

        ~framebuffers_t()
        {
            destroy();
        }

        void destroy()
        {
            for (auto& fb : handles)
            {
                if (!fb) continue;

                vkDestroyFramebuffer(device, fb, nullptr);
                fb = nullptr;
            }
        }
    };

    class framebuffers_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device, VkRenderPass pass)
            -> result<framebuffers_builder_t>
        {
            framebuffers_builder_t d;
            d.m_device = device;
            d.m_pass   = pass;
            return d;
        }

        [[nodiscard]] auto build() -> result<framebuffers_t>
        {
            framebuffers_t fbs;
            fbs.device = m_device->handle;

            const size_t fb_count         = m_count != 0
                                              ? m_count
                                              : m_attachments.size();
            const size_t attachment_count = m_attachments.size() / fb_count;

            if (m_attachments.size() % fb_count != 0)
            {
                return error_t {
                    "Attachment count ({}) is invalid, it should a multiple of frame buffer count ({})",
                    attachment_count,
                    fb_count,
                };
            }

            fbs.handles.resize(fb_count);
            auto info = structs::create::framebuffer();

            for (auto [i, fb] : flux::enumerate_mut(fbs.handles))
            {
                info.renderPass      = m_pass;
                info.attachmentCount = attachment_count;
                info.pAttachments    = m_attachments.data() + i * attachment_count;
                info.width           = m_width;
                info.height          = m_height;
                info.layers          = 1;

                if (auto res = vkCreateFramebuffer(m_device->handle, &info, nullptr, &fb); res != vkres::ok)
                {
                    return error_t { "Could not create framebuffer {} : {}", i, (ui32)res };
                }
            }

            return fbs;
        }

        auto attachment(VkImageView view) -> framebuffers_builder_t&
        {
            m_attachments.push_back(view);
            return *this;
        }

        auto count(size_t count) -> framebuffers_builder_t&
        {
            m_count = count;
            return *this;
        }

        auto size(ui32 w, ui32 h) -> framebuffers_builder_t
        {
            m_width  = w;
            m_height = h;
            return *this;
        }

    private:
        weak<device_t> m_device = nullptr;
        VkRenderPass   m_pass   = nullptr;
        ui32           m_width  = 800;
        ui32           m_height = 600;
        size_t         m_count  = 0;

        std::vector<VkImageView> m_attachments;
    };
} // namespace orb::vk
