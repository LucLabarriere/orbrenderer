#pragma once

#include "orb/vk/vk_structs.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <vector>

namespace orb::vk
{
    struct device_t;

    struct framebuffers_t
    {
        std::vector<VkFramebuffer> handles;

        VkDevice device = nullptr;
    };

    class framebuffers_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t>, VkRenderPass) -> result<framebuffers_builder_t>;
        [[nodiscard]] auto        build() -> result<framebuffers_t>;

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

    void destroy(framebuffers_t&);

} // namespace orb::vk
