#include <array>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <iostream>
#include <vector>

#include <orb/eval.hpp>
#include <orb/flux.hpp>

#include <GLFW/glfw3.h>
#include <orb/glfw.hpp>
#include <orb/vk.hpp>
#include <orb/vk/attachments.hpp>
#include <orb/vk/cmd_pool.hpp>
#include <orb/vk/desc_pool.hpp>
#include <orb/vk/framebuffers.hpp>
#include <orb/vk/imgui.hpp>
#include <orb/vk/render_pass.hpp>
#include <orb/vk/subpasses.hpp>
#include <orb/vk/sync_objects.hpp>

using namespace orb;

namespace
{
    auto create_imgui_pass(VkDevice device, VkFormat sc_img_format) -> vk::render_pass_t
    {
        vk::attachments_t attachments;
        vk::subpasses_t   subpasses;

        attachments.add({
            .img_format        = sc_img_format,
            .samples           = vk::sample_count_flags::_1,
            .load_ops          = vk::attachment_load_ops::clear,
            .store_ops         = vk::attachment_store_ops::store,
            .stencil_load_ops  = vk::attachment_load_ops::dont_care,
            .stencil_store_ops = vk::attachment_store_ops::dont_care,
            .initial_layout    = vk::image_layouts::undefined,
            .final_layout      = vk::image_layouts::present_src_khr,
            .attachment_layout = vk::image_layouts::color_attachment_optimal,
        });

        const auto [color_descs, color_refs] = attachments.spans(0, 1);

        subpasses.add_subpass({
            .bind_point = vk::pipeline_bind_points::graphics,
            .color_refs = color_refs,
        });

        subpasses.add_dependency({
            .src        = vk::subpass_external,
            .dst        = 0,
            .src_stage  = vk::pipeline_stage_flags::color_attachment_output,
            .dst_stage  = vk::pipeline_stage_flags::color_attachment_output,
            .src_access = 0,
            .dst_access = vk::access_flags::color_attachment_write,
        });

        vk::render_pass_t imgui_pass = vk::render_pass_builder_t::prepare(device)
                                           .unwrap()
                                           .build(subpasses, attachments)
                                           .unwrap();

        return imgui_pass;
    }
} // namespace

