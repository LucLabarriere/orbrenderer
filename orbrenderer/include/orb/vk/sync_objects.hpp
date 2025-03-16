#pragma once

#include "orb/vk/device.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>

#include <span>
#include <vector>

namespace orb::vk
{
    struct device_t;

    struct fences_t
    {
        std::span<VkFence> handles;
        VkDevice           device = nullptr;

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

    struct semaphores_t
    {
        std::span<VkSemaphore> handles;
        VkDevice               device = nullptr;
    };

    struct sync_objects_t
    {

        VkDevice                 device = nullptr;
        std::vector<VkSemaphore> semaphore_handles;
        std::vector<VkFence>     fence_handles;

        sync_objects_t() = default;

        sync_objects_t(const sync_objects_t&)                    = delete;
        auto operator=(const sync_objects_t&) -> sync_objects_t& = delete;

        sync_objects_t(sync_objects_t&& other) noexcept
            : device(other.device),
              semaphore_handles(std::move(other.semaphore_handles)),
              fence_handles(std::move(other.fence_handles))
        {
        }

        auto operator=(sync_objects_t&& other) noexcept -> sync_objects_t&
        {
            device            = other.device;
            semaphore_handles = std::move(other.semaphore_handles);
            fence_handles     = std::move(other.fence_handles);

            return *this;
        }

        ~sync_objects_t()
        {
            destroy();
        }

        void destroy()
        {
            for (auto& fence : fence_handles)
            {
                if (!fence)
                {
                    continue;
                }

                vkDestroyFence(device, fence, nullptr);
                fence = nullptr;
            }

            for (auto& semaphore : semaphore_handles)
            {
                if (!semaphore)
                {
                    continue;
                }

                vkDestroySemaphore(device, semaphore, nullptr);
                semaphore = nullptr;
            }
        }

        [[nodiscard]] auto fences(ui32 offset, ui32 count) -> fences_t
        {
            return {
                .handles = std::span { fence_handles }.subspan(offset, count),
                .device  = device,
            };
        }

        [[nodiscard]] auto semaphores(ui32 offset, ui32 count) -> semaphores_t
        {
            return {
                .handles = std::span { semaphore_handles }.subspan(offset, count),
                .device  = device,
            };
        }
    };

    class sync_objects_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device) -> result<sync_objects_builder_t>
        {
            sync_objects_builder_t d;
            d.m_device = device;
            return d;
        }

        [[nodiscard]] auto build() -> result<sync_objects_t>
        {
            sync_objects_t objs;
            objs.device = m_device->handle;

            objs.semaphore_handles.resize(m_semaphore_count);
            objs.fence_handles.resize(m_fence_count);

            auto sem_info   = structs::create::semaphore();
            auto fence_info = structs::create::fence();

            for (auto& sem : objs.semaphore_handles)
            {
                if (auto res = vkCreateSemaphore(m_device->handle, &sem_info, nullptr, &sem); res != vkres::ok)
                {
                    return error_t { "Could not create semaphore: {}", vkres::get_repr(res) };
                }
            }

            for (auto& fence : objs.fence_handles)
            {
                if (auto res = vkCreateFence(m_device->handle, &fence_info, nullptr, &fence); res != vkres::ok)
                {
                    return error_t { "Could not create fence: {}", vkres::get_repr(res) };
                }
            }

            return objs;
        }

        auto semaphores(ui32 count) -> sync_objects_builder_t&
        {
            m_semaphore_count = count;
            return *this;
        }

        auto fences(ui32 count) -> sync_objects_builder_t&
        {
            m_fence_count = count;
            return *this;
        }

    private:
        weak<device_t> m_device          = nullptr;
        ui32           m_semaphore_count = 0;
        ui32           m_fence_count     = 0;
    };
} // namespace orb::vk
