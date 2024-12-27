#include "orb/vk/desc_pool.hpp"
#include "orb/vk/device.hpp"

namespace orb::vk
{
    auto desc_pool_builder_t::prepare(weak<device_t> device) -> result<desc_pool_builder_t>
    {
        desc_pool_builder_t d;
        d.m_device = device;
        return d;
    }

    auto desc_pool_builder_t::build() -> result<desc_pool_t>
    {
        desc_pool_t pool;
        pool.device = m_device->handle;

        if (m_pool_sizes.empty())
        {
            return error_t { "Could not create descriptor pool: no pool size given" };
        }

        auto pool_info          = vk::structs::create::descriptor_pool();
        pool_info.flags         = vk::descriptor_pool_create_flags::free_descriptor_set_bit;
        pool_info.maxSets       = m_max_sets;
        pool_info.poolSizeCount = m_pool_sizes.size();
        pool_info.pPoolSizes    = m_pool_sizes.data();

        if (auto r = vkCreateDescriptorPool(m_device->handle, &pool_info, nullptr, &pool.handle);
            r != vk::vkres::ok)
        {
            return error_t { "Could not create descriptor pool: {}", vk::vkres::get_repr(r) };
        }

        return pool;
    }

    void destroy(desc_pool_t& pool)
    {
        vkDestroyDescriptorPool(pool.device, pool.handle, nullptr);
    }

} // namespace orb::vk
