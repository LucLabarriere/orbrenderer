#include "orb/vk/framebuffers.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/vk_structs.hpp"

#include <orb/flux.hpp>

namespace orb::vk
{
    auto framebuffers_builder_t::prepare(weak<device_t> device, VkRenderPass pass) -> result<framebuffers_builder_t>
    {
        framebuffers_builder_t d;
        d.m_device = device;
        d.m_pass   = pass;
        return d;
    }

    auto framebuffers_builder_t::build() -> result<framebuffers_t>
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

    void destroy(framebuffers_t& fbs)
    {
        for (auto& fb : fbs.handles)
        {
            vkDestroyFramebuffer(fbs.device, fb, nullptr);
            fb = nullptr;
        }
    }

} // namespace orb::vk
