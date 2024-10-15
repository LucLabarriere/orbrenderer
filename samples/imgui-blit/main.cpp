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
#include <orb/vk/imgui.hpp>

auto main() -> int
{
    using namespace orb;

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
                                      .debug_layer(vk::validation_layers::validation)
                                      .add_extension(vk::extensions::debug_report)
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

        vk::device_t device = //
            vk::device_builder_t::prepare(instance.handle)
                .unwrap()
                .add_extension(vk::khr_extensions::swapchain)
                .add_queues(graphics_queue_family, queue_priorities)
                .build(gpu.getmut())
                .unwrap();

        println("- Created vulkan device");

        constexpr ui32 max_frames_in_flight = 2;

        // Create swapchain
        vk::swapchain_t swapchain = //
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

        vk::render_pass_t imgui_pass = //
            vk::render_pass_builder_t::prepare(weak { &window },
                                               weak { &instance },
                                               gpu.getmut(),
                                               weak { &device },
                                               weak { &swapchain })
                .unwrap()
                .desc_pool_size(vk::desc_types::sampler, 1)
                .cmds(max_frames_in_flight)
                .fences(max_frames_in_flight)
                .framebuffers(max_frames_in_flight)
                .semaphores(max_frames_in_flight)
                .build()
                .unwrap();

        vk::imgui::initialize({
                                  .window    = weak { &window },
                                  .instance  = weak { &instance },
                                  .gpu       = gpu.getmut(),
                                  .device    = weak { &device },
                                  .swapchain = weak { &swapchain },
                                  .pass      = weak { &imgui_pass },
                              })
            .throw_if_error();

        vk::render_info_t render_info {
            .device    = device.handle,
            .pass      = imgui_pass.handle,
            .queue     = device.queues.back(),
            .swapchain = weak { &swapchain },
        };

        ui32 frame    = 0;
        ui32 sc_frame = 0;

        VkExtent2D extent {
            .width  = swapchain.width,
            .height = swapchain.height,
        };

        describe(render_info);

        while (!window.should_close())
        {
            glfwPollEvents();

            render_info.fb         = imgui_pass.fbs.at(frame);
            render_info.cmd        = imgui_pass.cmds.at(frame);
            render_info.wait_fence = imgui_pass.fences.at(frame);

            vk::imgui::new_frame();
            ImGui::ShowDemoWindow();
            vk::imgui::render();

            vk::render_begin(render_info).throw_if_error();
            vk::imgui::submit_render(render_info.cmd);
            vk::render_end(render_info).throw_if_error();

            if (vk::acquire_next_img(swapchain, imgui_pass.semaphores.at(frame), nullptr)
                    .require_sc_rebuild())
            {
                throw orb::exception("Resize not supported");
            }

            auto copy_cmd       = vk::alloc_cmd(device, imgui_pass.cmd_pool).unwrap();
            auto cmd_begin_info = vk::structs::one_time_cmd_buffer_begin();

            vkBeginCommandBuffer(copy_cmd, &cmd_begin_info);
            vk::transition_layout(copy_cmd,
                                  imgui_pass.images.at(frame),
                                  vk::image_layouts::undefined,
                                  vk::image_layouts::transfer_src_optimal);
            vk::transition_layout(copy_cmd,
                                  swapchain.images.at(sc_frame),
                                  vk::image_layouts::undefined,
                                  vk::image_layouts::transfer_dst_optimal);
            vk::copy_img(copy_cmd, imgui_pass.images.at(frame), swapchain.images.at(sc_frame), extent);
            vk::transition_layout(copy_cmd,
                                  swapchain.images.at(sc_frame),
                                  vk::image_layouts::transfer_dst_optimal,
                                  vk::image_layouts::present_src_khr);
            vkEndCommandBuffer(copy_cmd);

            auto submit_copy_info                 = vk::structs::submit();
            submit_copy_info.pCommandBuffers      = &copy_cmd;
            submit_copy_info.commandBufferCount   = 1;
            submit_copy_info.waitSemaphoreCount   = 1;
            submit_copy_info.pWaitSemaphores      = &imgui_pass.semaphores.at(frame);
            submit_copy_info.signalSemaphoreCount = 1;
            submit_copy_info.pSignalSemaphores    = &swapchain.semaphores.at(frame);
            vkQueueSubmit(device.queues.back(), 1, &submit_copy_info, nullptr);

            auto info               = vk::structs::present();
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores    = &swapchain.semaphores.at(frame);
            info.swapchainCount     = 1;
            info.pSwapchains        = &swapchain.handle;
            info.pImageIndices      = &sc_frame;

            std::span present_sem = std::span { swapchain.semaphores }.subspan(frame, 1);

            if (vk::present_img(swapchain, device.queues.back(), present_sem, sc_frame).require_sc_rebuild())
            {
                throw orb::exception("Resize not supported");
            }

            frame = (frame + 1) % max_frames_in_flight;
        }

        vk::imgui::terminate();

        // Terminate glfw
        window.destroy();
        vk::device_idle(device);
        println("- Destroyed window");

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
