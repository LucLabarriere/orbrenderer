#pragma once

#include "orb/vk/vk_structs.hpp"
#include "orb/vk/vk_types.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <vector>

namespace orb::vk
{
    struct img_view_pair_t
    {
        VkImage     img  = nullptr;
        VkImageView view = nullptr;
    };

    struct views_t
    {
        VkDevice device = nullptr;

        std::vector<img_view_pair_t> handles;
    };

    class views_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(VkDevice) -> result<views_builder_t>;
        [[nodiscard]] auto        build() -> result<views_t>;

        auto image(VkImage img) -> views_builder_t&
        {
            m_handles.emplace_back(img, nullptr);
            return *this;
        }

        auto type(vk::image_view_types::enum_t type) -> views_builder_t&
        {
            m_info.viewType = type;
            return *this;
        }

        auto format(vk::formats::enum_t format) -> views_builder_t&
        {
            m_info.format = format;
            return *this;
        }

        auto r(vk::component_swizzles::enum_t r) -> views_builder_t&
        {
            m_info.components.r = r;
            return *this;
        }

        auto g(vk::component_swizzles::enum_t g) -> views_builder_t&
        {
            m_info.components.g = g;
            return *this;
        }

        auto b(vk::component_swizzles::enum_t b) -> views_builder_t&
        {
            m_info.components.b = b;
            return *this;
        }

        auto a(vk::component_swizzles::enum_t a) -> views_builder_t&
        {
            m_info.components.a = a;
            return *this;
        }

        auto aspect_mask(vk::image_aspect_flags::enum_t mask) -> views_builder_t&
        {
            m_info.subresourceRange.aspectMask |= mask;
            return *this;
        }

        auto base_mip_level(ui32 level) -> views_builder_t&
        {
            m_info.subresourceRange.baseMipLevel = level;
            return *this;
        }

        auto mip_level_count(ui32 count) -> views_builder_t&
        {
            m_info.subresourceRange.levelCount = count;
            return *this;
        }

        auto layer_count(ui32 count) -> views_builder_t&
        {
            m_info.subresourceRange.layerCount = count;
            return *this;
        }

        auto base_array_layer(ui32 layer) -> views_builder_t&
        {
            m_info.subresourceRange.baseArrayLayer = layer;
            return *this;
        }

    private:
        VkDevice m_device = nullptr;

        VkImageViewCreateInfo        m_info = vk::structs::create::image_view();
        std::vector<img_view_pair_t> m_handles;
    };

    void destroy(views_t&);
} // namespace orb::vk
