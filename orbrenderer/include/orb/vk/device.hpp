#pragma once

#include "orb/vk/vk_types.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <span>
#include <vector>

using VmaAllocator = struct VmaAllocator_T*;

namespace orb::vk
{
    struct gpu_t;
    struct queue_family_t;

    using priority_t = f32;

    struct device_t
    {
        VkDevice             handle {};
        std::vector<VkQueue> queues {};
        VmaAllocator         allocator {};
    };

    class device_builder_t
    {
    public:
        static auto        prepare(VkInstance instance) -> result<device_builder_t>;
        [[nodiscard]] auto build(weak<gpu_t>) -> result<device_t>;

        auto add_extension(const char* ext) -> device_builder_t&
        {
            m_extensions.push_back(ext);
            return *this;
        };

        auto add_queues(queue_family_t& qf, std::span<const priority_t> priorities) -> device_builder_t&;

    private:
        device_builder_t() = default;

        VkInstance                           m_instance {};
        std::vector<const char*>             m_extensions;
        weak<gpu_t>                          m_gpu;
        std::vector<VkDeviceQueueCreateInfo> m_queue_infos;
    };

    auto alloc_cmd(device_t&, VkCommandPool) -> result<VkCommandBuffer>;
    void device_idle(device_t&);
    void destroy(device_t&);

} // namespace orb::vk
