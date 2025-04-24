#pragma once

#include "orb/vk/device.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>
#include <orb/utility.hpp>

#include <vector>

namespace orb::vk
{
    struct desc_pool_t
    {
        VkDescriptorPool handle = nullptr;
        VkDevice         device = nullptr;

        desc_pool_t() = default;

        desc_pool_t(const desc_pool_t&)                    = delete;
        auto operator=(const desc_pool_t&) -> desc_pool_t& = delete;

        desc_pool_t(desc_pool_t&& other) noexcept
        {
            handle = other.handle;
            device = other.device;

            other.handle = nullptr;
            other.device = nullptr;
        }

        auto operator=(desc_pool_t&& other) noexcept -> desc_pool_t&
        {
            destroy();

            handle = other.handle;
            device = other.device;

            other.handle = nullptr;
            other.device = nullptr;

            return *this;
        }

        ~desc_pool_t()
        {
            destroy();
        }

        void destroy()
        {
            if (!handle) return;

            vkDestroyDescriptorPool(device, handle, nullptr);
            handle = nullptr;
        }
    };

    class desc_pool_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device) -> result<desc_pool_builder_t>
        {
            desc_pool_builder_t d;
            d.m_device = device;
            return d;
        }

        [[nodiscard]] auto build() -> result<desc_pool_t>
        {
            desc_pool_t pool;
            pool.device = m_device->handle;

            if (m_pool_sizes.empty())
            {
                return error_t { "Could not create descriptor pool: no pool size given" };
            }

            auto pool_info          = structs::create::descriptor_pool();
            pool_info.flags         = vkenum(descriptor_pool_create_flag::free_descriptor_set);
            pool_info.maxSets       = m_max_sets;
            pool_info.poolSizeCount = m_pool_sizes.size();
            pool_info.pPoolSizes    = m_pool_sizes.data();

            if (auto r = vkCreateDescriptorPool(m_device->handle, &pool_info, nullptr, &pool.handle);
                r != vkres::ok)
            {
                return error_t { "Could not create descriptor pool: {}", vkres::get_repr(r) };
            }

            return pool;
        }

        auto pool(descriptor_type type, ui32 count) -> desc_pool_builder_t&
        {
            m_pool_sizes.push_back({ .type = vkenum(type), .descriptorCount = count });
            return *this;
        }

        auto flag(descriptor_pool_create_flag flag) -> desc_pool_builder_t&
        {
            m_flags |= static_cast<ui32>(flag);
            return *this;
        }

        auto max_desc_sets(ui32 count) -> desc_pool_builder_t&
        {
            m_max_sets = count;
            return *this;
        }

    private:
        std::vector<VkDescriptorPoolSize> m_pool_sizes;
        weak<device_t>                    m_device = nullptr;

        ui32 m_flags {};
        ui32 m_max_sets = 1;
    };
} // namespace orb::vk
