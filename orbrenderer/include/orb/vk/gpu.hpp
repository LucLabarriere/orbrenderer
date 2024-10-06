#pragma once

#include "orb/vk/vk_types.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <vector>

namespace orb::vk
{
    struct queue_family_t
    {
        ui32                    index {};
        VkQueueFamilyProperties properties {};
    };

    struct gpu_t
    {
        VkPhysicalDevice                 handle {};
        ui32                             api_version {};
        ui32                             driver_version {};
        ui32                             vendor_id {};
        ui32                             device_id {};
        gpu_types::enum_t                device_type {};
        std::string                      name {};
        VkPhysicalDeviceLimits           limits {};
        VkPhysicalDeviceSparseProperties sparse_properties {};
        std::vector<queue_family_t>      queue_families;
    };

    void describe_gpu(const gpu_t& gpu);

    class available_gpus_t
    {
    public:
        [[nodiscard]] static auto create(VkInstance instance) -> result<available_gpus_t>;

        [[nodiscard]] auto begin() const { return m_gpus.begin(); }
        [[nodiscard]] auto end() const { return m_gpus.end(); }
        [[nodiscard]] auto begin() { return m_gpus.begin(); }
        [[nodiscard]] auto end() { return m_gpus.end(); }

        [[nodiscard]] auto select(size_t index) -> box<gpu_t>
        {
            auto gpu = std::move(m_gpus[index]);
            m_gpus.clear();
            return gpu;
        }

    private:
        available_gpus_t() = default;

        std::vector<box<gpu_t>> m_gpus;
    };
} // namespace orb::vk
