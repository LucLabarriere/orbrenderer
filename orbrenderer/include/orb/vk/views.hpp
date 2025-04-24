#pragma once

#include "orb/vk/core.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <span>
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

        views_t() = default;

        views_t(const views_t&)                    = delete;
        auto operator=(const views_t&) -> views_t& = delete;

        views_t(views_t&& other) noexcept
        {
            destroy();

            device  = other.device;
            handles = std::move(other.handles);
        }

        auto operator=(views_t&& other) noexcept -> views_t&
        {
            destroy();

            device  = other.device;
            handles = std::move(other.handles);

            return *this;
        }

        ~views_t()
        {
            destroy();
        }

        void destroy()
        {
            for (auto& [img, view] : handles)
            {
                vkDestroyImageView(device, view, nullptr);
                view = nullptr;
            }
        }
    };

    class views_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(VkDevice device) -> result<views_builder_t>
        {
            views_builder_t b;

            b.m_device = device;

            b.m_info.viewType = vkenum(image_view_type::_2d);
            b.m_info.format   = vkenum(format::b8g8r8a8_srgb);

            b.m_info.components.r = vkenum(component_swizzle::r);
            b.m_info.components.g = vkenum(component_swizzle::g);
            b.m_info.components.b = vkenum(component_swizzle::b);
            b.m_info.components.a = vkenum(component_swizzle::a);

            b.m_info.subresourceRange.aspectMask     = vkenum(image_aspect_flag::none);
            b.m_info.subresourceRange.baseMipLevel   = 0;
            b.m_info.subresourceRange.levelCount     = 1;
            b.m_info.subresourceRange.baseArrayLayer = 0;
            b.m_info.subresourceRange.layerCount     = 1;

            return std::move(b);
        }

        [[nodiscard]] auto build() -> result<views_t>
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

        auto image(VkImage img) -> views_builder_t&
        {
            m_handles.emplace_back(img, nullptr);

            return *this;
        }

        auto images(std::span<VkImage> imgs) -> views_builder_t&
        {
            for (auto img : imgs)
            {
                image(img);
            }

            return *this;
        }

        auto type(image_view_type type) -> views_builder_t&
        {
            m_info.viewType = vkenum(type);
            return *this;
        }

        auto format(format format) -> views_builder_t&
        {
            m_info.format = vkenum(format);
            return *this;
        }

        auto r(component_swizzle r) -> views_builder_t&
        {
            m_info.components.r = vkenum(r);
            return *this;
        }

        auto g(component_swizzle g) -> views_builder_t&
        {
            m_info.components.g = vkenum(g);
            return *this;
        }

        auto b(component_swizzle b) -> views_builder_t&
        {
            m_info.components.b = vkenum(b);
            return *this;
        }

        auto a(component_swizzle a) -> views_builder_t&
        {
            m_info.components.a = vkenum(a);
            return *this;
        }

        auto aspect_mask(image_aspect_flag mask) -> views_builder_t&
        {
            m_info.subresourceRange.aspectMask |= vkenum(mask);
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

        VkImageViewCreateInfo        m_info = structs::create::image_view();
        std::vector<img_view_pair_t> m_handles;
    };
} // namespace orb::vk
