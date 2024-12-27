#pragma once

#include <orb/vk/vk_structs.hpp>

#include <orb/box.hpp>
#include <orb/result.hpp>
#include <orb/utility.hpp>

#include <vector>
#include <vulkan/vulkan_core.h>

namespace orb::vk
{
    struct device_t;

    struct desc_pool_t
    {
        VkDescriptorPool handle = nullptr;
        VkDevice         device = nullptr;
    };

    class desc_pool_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t>) -> result<desc_pool_builder_t>;
        [[nodiscard]] auto        build() -> result<desc_pool_t>;

        auto pool(VkDescriptorType type, ui32 count) -> desc_pool_builder_t&
        {
            m_pool_sizes.push_back({ .type = type, .descriptorCount = count });
            return *this;
        }

        auto flag(descriptor_pool_create_flags::enum_t flag) -> desc_pool_builder_t&
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

    void destroy(desc_pool_t&);
} // namespace orb::vk
