#pragma once

#include <vector>

using VkSemaphore = struct VkSemaphore_T;
using VkFence = struct VkFence_T;

namespace orb::vk
{
    class sync_objects_t
    {
    public:

    private:
        std::vector<VkSemaphore> m_semaphores;
        std::vector<VkFence> m_fences;
    };
} // namespace orb::vk
