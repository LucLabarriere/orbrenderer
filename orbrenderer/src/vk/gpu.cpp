#include "orb/vk/gpu.hpp"

#include <orb/flux.hpp>

#include <vector>

namespace orb::vk
{
    auto queue_family_map_t::create(std::span<box<queue_family_t>> queues)
        -> box<queue_family_map_t>
    {
        auto family_map = make_box<queue_family_map_t>();

        for (auto [i, qf] : flux::enumerate_mut(queues))
        {
            if (qf->properties.queueFlags & vkenum(queue_family::graphics))
            {
                constexpr size_t index = queue_family_map_t::get_index(queue_family::graphics);
                family_map->m_families[index].push_back(qf.getmut());
            }
            if (qf->properties.queueFlags & vkenum(queue_family::transfer))
            {
                constexpr size_t index = queue_family_map_t::get_index(queue_family::transfer);
                family_map->m_families[index].push_back(qf.getmut());
            }
            if (qf->properties.queueFlags & vkenum(queue_family::compute))
            {
                constexpr size_t index = queue_family_map_t::get_index(queue_family::compute);
                family_map->m_families[index].push_back(qf.getmut());
            }
            if (qf->properties.queueFlags & vkenum(queue_family::sparse))
            {
                constexpr size_t index = queue_family_map_t::get_index(queue_family::sparse);
                family_map->m_families[index].push_back(qf.getmut());
            }
        }

        return family_map;
    }

    auto gpu_selector_t::prepare(VkInstance instance) -> result<gpu_selector_t>
    {
        gpu_selector_t d;
        auto&          gpus = d.m_gpus;

        ui32                          count = 0;
        std::vector<VkPhysicalDevice> gpu_handles;
        vkEnumeratePhysicalDevices(instance, &count, nullptr);
        gpu_handles.resize(count);
        auto res = vkEnumeratePhysicalDevices(instance, &count, gpu_handles.data());

        if (res != vk::vkres::ok)
        {
            return error_t { "Could not enumerate physical devices: VkError {}", vk::vkres::get_repr(res) };
        }

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
                .device_type       = gpu_type { (ui32)properties.deviceType },
                .name              = properties.deviceName,
                .limits            = properties.limits,
                .sparse_properties = properties.sparseProperties,
            }));
        }

        if (gpus.empty())
        {
            return error_t { "No GPU found" };
        }

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
                        return make_box<queue_family_t>((ui32)pair.first, pair.second);
                    })
                    .to<std::vector>();

            gpu->queue_family_map = queue_family_map_t::create(gpu->queue_families);
        }

        return d;
    }

    void gpu_t::describe() const
    {
        fmt::println("- GPU description");
        fmt::println("  * GPU {}: {}", device_id, name);
        fmt::println("  * Driver: {}.{}.{}",
                     VK_VERSION_MAJOR(driver_version),
                     VK_VERSION_MINOR(driver_version),
                     VK_VERSION_PATCH(driver_version));
        fmt::println("  * Vulkan version: {}.{}.{}",
                     VK_VERSION_MAJOR(api_version),
                     VK_VERSION_MINOR(api_version),
                     VK_VERSION_PATCH(api_version));
        fmt::println("  * Vulkan handle: {}", fmt::ptr(handle));
        fmt::println("  * GPU type: {}", gpu_type_names.at(device_type));
        fmt::println("  * Available queue families");

        for (const auto& [i, qf] : flux::enumerate(queue_families))
        {
            fmt::print("    - Queue family {}:", i);
            if (qf->properties.queueFlags & vkenum(queue_family::graphics)) { fmt::print(" Graphics"); }
            if (qf->properties.queueFlags & vkenum(queue_family::transfer)) { fmt::print(" Transfer"); }
            if (qf->properties.queueFlags & vkenum(queue_family::compute)) { fmt::print(" Compute"); }

            fmt::println("");
            fmt::println("       Count: {}", qf->properties.queueCount);
        }
    };

} // namespace orb::vk