auto main() -> int
{
    constexpr auto portability = vk::khr_extensions::portability_enumeration;

    try
    {
        // Initialize GLFW and the window
        glfw::driver_t::initialize().throw_if_error();
        glfw::window_t window = glfw::driver_t::create_window_for_vk().unwrap();

        // Initialize the vulkan instance
        auto instance_builder = vk::instance_builder_t::prepare().unwrap();

        if (instance_builder.is_ext_available(portability))
        {
            instance_builder.add_extension(portability);
            instance_builder.create_info.flags |= vk::instance_create::portability;
        }

        vk::instance_t instance = instance_builder.add_glfw_required_extensions()
                                      .add_extension(vk::khr_extensions::device_properties_2)
                                      .add_extension(vk::extensions::debug_utils)
                                      .debug_layer(vk::validation_layers::validation)
                                      .build()
                                      .unwrap();

        // Select the GPU
        box<vk::gpu_t> gpu = orb::eval | [&] {
            auto gpus = vk::available_gpus_t::create(instance.handle).unwrap();

            for (const auto& [i, gpu] : flux::enumerate(gpus))
            {
                if (gpu->device_type == vk::gpu_types::discrete) { return gpus.select(i); }
            }

            println("No discrete GPU found, trying out integrated GPUs");
            for (const auto& [i, gpu] : flux::enumerate(gpus))
            {
                if (gpu->device_type == vk::gpu_types::integrated) { return gpus.select(i); }
            }

            println("No integrated GPU found either, return first available device");

            return gpus.select(0);
        };

        // Print informations on select gpu
        describe(*gpu);

        // Select queue family
        vk::queue_family_t graphics_queue_family = orb::eval | [&]() -> vk::queue_family_t& {
            for (auto& qf : gpu->queue_families)
            {
                if (qf.properties.queueFlags & vk::queue_families::graphics) { return qf; }
            }

            panic("No suitable queue family found");
        };

        println("- Selected queue family {} with {} queues",
                graphics_queue_family.index,
                graphics_queue_family.properties.queueCount);

        // Create vulkan device
        constexpr std::array queue_priorities { 1.0f };

        vk::device_t device =
            vk::device_builder_t::prepare(instance.handle)
                .unwrap()
                .add_extension(vk::khr_extensions::swapchain)
                .add_queues(graphics_queue_family, queue_priorities)
                .build(gpu.getmut())
                .unwrap();

        println("- Created vulkan device");

        constexpr ui32 max_frames_in_flight = 2;

        // Create swapchain
        vk::swapchain_t swapchain =
            vk::swapchain_builder_t::prepare(weak { &instance },
                                             gpu.getmut(),
                                             weak { &device },
                                             weak { &window })
                .unwrap()
                .fb_dimensions_from_window()
                .present_queue_family_index(0)

                .color_space(vk::color_spaces::srgb_nonlinear_khr)
                .format(vk::formats::b8g8r8a8_unorm)
                .format(vk::formats::r8g8b8a8_unorm)
                .format(vk::formats::b8g8r8_unorm)
                .format(vk::formats::r8g8b8_unorm)

                .present_mode(vk::present_modes::mailbox_khr)
                .present_mode(vk::present_modes::immediate_khr)
                .present_mode(vk::present_modes::fifo_khr)

                .semaphores(max_frames_in_flight)

                .build()
                .unwrap();

        println("- Created swapchain");

        // Create ImGui pass
        auto imgui_pass = create_imgui_pass(device.handle,
                                            swapchain.format.format);

        auto imgui_fbs_builder = vk::framebuffers_builder_t::prepare(&device, imgui_pass.handle)
                                     .unwrap()
                                     .size(swapchain.width, swapchain.height);

        for (auto& view : swapchain.views)
        {
            imgui_fbs_builder.attachment(view);
        }

        auto imgui_fbs = imgui_fbs_builder
                             .build()
                             .unwrap();

        auto desc_pool = vk::desc_pool_builder_t::prepare(&device)
                             .unwrap()
                             .pool(vk::desc_types::sampler, 100)
                             .flag(vk::descriptor_pool_create_flags::free_descriptor_set_bit)
                             .build()
                             .unwrap();

        // Synchronization
        auto sync_objects = vk::sync_objects_builder_t::prepare(&device)
                                .unwrap()
                                .semaphores(max_frames_in_flight * 2)
                                .fences(max_frames_in_flight)
                                .build()
                                .unwrap();

        auto cmd_pool = vk::cmd_pool_builder_t::prepare(&device, gpu->queue_families.front().index)
                            .unwrap()
                            .flag(vk::command_pool_create_flags::reset_command_buffer_bit)
                            .build()
                            .unwrap();

        auto cmd_buffers = vk::alloc_cmds(cmd_pool, max_frames_in_flight, vk::cmd_buffer_levels::primary)
                               .unwrap();

        // Init ImGui
        vk::imgui::initialize({ .window    = &window,
                                .instance  = &instance,
                                .gpu       = gpu.getmut(),
                                .device    = &device,
                                .swapchain = &swapchain,
                                .pass      = imgui_pass.handle,
                                .desc_pool = desc_pool.handle })
            .throw_if_error();

        // While loop
        uint32_t frame = 0;

        while (!window.should_close())
        {
            // Poll events (specific to your windowing library, e.g., GLFW)
            glfwPollEvents();

            auto fence           = sync_objects.subspan_fences(frame, 1);
            auto img_avail       = sync_objects.subspan_semaphores(frame, 1);
            auto render_finished = sync_objects.subspan_semaphores(frame + max_frames_in_flight, 1);

            // Start a new ImGui frame
            vk::imgui::new_frame();

            // Render the ImGui demo window
            ImGui::ShowDemoWindow();

            // Prepare ImGui for rendering
            vk::imgui::render();

            // Wait and reset fences
            vk::wait_and_reset_fences(fence).throw_if_error();

            // Acquire the next swapchain image
            uint32_t img_index {};
            VkResult result = vkAcquireNextImageKHR(device.handle,
                                                    swapchain.handle,
                                                    UINT64_MAX,
                                                    img_avail.handles.back(),
                                                    VK_NULL_HANDLE,
                                                    &img_index);

            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            {
                println("Requiring sc rebuilt");
                return 1;
            }

            // Begin command buffer recording
            const auto [cmd, begin_res] = cmd_buffers.begin_one_time(frame);

            // Begin the render pass
            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass            = imgui_pass.handle;
            renderPassInfo.framebuffer           = imgui_fbs.handles[img_index];
            renderPassInfo.renderArea.offset     = { .x = 0, .y = 0 };
            renderPassInfo.renderArea.extent     = swapchain.extent;

            VkClearValue clearColor        = { 0.0f, 0.0f, 0.0f, 1.0f };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues    = &clearColor;

            vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            // Render ImGui
            vk::imgui::submit_render(cmd);

            // End the render pass
            vkCmdEndRenderPass(cmd);

            // End command buffer recording
            vkEndCommandBuffer(cmd);

            // Submit the command buffer
            VkSubmitInfo submit_info = {};
            submit_info.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            std::array<VkPipelineStageFlags, 1> wait_stages = {
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            };

            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores    = img_avail.handles.data();
            submit_info.pWaitDstStageMask  = wait_stages.data();

            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers    = &cmd;

            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores    = render_finished.handles.data();

            if (vkQueueSubmit(device.queues[0], 1, &submit_info, fence.handles.back()) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to submit draw command buffer!");
            }

            // Present the rendered image
            VkPresentInfoKHR present_info = {};
            present_info.sType            = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores    = render_finished.handles.data();

            present_info.swapchainCount = 1;
            present_info.pSwapchains    = &swapchain.handle;
            present_info.pImageIndices  = &img_index;

            result = vkQueuePresentKHR(device.queues[0], &present_info);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            {
                println("Requiring sc rebuilt");
            }

            frame = (frame + 1) % max_frames_in_flight;
        }

        vk::device_idle(device);

        // Terminate glfw
        window.destroy();
        println("- Destroyed window");

        vk::imgui::terminate();

        vk::destroy(desc_pool);
        vk::destroy(imgui_fbs);
        vk::destroy(cmd_pool);
        vk::destroy(sync_objects);
        vk::destroy(imgui_pass);
        vk::destroy(swapchain);
        vk::destroy(device);
        vk::destroy(instance);
        println("- Terminated Vulkan");

        glfw::driver_t::terminate();
        println("- Terminated GLFW");
    }
    catch (const orb::exception& e)
    {
        println("Fatal error: {}", e.what());
        return 1;
    }

    return 0;
}
