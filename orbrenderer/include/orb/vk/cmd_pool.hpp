#pragma once

#include <orb/vk/vk_structs.hpp>

#include <orb/box.hpp>
#include <orb/result.hpp>
#include <orb/utility.hpp>

#include <span>
#include <vector>

namespace orb::vk
{
    struct device_t;
    struct gpu_t;

    struct cmd_pool_t
    {
        VkCommandPool handle   = nullptr;
        VkDevice      device   = nullptr;
        ui32          qf_index = 0;
    };

    struct cmd_buffers_t
    {
        std::vector<VkCommandBuffer> handles;

        auto begin_one_time(size_t offset) -> std::tuple<VkCommandBuffer, VkResult>;
    };

    class cmd_pool_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t>, ui32 qf_index) -> result<cmd_pool_builder_t>;
        [[nodiscard]] auto        build() -> result<cmd_pool_t>;

        auto flag(command_pool_create_flags::enum_t flag) -> cmd_pool_builder_t&
        {
            m_flags |= static_cast<ui32>(flag);
            return *this;
        };

    private:
        weak<device_t> m_device   = nullptr;
        ui32           m_qf_index = 0;

        ui32 m_flags {};
    };

    class submit_helper_t
    {
    public:
        auto wait_semaphores(std::span<VkSemaphore> semaphores) -> submit_helper_t&
        {
            m_info.waitSemaphoreCount = semaphores.size();
            m_info.pWaitSemaphores    = semaphores.data();
            return *this;
        }

        auto signal_semaphores(std::span<VkSemaphore> semaphores) -> submit_helper_t&
        {
            m_info.signalSemaphoreCount = semaphores.size();
            m_info.pSignalSemaphores    = semaphores.data();
            return *this;
        }

        auto cmd_buffers(std::span<VkCommandBuffer> cmds) -> submit_helper_t&
        {
            m_info.commandBufferCount = cmds.size();
            m_info.pCommandBuffers    = cmds.data();
            return *this;
        }

        auto cmd_buffer(VkCommandBuffer* cmd) -> submit_helper_t&
        {
            m_info.commandBufferCount = 1;
            m_info.pCommandBuffers    = cmd;
            return *this;
        }

        auto wait_stage(VkPipelineStageFlags* stage) -> submit_helper_t&
        {
            m_info.pWaitDstStageMask = stage;
            return *this;
        }

        auto submit(VkQueue queue, VkFence fence = nullptr) -> result<void>
        {
            if (auto res = vkQueueSubmit(queue, 1, &m_info, fence); res != vkres::ok)
            {
                return error_t { "Failed to submit command buffer: {}", vkres::get_repr(res) };
            }

            return {};
        }

    private:
        VkSubmitInfo m_info = structs::submit();
    };

    auto alloc_cmds(cmd_pool_t&, size_t, cmd_buffer_levels::enum_t) -> result<cmd_buffers_t>;
    auto end(VkCommandBuffer& cmd) -> result<void>;
    void destroy(cmd_pool_t&);

} // namespace orb::vk
