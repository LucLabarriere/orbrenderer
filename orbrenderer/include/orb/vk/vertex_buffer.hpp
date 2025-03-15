#pragma once

#include "orb/vk/device.hpp"
#include "orb/vk/vk_types.hpp"

#include "orb/result.hpp"

namespace orb::vk
{
    struct vertex_buffer_t
    {
        vertex_buffer_t() = default;

        vertex_buffer_t(const vertex_buffer_t&)                    = delete;
        auto operator=(const vertex_buffer_t&) -> vertex_buffer_t& = delete;

        vertex_buffer_t(vertex_buffer_t&& other) noexcept
            : allocator(other.allocator),
              allocation(other.allocation),
              buffer(other.buffer),
              size(other.size)
        {
            other.buffer     = nullptr;
            other.allocation = nullptr;
        }

        auto operator=(vertex_buffer_t&& other) noexcept -> vertex_buffer_t&
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

        ~vertex_buffer_t()
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

    class vertex_buffer_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device)
            -> result<vertex_buffer_builder_t>
        {
            vertex_buffer_builder_t builder;

            builder.m_device = device;

            builder.m_create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            builder.m_create_info.usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            builder.m_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            builder.m_alloc_info.usage        = VMA_MEMORY_USAGE_AUTO;
            builder.m_alloc_info.flags        = 0;

            return builder;
        }

        template <typename TVertexStruct>
        auto vertices(std::span<const TVertexStruct> vertices)
            -> vertex_buffer_builder_t&
        {
            m_create_info.size = vertices.size() * sizeof(TVertexStruct);
            return *this;
        }

        auto sharing_mode(sharing_modes::enum_t mode)
            -> vertex_buffer_builder_t&
        {
            m_create_info.sharingMode = mode;
            return *this;
        }

        auto buffer_usage_flag(buffer_usage_flags::enum_t flag) -> vertex_buffer_builder_t&
        {
            m_create_info.usage |= flag;
            return *this;
        }

        auto memory_usage(vma_mem_usages::enum_t usage) -> vertex_buffer_builder_t&
        {
            m_alloc_info.usage = usage;
            return *this;
        }

        auto memory_flags(vma_alloc_flags::enum_t flags) -> vertex_buffer_builder_t&
        {
            m_alloc_info.flags |= flags;
            return *this;
        }

        [[nodiscard]] auto build() -> result<vertex_buffer_t>
        {
            vertex_buffer_t buffer {};
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
                return error_t { "Failed to create vertex buffer: {}", vkres::get_repr(res) };
            }

            return buffer;
        }

    private:
        weak<device_t>          m_device;
        VkBufferCreateInfo      m_create_info {};
        VmaAllocationCreateInfo m_alloc_info {};
    };
} // namespace orb::vk
