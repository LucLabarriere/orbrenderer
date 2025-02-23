#include "orb/vk/cmd_pool.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/gpu.hpp"
#include "orb/vk/vk_structs.hpp"

namespace orb::vk
{
    auto cmd_buffers_t::reset(size_t offset) -> VkResult
    {
        VkCommandBuffer cmd = handles[offset];
        return vkResetCommandBuffer(cmd, 0);
    }

    auto cmd_buffers_t::begin_one_time(size_t offset) -> std::tuple<VkCommandBuffer, VkResult>
    {
        VkCommandBuffer cmd  = handles[offset];
        auto            info = structs::cmd_buffer_begin();
        info.flags           = command_buffer_usage_flags::one_time_submit;

        if (auto res = vkResetCommandBuffer(cmd, 0); res != vkres::ok)
        {
            return { cmd, res };
        }

        return { cmd, vkBeginCommandBuffer(cmd, &info) };
    }

    auto cmd_pool_builder_t::prepare(weak<device_t> device, ui32 qf_index) -> result<cmd_pool_builder_t>
    {
        cmd_pool_builder_t d;
        d.m_device   = device;
        d.m_qf_index = qf_index;
        return d;
    }

    auto cmd_pool_builder_t::build() -> result<cmd_pool_t>
    {
        cmd_pool_t pool;
        pool.device   = m_device->handle;
        pool.qf_index = m_qf_index;

        auto cmd_pool_info             = structs::create::cmd_pool();
        cmd_pool_info.queueFamilyIndex = m_qf_index;
        cmd_pool_info.flags            = m_flags;

        if (auto res = vkCreateCommandPool(m_device->handle, &cmd_pool_info, nullptr, &pool.handle);
            res != vkres::ok)
        {
            return error_t { "Could not create command pool: {}", vkres::get_repr(res) };
        }

        return pool;
    }

    auto alloc_cmds(cmd_pool_t& pool, size_t count, cmd_buffer_levels::enum_t level) -> result<cmd_buffers_t>
    {

        std::vector<VkCommandBuffer> cmds;
        cmds.resize(count);

        auto cmd_info               = structs::create::cmd_buffer();
        cmd_info.commandBufferCount = cmds.size();
        cmd_info.commandPool        = pool.handle;
        cmd_info.level              = level;

        if (auto res = vkAllocateCommandBuffers(pool.device, &cmd_info, cmds.data());
            res != vkres::ok)
        {
            return error_t { "Could not allocate command buffer: {}", vkres::get_repr(res) };
        }

        return cmd_buffers_t { .handles = std::move(cmds) };
    }

    auto end(VkCommandBuffer& cmd) -> result<void>
    {
        if (auto res = vkEndCommandBuffer(cmd); res != vkres::ok)
        {
            return error_t { "Could not end command buffer: {}", vkres::get_repr(res) };
        }

        return {};
    }

    void destroy(cmd_pool_t& pool)
    {
        vkDestroyCommandPool(pool.device, pool.handle, nullptr);
        pool.handle = nullptr;
    }
} // namespace orb::vk
