#pragma once

#include "orb/vk/device.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <span>
#include <vector>

namespace orb::vk
{
    struct device_t;

    struct fence_t
    {
        VkDevice device = nullptr;
        VkFence  handle = nullptr;

        [[nodiscard]] auto wait() -> result<void>
        {
            const auto res = vkWaitForFences(device, 1, &handle, VK_TRUE, UINT64_MAX);

            if (res != vkres::ok)
            {
                return error_t { "Failed to wait for fence: {}", vkres::get_repr(res) };
            }

            return {};
        }

        [[nodiscard]] auto reset() -> result<void>
        {
            const auto res = vkResetFences(device, 1, &handle);

            if (res != vkres::ok)
            {
                return error_t { "Failed to reset fence: {}", vkres::get_repr(res) };
            }

            return {};
        }
    };

    struct fences_t
    {

        VkDevice             device = nullptr;
        std::vector<VkFence> handles;

        fences_t() = default;

        fences_t(const fences_t&)                    = delete;
        auto operator=(const fences_t&) -> fences_t& = delete;

        fences_t(fences_t&& other) noexcept
            : device(other.device),
              handles(std::move(other.handles))
        {
        }

        auto operator=(fences_t&& other) noexcept -> fences_t&
        {
            device  = other.device;
            handles = std::move(other.handles);

            return *this;
        }

        ~fences_t()
        {
            destroy();
        }

        void destroy()
        {
            for (auto& fence : handles)
            {
                if (!fence)
                {
                    continue;
                }

                vkDestroyFence(device, fence, nullptr);
                fence = nullptr;
            }
        }

        auto operator[](size_t index) const -> fence_t
        {
            return { device, handles.at(index) };
        }

        [[nodiscard]] auto wait() -> result<void>
        {
            const auto res = vkWaitForFences(device,
                                             handles.size(),
                                             handles.data(),
                                             VK_TRUE,
                                             UINT64_MAX);
            if (res != vkres::ok)
            {
                return error_t { "Failed to wait for fence: {}", vkres::get_repr(res) };
            }

            return {};
        }

        [[nodiscard]] auto reset() -> result<void>
        {
            const auto res = vkResetFences(device,
                                           handles.size(),
                                           handles.data());

            if (res != vkres::ok)
            {
                return error_t { "Failed to reset fence: {}", vkres::get_repr(res) };
            }

            return {};
        }
    };

    class fences_builder_t
    {
    public:
        [[nodiscard]] static auto create(weak<device_t> device, ui32 count) -> result<fences_t>
        {
            fences_t objs;

            objs.device = device->handle;
            objs.handles.resize(count);

            auto fence_info = structs::create::fence();

            for (auto& fence : objs.handles)
            {
                if (auto res = vkCreateFence(device->handle, &fence_info, nullptr, &fence); res != vkres::ok)
                {
                    return error_t { "Could not create fence: {}", vkres::get_repr(res) };
                }
            }

            return objs;
        }
    };
} // namespace orb::vk
