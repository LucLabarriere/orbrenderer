#include "orb/vk/render.hpp"

#include "orb/vk/render_pass.hpp"
#include "orb/vk/swapchain.hpp"

namespace orb::vk
{
    auto render_begin(render_info_t& info) -> result<void>
    {
        if (auto res = vkWaitForFences(info.device, 1, &info.wait_fence, VK_TRUE, 1000); res != vkres::ok)
        {
            return error_t { "Could not wait for ImGui fence: {}", vkres::get_repr(res) };
        }

        if (auto res = vkResetFences(info.device, 1, &info.wait_fence); res != vkres::ok)
        {
            return error_t { "Could not reset ImGui fences: {}", vkres::get_repr(res) };
        }

        auto cmd_begin_info = structs::cmd_buffer_begin();
        if (auto res = vkBeginCommandBuffer(info.cmd, &cmd_begin_info); res != vkres::ok)
        {
            return error_t { "Could not start recording cmd buffer: {}", vkres::get_repr(res) };
        }

        auto pass_begin_info                     = structs::render_pass_begin();
        pass_begin_info.renderPass               = info.pass;
        pass_begin_info.framebuffer              = info.fb;
        pass_begin_info.renderArea.extent.width  = info.swapchain->width;
        pass_begin_info.renderArea.extent.height = info.swapchain->height;
        pass_begin_info.renderArea.offset        = { .x = 0, .y = 0 },

        pass_begin_info.clearValueCount = 1;
        pass_begin_info.pClearValues    = &info.clear_color;
        vkCmdBeginRenderPass(info.cmd, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        return {};
    }

    auto render_end(render_info_t& info) -> result<void>
    {
        vkCmdEndRenderPass(info.cmd);

        if (auto r = vkEndCommandBuffer(info.cmd); r != vkres::ok)
        {
            return error_t { "Could not finish recording cmd buffer" };
        }

        VkPipelineStageFlags wait_stage  = pipeline_stage_flags::color_attachment_output;
        auto                 submit_info = structs::submit();
        submit_info.waitSemaphoreCount   = info.wait_semaphores.size();
        submit_info.pWaitSemaphores      = info.wait_semaphores.data();
        submit_info.pWaitDstStageMask    = &wait_stage;
        submit_info.commandBufferCount   = 1;
        submit_info.pCommandBuffers      = &info.cmd;
        submit_info.signalSemaphoreCount = info.signal_semaphores.size();
        submit_info.pSignalSemaphores    = info.signal_semaphores.data();

        if (auto r = vkQueueSubmit(info.queue, 1, &submit_info, info.wait_fence); r != vkres::ok)
        {
            return error_t { "Could not submit render: {}", vkres::get_repr(r) };
        }

        return {};
    }

    void describe(const render_info_t& info)
    {
        println("  --- Begin render_info_t description");
        println("  - Device:      {}", orb::ptr(info.device));
        println("  - Pass:        {}", orb::ptr(info.pass));
        println("  - Wait fence:  {}", orb::ptr(info.wait_fence));
        println("  - Queue:       {}", orb::ptr(info.queue));
        println("  - Cmd:         {}", orb::ptr(info.cmd));
        println("  - Swapchain:   {}", orb::ptr(info.swapchain->handle));
        println("  - Framebuffer: {}", orb::ptr(info.fb));

        println("  - Signal semaphore count: {}", info.signal_semaphores.size());
        for (const auto sem : info.signal_semaphores)
        {
            println("     * {}", orb::ptr(sem));
        }

        println("  - Wait semaphore count: {}", info.wait_semaphores.size());
        for (const auto sem : info.wait_semaphores)
        {
            println("     * {}", orb::ptr(sem));
        }

        println("  --- End   render_info_t description");
    }

} // namespace orb::vk
