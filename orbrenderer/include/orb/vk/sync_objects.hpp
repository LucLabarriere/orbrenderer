#pragma once

#include "vk_types.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <vector>

namespace orb::vk
{
    struct device_t;

    struct sync_objects_t
    {
        VkDevice                 device = nullptr;
        std::vector<VkSemaphore> semaphores;
        std::vector<VkFence>     fences;
    };

    class sync_objects_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device) -> result<sync_objects_builder_t>;
        [[nodiscard]] auto        build() -> result<sync_objects_t>;

        auto semaphores(ui32 count) -> sync_objects_builder_t&
        {
            m_semaphore_count = count;
            return *this;
        }

        auto fences(ui32 count) -> sync_objects_builder_t&
        {
            m_fence_count = count;
            return *this;
        }

    private:
        weak<device_t> m_device          = nullptr;
        ui32           m_semaphore_count = 0;
        ui32           m_fence_count     = 0;
    };

    void destroy(sync_objects_t&);
} // namespace orb::vk
