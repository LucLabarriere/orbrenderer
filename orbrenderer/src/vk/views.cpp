#include "orb/vk/views.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/vk_structs.hpp"

#include <orb/flux.hpp>

namespace orb::vk
{
    auto views_builder_t::prepare(VkDevice device) -> result<views_builder_t>
    {
        views_builder_t b;

        b.m_device = device;

        b.m_info.viewType = vk::image_view_types::type_2d;
        b.m_info.format   = vk::formats::b8g8r8a8_srgb;

        b.m_info.components.r = vk::component_swizzles::r;
        b.m_info.components.g = vk::component_swizzles::g;
        b.m_info.components.b = vk::component_swizzles::b;
        b.m_info.components.a = vk::component_swizzles::a;

        b.m_info.subresourceRange.aspectMask     = vk::image_aspect_flags::none;
        b.m_info.subresourceRange.baseMipLevel   = 0;
        b.m_info.subresourceRange.levelCount     = 1;
        b.m_info.subresourceRange.baseArrayLayer = 0;
        b.m_info.subresourceRange.layerCount     = 1;

        return std::move(b);
    }

    auto views_builder_t::build() -> result<views_t>
    {
        views_t views;
        views.device = m_device;

        views.handles = std::move(m_handles);

        for (auto& [img, view] : views.handles)
        {
            m_info.image = img;
            vkCreateImageView(m_device, &m_info, nullptr, &view);
        }

        return views;
    }

    void destroy(views_t& views)
    {
        for (auto& [img, view] : views.handles)
        {
            vkDestroyImageView(views.device, view, nullptr);
            view = nullptr;
        }
    }

} // namespace orb::vk
