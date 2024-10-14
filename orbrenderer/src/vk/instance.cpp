#include "orb/vk/instance.hpp"
#include "orb/vk/vk_types.hpp"

#include <orb/eval.hpp>
#include <orb/print_time.hpp>
#include <orb/utility.hpp>

#include "glfw/glfw_header.hpp"

namespace
{
    inline VKAPI_ATTR auto VKAPI_CALL debug_report(VkDebugReportFlagsEXT      flags,
                                                   VkDebugReportObjectTypeEXT objectType,
                                                   uint64_t                   object,
                                                   size_t                     location,
                                                   int32_t                    messageCode,
                                                   const char*                pLayerPrefix,
                                                   const char*                pMessage,
                                                   void*                      pUserData) -> VkBool32
    {
        (void)flags;
        (void)object;
        (void)location;
        (void)messageCode;
        (void)pUserData;
        (void)pLayerPrefix;
        orb::println("Vulkan error from {}: {}", (orb::i64)objectType, pMessage);
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

    auto vk::instance_builder_t::build() -> result<instance_t>
    {
        instance_t data;
        create_info.enabledExtensionCount   = (ui32)extensions.size();
        create_info.ppEnabledExtensionNames = extensions.data();

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

        auto res = vkCreateInstance(&create_info, nullptr, &data.handle);
        if (res != VK_SUCCESS) { return error_t { "Could not create Vulkan instance: {}", vkres::get_repr(res) }; }

        begin_chrono();
        auto create_deb_callback_fn = proc_addresses::create_deb_callback(data.handle);

        auto debug_report_ci        = structs::create::debug_report_callback();
        debug_report_ci.pfnCallback = debug_report;

        auto debug_res = create_deb_callback_fn(data.handle, &debug_report_ci, nullptr, &data.debug_report);
        if (debug_res != VK_SUCCESS)
        {
            return error_t { "Could not create debug report callback: {}", (i64)debug_res };
        }

        print_chrono("- Create debug report callback: {}");

        return data;
    }

    void destroy(instance_t& instance)
    {
        if (instance.debug_report)
        {
            auto destroy_deb_callback_fn = proc_addresses::destroy_deb_callback(instance.handle);
            destroy_deb_callback_fn(instance.handle, instance.debug_report, nullptr);
        }
        vkDestroyInstance(instance.handle, nullptr);
    }
} // namespace orb::vk
