#include "orb/vk/device.hpp"

#include "orb/vk/gpu.hpp"

#include <orb/flux.hpp>

namespace orb::vk
{
    auto device_builder_t::prepare(VkInstance instance) -> result<device_builder_t>
    {
        device_builder_t d;
        d.m_instance = instance;
        return d;
    }

    auto device_builder_t::add_queues(queue_family_t& qf, std::span<const priority_t> priorities)
        -> device_builder_t&
    {
        auto& info            = m_queue_infos.emplace_back(structs::create::device_queue());
        info.pQueuePriorities = priorities.data();
        info.queueCount       = priorities.size();
        info.queueFamilyIndex = qf.index;

        return *this;
    }

    auto device_builder_t::build(gpu_t& gpu) -> result<box<device_t>>
    {
        auto set_debug_name_fn = proc_addresses::set_debug_name(m_instance);

        auto create_info                    = structs::create::device();
        create_info.queueCreateInfoCount    = m_queue_infos.size();
        create_info.pQueueCreateInfos       = m_queue_infos.data();
        create_info.enabledExtensionCount   = m_extensions.size();
        create_info.ppEnabledExtensionNames = m_extensions.data();

        auto device = make_box<device_t>();
        auto res    = vkCreateDevice(gpu.handle, &create_info, nullptr, &device->handle);

        if (res != VK_SUCCESS) { return error_t { "Could not create vulkan device: {}", (ui32)res }; }

        for (auto& queue_info : m_queue_infos)
        {
            for (size_t i : flux::range(queue_info.queueCount))
            {
                auto& queue = device->queues.emplace_back();
                vkGetDeviceQueue(device->handle, queue_info.queueFamilyIndex, (ui32)i, &queue);
            }
        }

        auto allocator_info             = structs::create::allocator();
        allocator_info.physicalDevice   = gpu.handle;
        allocator_info.device           = device->handle;
        allocator_info.instance         = m_instance;
        allocator_info.pVulkanFunctions = nullptr;

        vmaCreateAllocator(&allocator_info, &device->allocator);

        device->set_debug_name_fb = set_debug_name_fn;
        return device;
    }
} // namespace orb::vk
