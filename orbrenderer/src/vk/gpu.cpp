#include "orb/vk/gpu.hpp"

#include <orb/flux.hpp>

#include <vector>

namespace orb::vk
{
    auto available_gpus_t::create(VkInstance instance) -> result<available_gpus_t>
    {
        available_gpus_t d;
        auto&            gpus = d.m_gpus;

        ui32                          count = 0;
        std::vector<VkPhysicalDevice> gpu_handles;
        vkEnumeratePhysicalDevices(instance, &count, nullptr);
        gpu_handles.resize(count);
        auto err = vkEnumeratePhysicalDevices(instance, &count, gpu_handles.data());

        if (err != 0) { return error_t { "Could not enumerate physical devices: VkError {}", (int64_t)err }; }

        for (VkPhysicalDevice& device : gpu_handles)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);

            gpus.push_back(make_box<gpu_t>(gpu_t {
                .handle            = device,
                .api_version       = properties.apiVersion,
                .driver_version    = properties.driverVersion,
                .vendor_id         = properties.vendorID,
                .device_id         = properties.deviceID,
                .device_type       = properties.deviceType,
                .name              = properties.deviceName,
                .limits            = properties.limits,
                .sparse_properties = properties.sparseProperties,
            }));
        }

        if (gpus.empty()) { return error_t { "No GPU found" }; }

        for (auto& gpu : gpus)
        {
            ui32 count {};
            vkGetPhysicalDeviceQueueFamilyProperties(gpu->handle, &count, nullptr);
            std::vector<VkQueueFamilyProperties> qf_properties;
            qf_properties.resize(count);
            vkGetPhysicalDeviceQueueFamilyProperties(gpu->handle, &count, qf_properties.data());
            gpu->queue_families =
                flux::enumerate(qf_properties)
                    .map([](auto&& pair) {
                        const auto& [index, properties] = pair;
                        return queue_family_t { .index = (ui32)index, .properties = properties };
                    })
                    .to<std::vector>();
        }

        return d;
    }

    void describe(const vk::gpu_t& gpu)
    {
        println("- GPU description");
        println("  * GPU {}: {}", gpu.device_id, gpu.name);
        println("  * Driver: {}", gpu.driver_version);
        println("  * Vulkan handle: {}", orb::ptr(gpu.handle));
        println("  * GPU type: {}", gpu_types::strings[(size_t)gpu.device_type]);
        println("  * Available queue families");
        for (const auto& [i, qf] : flux::enumerate(gpu.queue_families))
        {
            print("    - Queue family {}:", i);
            if (qf.properties.queueFlags & vk::queue_families::graphics) { print(" Graphics"); }
            if (qf.properties.queueFlags & vk::queue_families::transfer) { print(" Transfer"); }
            if (qf.properties.queueFlags & vk::queue_families::compute) { print(" Compute"); }
            println("");
            println("       Count: {}", qf.properties.queueCount);
        }
    };

} // namespace orb::vk
