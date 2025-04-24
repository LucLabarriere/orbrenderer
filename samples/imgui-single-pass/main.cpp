#include <array>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include <orb/eval.hpp>
#include <orb/flux.hpp>
#include <orb/glfw.hpp>
#include <orb/platform.hpp>
#include <orb/time.hpp>
#include <orb/vk/all.hpp>

using namespace orb;

auto main() -> int
{
    static constexpr ui32 max_frames_in_flight = 2;

    try
    {
        box<glfw::driver_t> glfw_driver = glfw::driver_t::create().unwrap();

        weak<glfw::window_t> window = glfw_driver->create_window_for_vk().unwrap();

        box<vk::instance_t> instance = vk::instance_builder_t::prepare()
                                           .unwrap()
                                           .add_glfw_required_extensions()
                                           .molten_vk(orb::on_macos ? true : false)
                                           .add_extension(vk::khr_extensions::device_properties_2)
                                           .add_extension(vk::extensions::debug_utils)
                                           .debug_layer(vk::validation_layers::validation)
                                           .build()
                                           .unwrap();

        vk::surface_t surface = vk::surface_builder_t::prepare(instance->handle, window)
                                    .build()
                                    .unwrap();

        box<vk::gpu_t> gpu = vk::gpu_selector_t::prepare(instance->handle)
                                 .unwrap()
                                 .prefer_type(vk::gpu_type::discrete)
                                 .prefer_type(vk::gpu_type::integrated)
                                 .select()
                                 .unwrap();
        gpu->describe();

        auto graphics_qf = gpu->queue_family_map->graphics().unwrap().front();

        auto device = vk::device_builder_t::prepare(instance->handle)
                          .unwrap()
                          .add_extension(vk::khr_extensions::swapchain)
                          .add_queue(graphics_qf, 1.0f)
                          .build(*gpu)
                          .unwrap();

        box<vk::swapchain_t> swapchain =
            vk::swapchain_builder_t::prepare(instance.getmut(),
                                             gpu.getmut(),
                                             device.getmut(),
                                             window,
                                             &surface)
                .unwrap()
                .fb_dimensions_from_window()
                .present_queue_family_index(graphics_qf->index)

                .usage(vk::image_usage_flag::transfer_dst)
                .usage(vk::image_usage_flag::color_attachment)
                .color_space(vk::color_space::srgb_nonlinear_khr)
                .format(vk::format::b8g8r8a8_unorm)
                .format(vk::format::r8g8b8a8_unorm)
                .format(vk::format::b8g8r8_unorm)
                .format(vk::format::r8g8b8_unorm)

                .present_mode(vk::present_mode::mailbox_khr)
                .present_mode(vk::present_mode::immediate_khr)
                .present_mode(vk::present_mode::fifo_khr)

                .build()
                .unwrap();

        vk::attachments_t attachments;
        vk::subpasses_t   subpasses;

        attachments.add({
            .img_format        = swapchain->format.format,
            .samples           = vk::sample_count_flag::_1,
            .load_ops          = vk::attachment_load_op::clear,
            .store_ops         = vk::attachment_store_op::store,
            .stencil_load_ops  = vk::attachment_load_op::dont_care,
            .stencil_store_ops = vk::attachment_store_op::dont_care,
            .initial_layout    = vk::image_layout::undefined,
            .final_layout      = vk::image_layout::present_src_khr,
            .attachment_layout = vk::image_layout::color_attachment_optimal,
        });

        const auto [color_descs, color_refs] = attachments.spans(0, 1);

        subpasses.add_subpass({
            .bind_point = vk::pipeline_bind_point::graphics,
            .color_refs = color_refs,
        });

        subpasses.add_dependency({
            .src        = vk::subpass_external,
            .dst        = 0,
            .src_stage  = vk::pipeline_stage_flag::color_attachment_output,
            .dst_stage  = vk::pipeline_stage_flag::color_attachment_output,
            .src_access = 0,
            .dst_access = vk::access_flag::color_attachment_write,
        });

        auto imgui_pass = vk::render_pass_builder_t::prepare(device->handle)
                              .unwrap()
                              .clear_color({ 0.0f, 0.0f, 0.0f, 1.0f })
                              .build(subpasses, attachments)
                              .unwrap();

        const auto create_views = [&] {
            return vk::views_builder_t::prepare(device->handle)
                .unwrap()
                .images(swapchain->images)
                .aspect_mask(vk::image_aspect_flag::color)
                .format(vk::format::b8g8r8a8_unorm)
                .build()
                .unwrap();
        };

        vk::views_t views = create_views();

        const auto create_fbs = [&] {
            return vk::framebuffers_builder_t::prepare(device.getmut(), imgui_pass->handle)
                .unwrap()
                .size(swapchain->width, swapchain->height)
                .attachments(views.handles)
                .build()
                .unwrap();
        };

        auto imgui_fbs = create_fbs();

        auto desc_pool = vk::desc_pool_builder_t::prepare(device.getmut())
                             .unwrap()
                             .pool(vk::descriptor_type::combined_image_sampler, 100)
                             .flag(vk::descriptor_pool_create_flag::free_descriptor_set)
                             .build()
                             .unwrap();

        // Synchronization
        auto sync_objects = vk::sync_objects_builder_t::prepare(device.getmut())
                                .unwrap()
                                .semaphores(max_frames_in_flight + swapchain->images.size())
                                .fences(max_frames_in_flight)
                                .build()
                                .unwrap();

        auto graphics_cmd_pool = vk::cmd_pool_builder_t::prepare(device.getmut(),
                                                                 graphics_qf->index)
                                     .unwrap()
                                     .flag(vk::command_pool_create_flag::reset_command_buffer)
                                     .build()
                                     .unwrap();

        auto draw_cmds = graphics_cmd_pool->alloc_cmds(max_frames_in_flight).unwrap();

        auto imgui_driver = vk::imgui_driver_builder_t::prepare(window,
                                                                instance.getmut(),
                                                                gpu.getmut(),
                                                                device.getmut(),
                                                                swapchain.getmut(),
                                                                &desc_pool,
                                                                imgui_pass.getmut(),
                                                                graphics_qf->index,
                                                                graphics_qf->queues.front())
                                .dark_theme(true)
                                .config_flag(ImGuiConfigFlags_NavEnableKeyboard)
                                .build()
                                .unwrap();

        ui32 frame = 0;

        while (!window->should_close())
        {
            glfw_driver->poll_events();

            if (window->minimized())
            {
                using namespace std::literals;
                std::this_thread::sleep_for(orb::milliseconds_t(100));
                continue;
            }

            auto fence           = sync_objects.fences(frame, 1);
            auto img_avail       = sync_objects.semaphores(frame, 1);

            // Start a new ImGui frame
            imgui_driver.new_frame();

            // Render the ImGui demo window
            ImGui::ShowDemoWindow();

            // Render ImGui
            imgui_driver.render();

            // Wait fences
            fence.wait().unwrap();

            // Acquire the next swapchain image
            auto res = vk::acquire_img(*swapchain, img_avail.handles.back(), nullptr);

            if (res.require_sc_rebuild())
            {
                device->wait().unwrap();
                swapchain->rebuild().throw_if_error();

                views     = create_views();
                imgui_fbs = create_fbs();

                continue;
            }
            else if (res.is_error())
            {
                fmt::println("Acquire img error");
                return 1;
            }

            // Reset fences
            fence.reset().unwrap();

            uint32_t img_index = res.img_index();

            auto render_finished = sync_objects.semaphores(img_index + max_frames_in_flight, 1);

            // Render to the imgui pass framebuffer
            imgui_pass->begin_info.framebuffer       = imgui_fbs.handles[img_index];
            imgui_pass->begin_info.renderArea.extent = swapchain->extent;

            // Begin command buffer recording
            auto cmd = draw_cmds.get(frame).unwrap();
            cmd.begin_one_time().throw_if_error();

            // Begin the render pass
            imgui_pass->begin(cmd.handle);

            // Render ImGui
            imgui_driver.submit_render(cmd.handle);

            // End the render pass
            imgui_pass->end(cmd.handle);

            // End command buffer recording
            cmd.end();

            // Submit
            vk::submit_helper_t::prepare()
                .wait_semaphores(img_avail.handles)
                .signal_semaphores(render_finished.handles)
                .cmd_buffer(&cmd.handle)
                .wait_stage(vk::pipeline_stage_flag::color_attachment_output)
                .submit(graphics_qf->queues.front(), fence.handles.back())
                .throw_if_error();

            // Present the rendered image
            auto present_res = vk::present_helper_t::prepare()
                                   .swapchain(*swapchain)
                                   .wait_semaphores(render_finished.handles)
                                   .img_index(img_index)
                                   .present(graphics_qf->queues.front());

            if (present_res.require_sc_rebuild())
            {
                continue;
            }
            else if (present_res.is_error())
            {
                fmt::println("Frame present error: {}", vk::vkres::get_repr(present_res.error()));
                return 1;
            }

            frame = (frame + 1) % max_frames_in_flight;
        }

        device->wait().unwrap();
    }
    catch (const orb::exception& e)
    {
        fmt::println("Fatal error: {}", e.what());
        return 1;
    }

    return 0;
}
