#pragma once

#include "orb/vk/device.hpp"

#include <orb/result.hpp>

namespace orb::vk
{
    struct index_buffer_t
    {
        index_buffer_t() = default;

        index_buffer_t(const index_buffer_t&)                    = delete;
        auto operator=(const index_buffer_t&) -> index_buffer_t& = delete;

        index_buffer_t(index_buffer_t&& other) noexcept
            : allocator(other.allocator),
              allocation(other.allocation),
              buffer(other.buffer),
              size(other.size)
        {
            other.buffer     = nullptr;
            other.allocation = nullptr;
        }

        auto operator=(index_buffer_t&& other) noexcept -> index_buffer_t&
        {
            allocator  = other.allocator;
            allocation = other.allocation;
            buffer     = other.buffer;
            size       = other.size;

            other.allocation = nullptr;
            other.buffer     = nullptr;

            return *this;
        }

        VmaAllocator  allocator {};
        VmaAllocation allocation {};
        VkBuffer      buffer {};
        ui64          size {};
        size_t        count {};
        VkIndexType   index_type {};

        ~index_buffer_t()
        {
            destroy();
        }

        void destroy()
        {
            if (buffer && allocation)
            {
                vmaDestroyBuffer(allocator, buffer, allocation);

                allocation = nullptr;
                buffer     = nullptr;
            }
        }
    };

    class index_buffer_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device)
            -> result<index_buffer_builder_t>
        {
            index_buffer_builder_t builder;

            builder.m_device = device;

            builder.m_create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            builder.m_create_info.usage       = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            builder.m_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            builder.m_alloc_info.usage        = VMA_MEMORY_USAGE_AUTO;
            builder.m_alloc_info.flags        = 0;

            return builder;
        }

        template <typename TIndexType>
        auto indices(std::span<const TIndexType> indices)
            -> index_buffer_builder_t&
        {
            m_count            = indices.size();
            m_create_info.size = indices.size() * sizeof(TIndexType);
            if constexpr (std::is_same_v<TIndexType, ui32>)
            {
                m_index_type = VK_INDEX_TYPE_UINT32;
            }
            else if constexpr (std::is_same_v<TIndexType, ui16>)
            {
                m_index_type = VK_INDEX_TYPE_UINT16;
            }
            else if constexpr (std::is_same_v<TIndexType, ui8>)
            {
                m_index_type = VK_INDEX_TYPE_UINT8_EXT;
            }

            return *this;
        }

        auto sharing_mode(sharing_mode mode)
            -> index_buffer_builder_t&
        {
            m_create_info.sharingMode = vkenum(mode);
            return *this;
        }

        auto buffer_usage_flag(buffer_usage_flag flag) -> index_buffer_builder_t&
        {
            m_create_info.usage |= vkenum(flag);
            return *this;
        }

        auto memory_usage(memory_usage usage) -> index_buffer_builder_t&
        {
            m_alloc_info.usage = vkenum(usage);
            return *this;
        }

        auto memory_flags(memory_flag flags) -> index_buffer_builder_t&
        {
            m_alloc_info.flags |= vkenum(flags);
            return *this;
        }

        [[nodiscard]] auto build() -> result<index_buffer_t>
        {
            index_buffer_t buffer {};
            buffer.count     = m_count;
            buffer.allocator = m_device->allocator;

            auto res = vmaCreateBuffer(m_device->allocator,
                                       &m_create_info,
                                       &m_alloc_info,
                                       &buffer.buffer,
                                       &buffer.allocation,
                                       nullptr);

            buffer.size = m_create_info.size;

            if (res != vkres::ok)
            {
                return error_t { "Failed to create index buffer: {}", vkres::get_repr(res) };
            }

            return buffer;
        }

    private:
        weak<device_t>          m_device;
        VkBufferCreateInfo      m_create_info {};
        VmaAllocationCreateInfo m_alloc_info {};
        VkIndexType             m_index_type {};
        size_t                  m_count {};
    };
} // namespace orb::vk
