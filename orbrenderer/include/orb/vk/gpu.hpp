#pragma once

#include "orb/vk/core.hpp"

#include <orb/box.hpp>
#include <orb/flux.hpp>
#include <orb/result.hpp>

#include <vector>

namespace orb::vk
{
    struct queue_family_t
    {
        ui32                    index {};
        VkQueueFamilyProperties properties {};
        std::vector<VkQueue>    queues {};
    };

    class queue_family_map_t
    {
    public:
        using queue_family_ref_t  = weak<queue_family_t>;
        using queue_family_span_t = std::span<const queue_family_ref_t>;

        [[nodiscard]] static auto create(std::span<box<queue_family_t>>)
            -> box<queue_family_map_t>;

        [[nodiscard]] auto get_queues(queue_families::enum_t type) const
            -> result<queue_family_span_t>
        {
            const size_t index = get_index(type);

            if (index == 4)
            {
                return error_t { "Invalid queue family type" };
            }

            return queue_family_span_t { m_families[index] };
        }

        [[nodiscard]] auto graphics() const { return get_queues(queue_families::graphics); }
        [[nodiscard]] auto transfer() const { return get_queues(queue_families::transfer); }
        [[nodiscard]] auto compute() const { return get_queues(queue_families::compute); }
        [[nodiscard]] auto sparse() const { return get_queues(queue_families::sparse); }

    private:
        std::array<std::vector<queue_family_ref_t>, 4> m_families {};

        [[nodiscard]] static constexpr auto get_index(queue_families::enum_t type) -> size_t
        {
            switch (type)
            {
            case queue_families::graphics: return 0;
            case queue_families::transfer: return 1;
            case queue_families::compute: return 2;
            case queue_families::sparse: return 3;
            default: return 4;
            }
        }
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
        box<queue_family_map_t>          queue_family_map;
        std::vector<box<queue_family_t>> queue_families;

        void describe() const;
    };

    class gpu_selector_t
    {
    public:
        [[nodiscard]] static auto prepare(VkInstance instance) -> result<gpu_selector_t>;

        [[nodiscard]] auto begin() const { return m_gpus.begin(); }
        [[nodiscard]] auto end() const { return m_gpus.end(); }
        [[nodiscard]] auto begin() { return m_gpus.begin(); }
        [[nodiscard]] auto end() { return m_gpus.end(); }

        [[nodiscard]] auto prefer_type(gpu_types::enum_t type) -> gpu_selector_t&
        {
            m_prefered_types.push_back(type);
            return *this;
        }

        [[nodiscard]] auto select(size_t index) -> box<gpu_t>
        {
            auto gpu = std::move(m_gpus[index]);
            m_gpus.clear();
            return gpu;
        }

        [[nodiscard]] auto select() -> result<box<gpu_t>>
        {
            if (m_gpus.empty())
            {
                return error_t { "No GPU found" };
            }

            for (const auto& type : m_prefered_types)
            {
                for (auto [i, gpu] : flux::enumerate(m_gpus))
                {
                    if (gpu->device_type == type)
                    {
                        auto gpu = std::move(m_gpus[i]);
                        m_gpus.clear();
                        return gpu;
                    }
                }
            }

            auto gpu = std::move(m_gpus.front());
            m_gpus.clear();
            return gpu;
        }

    private:
        gpu_selector_t() = default;

        std::vector<gpu_types::enum_t> m_prefered_types;
        std::vector<box<gpu_t>>        m_gpus;
    };
} // namespace orb::vk
