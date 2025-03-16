#pragma once

#include "orb/vk/cmd_pool.hpp"
#include "orb/vk/device.hpp"

#include "orb/result.hpp"

namespace orb::vk
{
    struct staging_buffer_t
    {
        staging_buffer_t()                                           = default;
        staging_buffer_t(const staging_buffer_t&)                    = delete;
        auto operator=(const staging_buffer_t&) -> staging_buffer_t& = delete;

        staging_buffer_t(staging_buffer_t&& other) noexcept
            : allocator(other.allocator),
              allocation(other.allocation),
              buffer(other.buffer),
              size(other.size)
        {
            other.buffer     = nullptr;
            other.allocation = nullptr;
        }

        auto operator=(staging_buffer_t&& other) noexcept -> staging_buffer_t&
        {
            allocator  = other.allocator;
            allocation = other.allocation;
            buffer     = other.buffer;
            size       = other.size;

            other.allocation = nullptr;
            other.buffer     = nullptr;

            return *this;
        }

        ~staging_buffer_t()
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

        VmaAllocator  allocator {};
        VmaAllocation allocation {};
        VkBuffer      buffer {};
        VkDeviceSize  size {};

        [[nodiscard]] auto transfer(const void* src_data, ui64 size) -> result<void>
        {
            void* dst_data {};
            if (size > this->size)
            {
                return error_t { "Staging buffer too small" };
            }

            vmaMapMemory(allocator, allocation, &dst_data);
            memcpy(dst_data, src_data, size);
            vmaUnmapMemory(allocator, allocation);

            return {};
        }
    };

    class staging_buffer_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device, ui64 size)
            -> result<staging_buffer_builder_t>
        {
            staging_buffer_builder_t builder;

            builder.m_device = device;

            builder.m_create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            builder.m_create_info.usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            builder.m_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            builder.m_create_info.size        = size;
            builder.m_alloc_info.usage        = VMA_MEMORY_USAGE_AUTO;
            builder.m_alloc_info.flags        = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
            builder.m_size                    = size;

            return builder;
        }

        [[nodiscard]] auto build() -> result<staging_buffer_t>
        {
            staging_buffer_t buffer {};
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
                return error_t { "Failed to create staging buffer: {}", vkres::get_repr(res) };
            }

            return buffer;
        }

    private:
        weak<device_t>          m_device;
        VkBufferCreateInfo      m_create_info {};
        VmaAllocationCreateInfo m_alloc_info {};
        ui64                    m_size = 0;
    };
} // namespace orb::vk
