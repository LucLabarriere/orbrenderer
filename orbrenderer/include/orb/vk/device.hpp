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
        VkDevice                            handle {};
        std::vector<VkQueue>                queues {};
        VmaAllocator                        allocator {};
        proc_addresses::set_debug_name_fn_t set_debug_name_fb {};

        template <vk::vk_type T>
        void set_name(T obj, const char* name)
        {
            VkDebugUtilsObjectNameInfoEXT name_info {
                .sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                .objectType   = obj_types::obj_type<T>,
                .objectHandle = reinterpret_cast<uint64_t>(obj), // NOLINT
                .pObjectName  = name,
            };
            set_debug_name_fb(handle, &name_info);
        }
    };

    class device_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(VkInstance instance) -> result<device_builder_t>;
        [[nodiscard]] auto        build(gpu_t&) -> result<box<device_t>>;

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
