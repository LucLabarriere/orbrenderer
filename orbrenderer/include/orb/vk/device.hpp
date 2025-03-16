#pragma once

#include "orb/vk/core.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <span>
#include <unordered_map>
#include <vector>

using VmaAllocator = struct VmaAllocator_T*;

namespace orb::vk
{
    struct gpu_t;
    struct queue_family_t;

    using priority_t = f32;

    struct queue_info_t
    {
        VkDeviceQueueCreateInfo* create_info {};
        std::vector<priority_t>  priorities {};
    };

    struct device_t
    {
        VkDevice                            handle {};
        std::vector<VkQueue>                queues {};
        VmaAllocator                        allocator {};
        proc_addresses::set_debug_name_fn_t set_debug_name_fb {};

        device_t() = default;

        device_t(const device_t&)                    = delete;
        auto operator=(const device_t&) -> device_t& = delete;

        device_t(device_t&& other) noexcept
        {
            destroy();

            handle            = other.handle;
            queues            = std::move(other.queues);
            allocator         = other.allocator;
            set_debug_name_fb = other.set_debug_name_fb;

            other.handle            = nullptr;
            other.allocator         = nullptr;
            other.set_debug_name_fb = nullptr;
        }

        auto operator=(device_t&& other) noexcept -> device_t&
        {
            destroy();

            handle            = other.handle;
            queues            = std::move(other.queues);
            allocator         = other.allocator;
            set_debug_name_fb = other.set_debug_name_fb;

            other.handle            = nullptr;
            other.allocator         = nullptr;
            other.set_debug_name_fb = nullptr;

            return *this;
        }

        ~device_t()
        {
            destroy();
        }

        void destroy()
        {
            if (allocator)
            {
                vmaDestroyAllocator(allocator);
                allocator = nullptr;
            }

            if (handle)
            {
                vkDestroyDevice(handle, nullptr);
                handle = nullptr;
            }
        }

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

        [[nodiscard]] auto wait() -> result<void>
        {
            const auto res = vkDeviceWaitIdle(handle);

            if (res != vkres::ok)
            {
                return error_t { "Failed to wait for device: {}", vkres::get_repr(res) };
            }

            return {};
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

        auto add_queue(weak<queue_family_t>, priority_t) -> device_builder_t&;

    private:
        device_builder_t() = default;

        VkInstance               m_instance {};
        std::vector<const char*> m_extensions;
        weak<gpu_t>              m_gpu;
        std::vector<priority_t>  m_priorities;

        struct queue_info_t
        {
            size_t create_info_index = 0;
            std::vector<priority_t>  priorities {};
        };

        std::vector<VkDeviceQueueCreateInfo>                    m_queue_infos_raw;
        std::unordered_map<queue_family_t*, queue_info_t> m_queue_infos;
    };
} // namespace orb::vk
