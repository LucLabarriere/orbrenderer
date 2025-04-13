#pragma once

#include "orb/vk/cmd_pool.hpp"
#include "orb/vk/device.hpp"

#include "orb/result.hpp"

namespace orb::vk
{
    struct uniform_buffer_t
    {
        uniform_buffer_t()                                           = default;
        uniform_buffer_t(const uniform_buffer_t&)                    = delete;
        auto operator=(const uniform_buffer_t&) -> uniform_buffer_t& = delete;

        uniform_buffer_t(uniform_buffer_t&& other) noexcept
            : allocator(other.allocator),
              allocation(other.allocation),
              buffer(other.buffer),
              ptr(other.ptr),
              size(other.size)
        {
            other.buffer     = nullptr;
            other.ptr        = nullptr;
            other.allocation = nullptr;
        }

        auto operator=(uniform_buffer_t&& other) noexcept -> uniform_buffer_t&
        {
            allocator  = other.allocator;
            allocation = other.allocation;
            buffer     = other.buffer;
            ptr        = other.ptr;
            size       = other.size;

            other.allocation = nullptr;
            other.buffer     = nullptr;
            other.ptr        = nullptr;

            return *this;
        }

        ~uniform_buffer_t()
        {
            destroy();
        }

        void destroy()
        {
            if (buffer && allocation)
            {
                vmaUnmapMemory(allocator, allocation);
                vmaDestroyBuffer(allocator, buffer, allocation);

                allocation = nullptr;
                buffer     = nullptr;
                ptr        = nullptr;
            }
        }

        VmaAllocator  allocator {};
        VmaAllocation allocation {};
        VkBuffer      buffer {};
        VkDeviceSize  size {};
        void*         ptr { nullptr };

        [[nodiscard]] auto transfer(const void* src_data, ui64 size) -> result<void>
        {
            if (size > this->size)
            {
                return error_t { "Uniform buffer too small" };
            }

            memcpy(ptr, src_data, size);

            return {};
        }
    };

    class uniform_buffer_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device, ui64 size)
            -> result<uniform_buffer_builder_t>
        {
            uniform_buffer_builder_t builder;

            builder.m_device = device;

            builder.m_create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            builder.m_create_info.usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            builder.m_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            builder.m_create_info.size        = size;
            builder.m_alloc_info.usage        = VMA_MEMORY_USAGE_AUTO;
            builder.m_alloc_info.flags        = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
            builder.m_size                    = size;

            return builder;
        }

        [[nodiscard]] auto build() -> result<uniform_buffer_t>
        {
            uniform_buffer_t buffer {};
            buffer.allocator = m_device->allocator;

            auto res = vmaCreateBuffer(m_device->allocator,
                                       &m_create_info,
                                       &m_alloc_info,
                                       &buffer.buffer,
                                       &buffer.allocation,
                                       nullptr);

            buffer.size = m_size;

            if (res != vkres::ok)
            {
                return error_t { "Failed to create mapped buffer: {}", vkres::get_repr(res) };
            }

            vmaMapMemory(m_device->allocator, buffer.allocation, &buffer.ptr);

            return buffer;
        }

    private:
        weak<device_t>          m_device;
        VkBufferCreateInfo      m_create_info {};
        VmaAllocationCreateInfo m_alloc_info {};
        ui64                    m_size = 0;
    };
} // namespace orb::vk
