#include "orb/vk/device.hpp"
#include "orb/vk/gpu.hpp"
#include "orb/vk/vk_structs.hpp"
#include "orb/vk/vma.hpp"

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

    auto device_builder_t::build(weak<gpu_t> gpu) -> result<device_t>
    {
        auto create_info                    = structs::create::device();
        create_info.queueCreateInfoCount    = m_queue_infos.size();
        create_info.pQueueCreateInfos       = m_queue_infos.data();
        create_info.enabledExtensionCount   = m_extensions.size();
        create_info.ppEnabledExtensionNames = m_extensions.data();

        device_t device;
        auto     res = vkCreateDevice(gpu->handle, &create_info, nullptr, &device.handle);

        if (res != VK_SUCCESS) { return error_t { "Could not create vulkan device: {}", (ui32)res }; }

        for (auto& queue_info : m_queue_infos)
        {
            for (size_t i : flux::range(queue_info.queueCount))
            {
                auto& queue = device.queues.emplace_back();
                vkGetDeviceQueue(device.handle, queue_info.queueFamilyIndex, (ui32)i, &queue);
            }
        }

        auto allocator_info             = structs::create::allocator();
        allocator_info.physicalDevice   = gpu->handle;
        allocator_info.device           = device.handle;
        allocator_info.instance         = m_instance;
        allocator_info.pVulkanFunctions = nullptr;

        vmaCreateAllocator(&allocator_info, &device.allocator);

        return device;
    }

    auto alloc_cmd(device_t& device, VkCommandPool pool) -> result<VkCommandBuffer>
    {
        auto cmd_info               = vk::structs::create::cmd_buffer();
        cmd_info.commandPool        = pool;
        cmd_info.commandBufferCount = 1;
        VkCommandBuffer cmd {};

        if (auto r = vkAllocateCommandBuffers(device.handle, &cmd_info, &cmd); r != vk::vkres::ok)
        {
            return error_t { "Could not allocate cmd buffer: {}", vkres::get_repr(r) };
        }

        return cmd;
    }

    void device_idle(device_t& device)
    {
        vkDeviceWaitIdle(device.handle);
    }

    void destroy(device_t& device)
    {
        vmaDestroyAllocator(device.allocator);
        vkDestroyDevice(device.handle, nullptr);
    }

} // namespace orb::vk
