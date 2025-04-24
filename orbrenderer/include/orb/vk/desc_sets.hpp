#pragma once

#include "orb/vk/device.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>
#include <orb/utility.hpp>

namespace orb::vk
{
    struct desc_sets_t
    {
        VkDevice                           device = nullptr;
        std::vector<VkDescriptorSet>       handles;
        std::vector<VkDescriptorSetLayout> layouts;

        desc_sets_t() = default;

        desc_sets_t(const desc_sets_t&)                    = delete;
        auto operator=(const desc_sets_t&) -> desc_sets_t& = delete;

        desc_sets_t(desc_sets_t&& other) noexcept
        {
            handles = std::move(other.handles);
            layouts = std::move(other.layouts);
            device  = other.device;

            other.device = nullptr;
        }

        auto operator=(desc_sets_t&& other) noexcept -> desc_sets_t&
        {
            destroy();

            handles = std::move(other.handles);
            layouts = std::move(other.layouts);
            device  = other.device;

            other.device = nullptr;

            return *this;
        }

        ~desc_sets_t()
        {
            destroy();
        }

        void destroy()
        {
            handles.clear();
            layouts.clear();
            device = nullptr;
        }
    };

    class desc_sets_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t>        device,
                                          VkDescriptorPool      pool,
                                          VkDescriptorSetLayout layout) -> desc_sets_builder_t
        {
            desc_sets_builder_t d;
            d.m_device = device;
            d.m_pool   = pool;
            d.m_layout = layout;
            return d;
        }

        [[nodiscard]] auto count(size_t count) -> desc_sets_builder_t&
        {
            m_count = count;
            return *this;
        }

        [[nodiscard]] auto build() -> result<desc_sets_t>
        {
            desc_sets_t sets;
            sets.device = m_device->handle;

            if (m_count == 0)
            {
                return error_t { "Could not create descriptor sets: count is 0" };
            }

            sets.handles.resize(m_count);
            sets.layouts.resize(m_count, m_layout);

            auto alloc_info               = structs::create::desc_sets(m_pool);
            alloc_info.descriptorSetCount = m_count;
            alloc_info.pSetLayouts        = sets.layouts.data();

            if (auto r = vkAllocateDescriptorSets(m_device->handle, &alloc_info, sets.handles.data());
                r != vkres::ok)
            {
                return error_t { "Could not create descriptor sets: {}", vkres::get_repr(r) };
            }

            return sets;
        }

    private:
        weak<device_t>        m_device = nullptr;
        VkDescriptorPool      m_pool   = nullptr;
        VkDescriptorSetLayout m_layout = nullptr;
        size_t                m_count  = 0;
    };

    class buffer_desc_set_writer_t
    {
    public:
        [[nodiscard]] static auto prepare(VkDevice device, VkDescriptorSet set, ui32 binding) -> buffer_desc_set_writer_t
        {
            buffer_desc_set_writer_t d {};
            d.m_device = device;

            d.m_info.buffer = nullptr;
            d.m_info.offset = 0;
            d.m_info.range  = VK_WHOLE_SIZE;

            d.m_write.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            d.m_write.dstSet           = set;
            d.m_write.dstBinding       = binding;
            d.m_write.dstArrayElement  = 0;
            d.m_write.descriptorType   = vkenum(descriptor_type::uniform_buffer);
            d.m_write.descriptorCount  = 1;
            d.m_write.pImageInfo       = nullptr;
            d.m_write.pTexelBufferView = nullptr;
            d.m_write.pBufferInfo      = &d.m_info;

            return d;
        }

        auto update_sets() -> result<void>
        {
            if (!m_info.buffer)
            {
                return error_t { "Could not update descriptor set: buffer is not prepared" };
            }

            if (m_write.descriptorCount == 0)
            {
                return error_t { "Could not update descriptor set: descriptor count is 0" };
            }

            vkUpdateDescriptorSets(m_device, 1, &m_write, 0, nullptr);

            return {};
        }

        auto buffer(VkBuffer buffer) -> buffer_desc_set_writer_t&
        {
            m_info.buffer = buffer;
            return *this;
        }

        auto offset(VkDeviceSize offset) -> buffer_desc_set_writer_t&
        {
            m_info.offset = offset;
            return *this;
        }

        auto range(VkDeviceSize range) -> buffer_desc_set_writer_t&
        {
            m_info.range = range;
            return *this;
        }

        auto element_index(ui32 index) -> buffer_desc_set_writer_t&
        {
            m_write.dstArrayElement = index;
            return *this;
        }

        auto descriptor_count(ui32 count) -> buffer_desc_set_writer_t&
        {
            m_write.descriptorCount = count;
            return *this;
        }

        [[nodiscard]] auto set() const -> const VkDescriptorSet&
        {
            return m_write.dstSet;
        }

    private:
        VkDevice               m_device = nullptr;
        VkDescriptorBufferInfo m_info {};
        VkWriteDescriptorSet   m_write {};
    };
} // namespace orb::vk
