#include "orb/vk/sync_objects.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/vk_structs.hpp"

namespace orb::vk
{
    auto sync_objects_builder_t::prepare(weak<device_t> device) -> result<sync_objects_builder_t>
    {
        sync_objects_builder_t d;
        d.m_device = device;
        return d;
    }

    auto sync_objects_builder_t::build() -> result<sync_objects_t>
    {
        sync_objects_t objs;
        objs.device = m_device->handle;

        objs.semaphores.resize(m_semaphore_count);
        objs.fences.resize(m_fence_count);

        auto sem_info   = structs::create::semaphore();
        auto fence_info = structs::create::fence();

        for (auto& sem : objs.semaphores)
        {
            if (auto res = vkCreateSemaphore(m_device->handle, &sem_info, nullptr, &sem); res != vkres::ok)
            {
                return error_t { "Could not create semaphore: {}", vkres::get_repr(res) };
            }
        }

        for (auto& fence : objs.fences)
        {
            if (auto res = vkCreateFence(m_device->handle, &fence_info, nullptr, &fence); res != vkres::ok)
            {
                return error_t { "Could not create fence: {}", vkres::get_repr(res) };
            }
        }

        return objs;
    }

    void destroy(sync_objects_t& objs)
    {
        for (auto& sem : objs.semaphores)
        {
            vkDestroySemaphore(objs.device, sem, nullptr);
        }

        for (auto& fence : objs.fences)
        {
            vkDestroyFence(objs.device, fence, nullptr);
        }
    }

} // namespace orb::vk
