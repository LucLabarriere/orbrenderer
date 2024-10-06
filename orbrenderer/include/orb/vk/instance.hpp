#pragma once

#include <orb/result.hpp>

#include "orb/vk/vk_structs.hpp"

#include <functional>
#include <span>
#include <vector>

namespace orb::vk
{
    struct instance_t
    {
        VkInstance               handle {};
        VkDebugReportCallbackEXT debug_report {};
    };

    class instance_builder_t
    {
    public:
        std::vector<const char*>           extensions;
        std::vector<const char*>           val_layers;
        VkInstanceCreateInfo               create_info {};
        std::vector<VkExtensionProperties> ext_properties;

        [[nodiscard]] static auto prepare() -> result<instance_builder_t>;

        [[nodiscard]] auto is_ext_available(std::string_view extension) -> bool;
        auto               add_glfw_required_extensions() -> instance_builder_t&;

        auto debug_layer(const char* layer) -> instance_builder_t&
        {
            val_layers.push_back(layer);
            return *this;
        }

        auto add_extension(const char* ext) -> instance_builder_t&
        {
            extensions.push_back(ext);
            return *this;
        }

        [[nodiscard]] auto build() -> result<instance_t>;

    private:
        instance_builder_t() = default;
    };

    void destroy(instance_t& instance);

} // namespace orb::vk
