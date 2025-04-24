#pragma once

#include "orb/vk/core.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <functional>
#include <span>
#include <vector>

namespace orb::vk
{
    struct instance_t
    {
        VkInstance               handle {};
        VkDebugUtilsMessengerEXT debug_utils {};

        instance_t() = default;

        instance_t(const instance_t&)                    = delete;
        auto operator=(const instance_t&) -> instance_t& = delete;

        instance_t(instance_t&& other) noexcept
        {
            destroy();

            handle      = other.handle;
            debug_utils = other.debug_utils;

            other.handle      = nullptr;
            other.debug_utils = nullptr;
        }

        auto operator=(instance_t&& other) noexcept -> instance_t&
        {
            destroy();

            handle      = other.handle;
            debug_utils = other.debug_utils;

            other.handle      = nullptr;
            other.debug_utils = nullptr;

            return *this;
        }

        ~instance_t()
        {
            destroy();
        }

        void destroy()
        {
            if (debug_utils)
            {
                auto destroy_deb_callback_fn = proc_addresses::destroy_deb_utils(handle);
                destroy_deb_callback_fn(handle, debug_utils, nullptr);
                debug_utils = nullptr;
            }

            if (handle)
            {
                vkDestroyInstance(handle, nullptr);
                handle = nullptr;
            }
        }
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

        auto molten_vk(bool enable) -> instance_builder_t&
        {
            constexpr auto ext = vk::khr_extensions::portability_enumeration;

            if (enable && is_ext_available(ext))
            {
                add_extension(ext);
                create_info.flags |= vkenum(instance_create::portability);
            }

            return *this;
        }

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

        [[nodiscard]] auto build() -> result<box<instance_t>>;

    private:
        instance_builder_t() = default;
    };
} // namespace orb::vk
