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

    auto device_builder_t::add_queue(weak<queue_family_t> qf, priority_t priority)
        -> device_builder_t&
    {
        auto infoIt = m_queue_infos.find(qf.raw());
        if (infoIt == m_queue_infos.end())
        {
            infoIt = m_queue_infos.emplace(qf.raw(), queue_info_t {}).first;

            infoIt->second.create_info_index = m_queue_infos_raw.size();

            auto& create_info = m_queue_infos_raw.emplace_back(structs::create::device_queue());

            create_info.queueFamilyIndex = qf->index;
            create_info.queueCount       = 0;
        }

        auto& info        = infoIt->second;
        auto& create_info = m_queue_infos_raw.at(info.create_info_index);

        if (qf->properties.queueCount == create_info.queueCount)
        {
            fmt::println("Warning: Requested {} queues from queue family {}, but only {} queue(s) available",
                         create_info.queueCount + 1,
                         qf->index,
                         qf->properties.queueCount);
            return *this;
        }

        info.priorities.push_back(priority);

        create_info.queueCount++;
        create_info.pQueuePriorities = info.priorities.data();

        return *this;
    }

    auto device_builder_t::build(gpu_t& gpu) -> result<box<device_t>>
    {
        auto set_debug_name_fn = proc_addresses::set_debug_name(m_instance);

        auto create_info                    = structs::create::device();
        create_info.queueCreateInfoCount    = m_queue_infos_raw.size();
        create_info.pQueueCreateInfos       = m_queue_infos_raw.data();
        create_info.enabledExtensionCount   = m_extensions.size();
        create_info.ppEnabledExtensionNames = m_extensions.data();

        auto device = make_box<device_t>();
        auto res    = vkCreateDevice(gpu.handle, &create_info, nullptr, &device->handle);

        if (res != vkres::ok)
        {
            return error_t { "Could not create vulkan device: {}", vkres::get_repr(res) };
        }

        for (auto& [qf, queue_info] : m_queue_infos)
        {
            for (size_t i : flux::range(queue_info.priorities.size()))
            {
                auto& queue = device->queues.emplace_back();
                vkGetDeviceQueue(device->handle, qf->index, (ui32)i, &queue);
                qf->queues.push_back(queue);
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
