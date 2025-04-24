#pragma once

#include "orb/vk/core.hpp"

#include <orb/box.hpp>
#include <orb/flux.hpp>
#include <orb/result.hpp>

#include <vector>

namespace orb::vk
{
    struct images_t
    {
        std::vector<VkImage>       handles;
        std::vector<VmaAllocation> allocations;

        VmaAllocator allocator = nullptr;

        images_t() = default;

        images_t(const images_t&)                    = delete;
        auto operator=(const images_t&) -> images_t& = delete;

        images_t(images_t&& other) noexcept
        {
            destroy();

            handles     = std::move(other.handles);
            allocations = std::move(other.allocations);
            allocator   = other.allocator;

            other.allocator = nullptr;
        }

        auto operator=(images_t&& other) noexcept -> images_t&
        {
            destroy();

            handles     = std::move(other.handles);
            allocations = std::move(other.allocations);
            allocator   = other.allocator;

            other.allocator = nullptr;

            return *this;
        }

        ~images_t()
        {
            destroy();
        }

        void destroy()
        {
            for (const auto& [img, alloc] : flux::zip_all_mut(handles, allocations))
            {
                vmaDestroyImage(allocator, img, alloc);
                img   = nullptr;
                alloc = nullptr;
            }
        }
    };

    class images_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(VmaAllocator) -> result<images_builder_t>;
        [[nodiscard]] auto        build() -> result<images_t>;

        auto count(size_t count) -> images_builder_t&
        {
            m_count = count;
            return *this;
        }

        auto size(ui32 w, ui32 h) -> images_builder_t&
        {
            m_info.extent.width  = w;
            m_info.extent.height = h;
            return *this;
        }

        auto depth(ui32 depth) -> images_builder_t&
        {
            m_info.extent.depth = depth;
            return *this;
        }

        auto mip_levels(ui32 levels) -> images_builder_t&
        {
            m_info.mipLevels = levels;
            return *this;
        }

        auto array_layers(ui32 layers) -> images_builder_t&
        {
            m_info.arrayLayers = layers;
            return *this;
        }

        auto tiling(image_tiling tiling) -> images_builder_t&
        {
            m_info.tiling = vkenum(tiling);
            return *this;
        }

        auto samples(sample_count_flag count) -> images_builder_t&
        {
            m_info.samples = vkenum(count);
            return *this;
        }

        auto sharing_mode(sharing_mode mode) -> images_builder_t&
        {
            m_info.sharingMode = vkenum(mode);
            return *this;
        }

        auto format(format format) -> images_builder_t&
        {
            m_info.format = vkenum(format);
            return *this;
        }

        auto usage(image_usage_flag flag) -> images_builder_t&
        {
            m_info.usage |= vkflag(flag);
            return *this;
        }

        auto mem_usage(memory_usage usage) -> images_builder_t&
        {
            m_alloc_info.usage = vkenum(usage);
            return *this;
        }

        auto mem_flags(memory_flag flag) -> images_builder_t&
        {
            m_alloc_info.flags |= vkflag(flag);
            return *this;
        }

    private:
        VmaAllocator m_allocator = nullptr;
        size_t       m_count     = 1;

        VkImageCreateInfo       m_info       = vk::structs::create::image();
        VmaAllocationCreateInfo m_alloc_info = vk::structs::create::allocation();
    };

    void transition_layout(VkCommandBuffer       cmd,
                           VkImage               img,
                           image_layout prev,
                           image_layout next);

    void copy_img(VkCommandBuffer cmd,
                  VkImage         src,
                  VkImage         dst,
                  VkExtent2D      src_extent);
} // namespace orb::vk
