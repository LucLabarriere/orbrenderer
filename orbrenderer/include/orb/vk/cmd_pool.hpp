#pragma once

#include "orb/vk/device.hpp"

#include <orb/box.hpp>
#include <orb/result.hpp>
#include <orb/utility.hpp>

#include <span>
#include <vector>

namespace orb::vk
{
    struct device_t;
    struct gpu_t;

    struct cmd_buffer_t
    {
        VkCommandBuffer handle = nullptr;

        auto reset() -> result<void>
        {
            if (auto res = vkResetCommandBuffer(handle, 0); res != vkres::ok)
            {
                return error_t { "Could not reset command buffer: {}", vkres::get_repr(res) };
            }

            return {};
        }

        auto begin_one_time() -> result<void>
        {
            auto info  = structs::cmd_buffer_begin();
            info.flags = command_buffer_usage_flags::one_time_submit;

            // Resetting cmd buffer
            if (auto res = vkResetCommandBuffer(handle, 0); res != vkres::ok)
            {
                return error_t { "Could not reset command buffer: {}", vkres::get_repr(res) };
            }

            if (auto res = vkBeginCommandBuffer(handle, &info); res != vkres::ok)
            {
                return error_t { "Could not start recording cmd buffer: {}", vkres::get_repr(res) };
            }

            return {};
        }

        auto copy_buffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) -> result<void>
        {
            VkBufferCopy copy_region {
                .srcOffset = 0,
                .dstOffset = 0,
                .size      = size,
            };

            vkCmdCopyBuffer(handle, src, dst, 1, &copy_region);

            return {};
        }

        auto end() -> result<void>
        {
            if (auto res = vkEndCommandBuffer(handle); res != vkres::ok)
            {
                return error_t { "Could not end command buffer: {}", vkres::get_repr(res) };
            }

            return {};
        }
    };

    struct cmd_buffers_t
    {
        std::vector<VkCommandBuffer> handles;

        [[nodiscard]] auto get(size_t offset) -> result<cmd_buffer_t>
        {
            if (offset >= handles.size())
            {
                return error_t { "Out of range" };
            }

            return cmd_buffer_t { .handle = handles[offset] };
        }
    };

    struct cmd_pool_t
    {
        VkCommandPool handle   = nullptr;
        VkDevice      device   = nullptr;
        ui32          qf_index = 0;

        cmd_pool_t() = default;

        cmd_pool_t(const cmd_pool_t&)                    = delete;
        auto operator=(const cmd_pool_t&) -> cmd_pool_t& = delete;

        cmd_pool_t(cmd_pool_t&& other) noexcept
        {
            destroy();

            handle   = other.handle;
            device   = other.device;
            qf_index = other.qf_index;

            other.handle = nullptr;
        }

        auto operator=(cmd_pool_t&& other) noexcept -> cmd_pool_t&
        {
            handle   = other.handle;
            device   = other.device;
            qf_index = other.qf_index;

            other.handle = nullptr;

            return *this;
        }

        ~cmd_pool_t()
        {
            destroy();
        }

        void destroy()
        {
            if (!handle)
            {
                return;
            }

            vkDestroyCommandPool(device, handle, nullptr);
            handle = nullptr;
        }

        [[nodiscard]] auto alloc_cmds(size_t                    count,
                        cmd_buffer_levels::enum_t level = cmd_buffer_levels::primary)
            -> result<cmd_buffers_t>
        {
            std::vector<VkCommandBuffer> cmds;
            cmds.resize(count);

            auto cmd_info               = structs::create::cmd_buffer();
            cmd_info.commandBufferCount = cmds.size();
            cmd_info.commandPool        = handle;
            cmd_info.level              = level;

            if (auto res = vkAllocateCommandBuffers(device, &cmd_info, cmds.data());
                res != vkres::ok)
            {
                return error_t { "Could not allocate command buffer: {}", vkres::get_repr(res) };
            }

            return cmd_buffers_t { .handles = std::move(cmds) };
        }
    };

    class cmd_pool_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device, ui32 qf_index)
            -> result<cmd_pool_builder_t>
        {
            cmd_pool_builder_t d;
            d.m_device   = device;
            d.m_qf_index = qf_index;
            return d;
        }

        [[nodiscard]] auto build() -> result<box<cmd_pool_t>>
        {
            auto pool      = make_box<cmd_pool_t>();
            pool->device   = m_device->handle;
            pool->qf_index = m_qf_index;

            auto cmd_pool_info             = structs::create::cmd_pool();
            cmd_pool_info.queueFamilyIndex = m_qf_index;
            cmd_pool_info.flags            = m_flags;

            if (auto res = vkCreateCommandPool(m_device->handle, &cmd_pool_info, nullptr, &pool->handle);
                res != vkres::ok)
            {
                return error_t { "Could not create command pool: {}", vkres::get_repr(res) };
            }

            return pool;
        }

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
        [[nodiscard]] static auto prepare() -> submit_helper_t { return {}; }

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

        auto wait_stage(vk::pipeline_stage_flags::enum_t stage) -> submit_helper_t&
        {
            m_wait_stage |= (ui32)stage;
            return *this;
        }

        [[nodiscard]] auto submit(VkQueue queue, VkFence fence = nullptr) -> result<void>
        {
            m_info.pWaitDstStageMask = &m_wait_stage;

            if (auto res = vkQueueSubmit(queue, 1, &m_info, fence); res != vkres::ok)
            {
                return error_t { "Failed to submit command buffer: {}", vkres::get_repr(res) };
            }

            return {};
        }

    private:
        VkSubmitInfo         m_info = structs::submit();
        VkPipelineStageFlags m_wait_stage {};
    };
} // namespace orb::vk
