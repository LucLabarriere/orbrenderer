#include "orb/vk/instance.hpp"
#include "orb/vk/vk_types.hpp"

#include <orb/eval.hpp>
#include <orb/print_time.hpp>
#include <orb/utility.hpp>

#include "glfw/glfw_header.hpp"

namespace
{
    VKAPI_ATTR auto VKAPI_CALL debug_report(
        VkDebugUtilsMessageSeverityFlagBitsEXT      msg_sev,
        VkDebugUtilsMessageTypeFlagsEXT             msg_type,
        const VkDebugUtilsMessengerCallbackDataEXT* data,
        void*                                       user_data) -> VkBool32
    {
        namespace flags = orb::vk::debug_utils_message_severity_flags;
        namespace col   = orb::colors;

        orb::ui32 severity = orb::eval | [&] {
            if ((msg_sev & flags::error) == flags::error) return flags::error;
            if ((msg_sev & flags::warning) == flags::warning) return flags::warning;
            if ((msg_sev & flags::info) == flags::info) return flags::info;
            if ((msg_sev & flags::verbose) == flags::verbose) return flags::verbose;
            return flags::verbose;
        };

        switch (severity)
        {
        case orb::vk::debug_utils_message_severity_flags::verbose:
        case orb::vk::debug_utils_message_severity_flags::info:
        {
            break;
        }
        case orb::vk::debug_utils_message_severity_flags::warning:
        {
            orb::println("WARNING: {}", (uint32_t)msg_sev);
            break;
        }
        case orb::vk::debug_utils_message_severity_flags::error:
        {
            orb::println("{}Vk error:{} {} ({})", col::red, col::reset, data->pMessageIdName, data->messageIdNumber);
            for (auto obj : std::span { data->pObjects, data->objectCount })
            {
                if (obj.pObjectName)
                {
                    orb::println("Obj: {}{}{}", col::red, obj.pObjectName, col::reset);
                }
            }
            orb::println("{}", data->pMessage);
            break;
        }
        default:
        {
            orb::println("DEBUG: {}", (uint32_t)msg_sev);
            break;
        }
        }

        return VK_FALSE;
    }

    inline auto enum_instance_ext_props() -> orb::result<std::vector<VkExtensionProperties>>
    {
        orb::ui32                          count = 0;
        std::vector<VkExtensionProperties> props;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        props.resize(count);
        auto err = vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());

        if (err != 0)
        {
            return orb::error_t { "Could not enumerate instance extension properties: VkError {}" };
        }

        return props;
    }

} // namespace

namespace orb::vk
{
    auto vk::instance_builder_t::is_ext_available(std::string_view extension) -> bool
    {
        for (const VkExtensionProperties& p : ext_properties)
        {
            if (p.extensionName == extension) return true;
        }

        return false;
    }

    auto vk::instance_builder_t::add_glfw_required_extensions() -> instance_builder_t&
    {
        ui32         count     = 0;
        const char** glfw_exts = glfwGetRequiredInstanceExtensions(&count);
        std::span    exts { glfw_exts, count };

        for (const auto ext : exts)
        {
            extensions.push_back(ext);
        }

        return *this;
    }

    auto vk::instance_builder_t::prepare() -> result<instance_builder_t>
    {
        instance_builder_t b;
        b.create_info = structs::create::instance();

        auto ext_properties_res = enum_instance_ext_props();
        if (!ext_properties_res) return ext_properties_res.error();
        b.ext_properties = ext_properties_res.value();

        return b;
    }

    auto vk::instance_builder_t::build() -> result<box<instance_t>>
    {
        auto instance = make_box<instance_t>();

        create_info.enabledExtensionCount   = (ui32)extensions.size();
        create_info.ppEnabledExtensionNames = extensions.data();

        println("- Vulkan instance extensions:");
        for (const char* ext : extensions)
        {
            orb::println("  * {}", ext);
        }

        ui32 layer_count {};
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> avail_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, avail_layers.data());

        for (const char* request : val_layers)
        {
            const bool found = orb::eval | [&] {
                for (auto& avail : avail_layers)
                {
                    if (strcmp(request, avail.layerName) == 0) return true;
                }

                return false;
            };

            if (!found) return error_t { "Could not find validation layer: {}", request };
        }

        create_info.enabledLayerCount   = val_layers.size();
        create_info.ppEnabledLayerNames = val_layers.data();

        auto create_deb_info            = structs::create::debug_utils();
        create_deb_info.pfnUserCallback = debug_report;
        create_info.pNext               = &create_deb_info;

        auto res = vkCreateInstance(&create_info, nullptr, &instance->handle);
        if (res != VK_SUCCESS) { return error_t { "Could not create Vulkan instance: {}", vkres::get_repr(res) }; }

        // begin_chrono();

        auto create_deb_utils_fn = proc_addresses::create_deb_utils(instance->handle);
        if (auto r = create_deb_utils_fn(instance->handle, &create_deb_info, nullptr, &instance->debug_utils); r != vkres::ok)
        {
            return error_t { "Could not create debug utils messenger: {}", vkres::get_repr(res) };
        }

        return instance;
    }

    void destroy(instance_t& instance)
    {
        if (instance.debug_utils)
        {
            auto destroy_deb_callback_fn = proc_addresses::destroy_deb_utils(instance.handle);
            destroy_deb_callback_fn(instance.handle, instance.debug_utils, nullptr);
            instance.debug_utils = nullptr;
        }

        vkDestroyInstance(instance.handle, nullptr);
        instance.handle = nullptr;
    }
} // namespace orb::vk
