#pragma once

#include <orb/result.hpp>
#include <orb/utility.hpp>

#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace orb::vk::details
{
    inline auto get_phy_device_surface_formats(
        VkPhysicalDevice pdevice, VkSurfaceKHR surface) -> result<std::vector<VkSurfaceFormatKHR>>
    {
        ui32                            count = 0;
        std::vector<VkSurfaceFormatKHR> formats;
        vkGetPhysicalDeviceSurfaceFormatsKHR(pdevice, surface, &count, nullptr);
        formats.resize(count);
        auto err = vkGetPhysicalDeviceSurfaceFormatsKHR(pdevice, surface, &count, formats.data());

        if (err != 0)
        {
            return error_t { "Could not get phy device surface formats: VkError {}", (int64_t)err };
        }

        return formats;
    }

    inline auto enum_phy_devices(VkInstance instance) -> result<std::vector<VkPhysicalDevice>>
    {
        ui32                          count = 0;
        std::vector<VkPhysicalDevice> devices;
        vkEnumeratePhysicalDevices(instance, &count, nullptr);
        devices.resize(count);
        auto err = vkEnumeratePhysicalDevices(instance, &count, devices.data());

        if (err != 0) { return error_t { "Could not enumerate physical devices: VkError {}", (int64_t)err }; }

        return devices;
    }

    inline auto enum_queue_family_properties(
        VkPhysicalDevice device) -> result<std::vector<VkQueueFamilyProperties>>
    {
        ui32 count {};
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
        std::vector<VkQueueFamilyProperties> queues(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queues.data());

        return std::move(queues);
    }

    inline auto enum_device_extension_properties(
        VkPhysicalDevice device) -> result<std::vector<VkExtensionProperties>>
    {
        ui32                               count {};
        std::vector<VkExtensionProperties> properties;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
        properties.resize(count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &count, properties.data());
        return std::move(properties);

    }

    template <typename Func, typename TReturnType, typename... Args>
    inline auto vk_call(Func func, Args&&... args) -> result<TReturnType>
    {
        ui32        count = 0;
        TReturnType values;
        func(std::forward<Args>(args)..., &count, nullptr);
        values.resize(count);
        auto err = func(std::forward<Args>(args)..., &count, values.data());

        if (err != 0) { return error_t { "VkError {}", (int64_t)err }; }

        return values;
    }

} // namespace orb::vk::details
