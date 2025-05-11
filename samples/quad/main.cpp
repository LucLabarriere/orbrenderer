#include <span>
#include <thread>

#include <orb/eval.hpp>
#include <orb/files.hpp>
#include <orb/flux.hpp>
#include <orb/renderer.hpp>
#include <orb/time.hpp>

using namespace orb;

static constexpr ui32 max_frames_in_flight = 2;

auto main() -> int
{
    try
    {
        box<glfw::driver_t> glfw_driver = glfw::driver_t::create().unwrap();

        weak<glfw::window_t> window   = glfw_driver->create_window_for_vk().unwrap();
        box<vk::instance_t>  instance = vk::instance_builder_t::prepare()
                                           .unwrap()
                                           .add_glfw_required_extensions()
                                           .molten_vk(orb::on_macos ? true : false)
                                           .add_extension(vk::khr_extensions::device_properties_2)
                                           .add_extension(vk::extensions::debug_utils)
                                           .debug_layer(vk::validation_layers::validation)
                                           .build()
                                           .unwrap();

        vk::surface_t surface = vk::surface_builder_t::prepare(instance->handle, window).build().unwrap();

        box<vk::gpu_t> gpu = vk::gpu_selector_t::prepare(instance->handle)
                                 .unwrap()
                                 .prefer_type(vk::gpu_type::discrete)
                                 .prefer_type(vk::gpu_type::integrated)
                                 .select()
                                 .unwrap();

        gpu->describe();

        auto [graphics_qf, transfer_qf] = orb::eval | [&] {
            std::span graphics_qfs = gpu->queue_family_map->graphics().unwrap();
            std::span transfer_qfs = gpu->queue_family_map->transfer().unwrap();

            auto graphics_qf = graphics_qfs.front();

            auto transfer_qf = orb::eval | [&] {
                for (auto qf : transfer_qfs)
                {
                    if (qf->index != graphics_qf->index)
                    {
                        return qf;
                    }
                }

                return transfer_qfs.front();
            };

            return std::make_tuple(graphics_qf, transfer_qf);
        };

        fmt::println("- Selected graphics queue family {} with {} queues",
                     graphics_qf->index,
                     graphics_qf->properties.queueCount);

        fmt::println("- Selected transfer queue family {} with {} queues",
                     transfer_qf->index,
                     transfer_qf->properties.queueCount);

        auto device = vk::device_builder_t::prepare(instance->handle)
                          .unwrap()
                          .add_extension(vk::khr_extensions::swapchain)
                          .add_queue(graphics_qf, 1.0f)
                          .add_queue(transfer_qf, 1.0f)
                          .build(*gpu)
                          .unwrap();

        box<vk::swapchain_t> swapchain = vk::swapchain_builder_t::prepare(instance.getmut(),
                                                                          gpu.getmut(),
                                                                          device.getmut(),
                                                                          window,
                                                                          &surface)
                                             .unwrap()
                                             .fb_dimensions_from_window()
                                             .present_queue_family_index(graphics_qf->index)

                                             .usage(vk::image_usage_flag::color_attachment)
                                             .color_space(vk::color_space::srgb_nonlinear_khr)
                                             .format(vk::format::b8g8r8a8_srgb)
                                             .format(vk::format::r8g8b8a8_srgb)
                                             .format(vk::format::b8g8r8_srgb)
                                             .format(vk::format::r8g8b8_srgb)

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

        auto render_pass = vk::render_pass_builder_t::prepare(device->handle)
                               .unwrap()
                               .clear_color({ 0.0f, 0.0f, 0.0f, 1.0f })
                               .build(subpasses, attachments)
                               .unwrap();

        const auto create_views = [&] {
            return vk::views_builder_t::prepare(device->handle)
                .unwrap()
                .images(swapchain->images)
                .aspect_mask(vk::image_aspect_flag::color)
                .format(vk::format::b8g8r8a8_srgb)
                .build()
                .unwrap();
        };

        vk::views_t views = create_views();

        const auto create_fbs = [&] {
            return vk::framebuffers_builder_t::prepare(device.getmut(), render_pass->handle)
                .unwrap()
                .size(swapchain->width, swapchain->height)
                .attachments(views.handles)
                .build()
                .unwrap();
        };

        vk::framebuffers_t fbs = create_fbs();

        const path vs_path { SAMPLE_DIR "main.vs.glsl" };
        const path fs_path { SAMPLE_DIR "main.fs.glsl" };

        vk::spirv_compiler_t compiler;
        compiler.option_target_env(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2)
            .option_generate_debug_info()
            .option_target_spirv(shaderc_spirv_version_1_3)
            .option_source_language(shaderc_source_language_glsl)
            .option_optimization_level(shaderc_optimization_level_zero)
            .option_warnings_as_errors();

        fmt::println("- Reading shader files");
        auto vs_content = vs_path.read_file().unwrap();
        auto fs_content = fs_path.read_file().unwrap();

        fmt::println("- Creating shader modules");
        auto vs_shader_module = vk::shader_module_builder_t::prepare(device.getmut(), &compiler)
                                    .unwrap()
                                    .kind(vk::shader_kind::glsl_vertex)
                                    .entry_point("main")
                                    .content(std::move(vs_content))
                                    .build()
                                    .unwrap();

        auto fs_shader_module = vk::shader_module_builder_t::prepare(device.getmut(), &compiler)
                                    .unwrap()
                                    .kind(vk::shader_kind::glsl_fragment)
                                    .entry_point("main")
                                    .content(std::move(fs_content))
                                    .build()
                                    .unwrap();

        struct vertex_t
        {
            std::array<float, 2> pos;
            std::array<float, 3> col;
        };

        fmt::println("- Creating graphics pipeline");
        auto pipeline = vk::pipeline_builder_t ::prepare(device.getmut())
                            .unwrap()
                            ->shader_stages()
                            .stage(vs_shader_module, vk::shader_stage_flag::vertex, "main")
                            .stage(fs_shader_module, vk::shader_stage_flag::fragment, "main")
                            .dynamic_states()
                            .dynamic_state(vk::dynamic_state::viewport)
                            .dynamic_state(vk::dynamic_state::scissor)
                            .vertex_input()
                            .binding<vertex_t>(0, vk::vertex_input_rate::vertex)
                            .attribute(0, offsetof(vertex_t, pos), vk::vertex_format::vec2_t)
                            .attribute(1, offsetof(vertex_t, col), vk::vertex_format::vec3_t)
                            .input_assembly()
                            .viewport_states()
                            .viewport(0.0f, 0.0f, (f32)swapchain->width, (f32)swapchain->height, 0.0f, 1.0f)
                            .scissor(0.0f, 0.0f, swapchain->width, swapchain->height)
                            .rasterizer()
                            .multisample()
                            .color_blending()
                            .new_color_blend_attachment()
                            .end_attachment()
                            .desc_set_layout()
                            .pipeline_layout()
                            .prepare_pipeline()
                            .render_pass(render_pass.getmut())
                            .subpass(0)
                            .build()
                            .unwrap();

        fmt::println("- Creating synchronization objects");

        // Synchronization
        auto fences = vk::fences_builder_t::create(device.getmut(), max_frames_in_flight)
                          .unwrap();

        auto img_avail_sems = vk::semaphores_builder_t::prepare(device.getmut())
                                  .unwrap()
                                  .count(max_frames_in_flight)
                                  .stage(vk::pipeline_stage_flag::color_attachment_output)
                                  .build()
                                  .unwrap();

        auto render_finished_sems = vk::semaphores_builder_t::prepare(device.getmut())
                                        .unwrap()
                                        .count(swapchain->images.size())
                                        .stage(vk::pipeline_stage_flag::color_attachment_output)
                                        .build()
                                        .unwrap();

        fmt::println("- Creating command pool and command buffers");
        auto graphics_cmd_pool = vk::cmd_pool_builder_t::prepare(device.getmut(), graphics_qf->index)
                                     .unwrap()
                                     .flag(vk::command_pool_create_flag::reset_command_buffer)
                                     .build()
                                     .unwrap();

        auto transfer_cmd_pool = vk::cmd_pool_builder_t::prepare(device.getmut(), transfer_qf->index)
                                     .unwrap()
                                     .flag(vk::command_pool_create_flag::reset_command_buffer)
                                     .build()
                                     .unwrap();

        fmt::println("- Creating command buffers");
        auto draw_cmds = graphics_cmd_pool->alloc_cmds(max_frames_in_flight).unwrap();

        std::vector<vertex_t> vertices = {
            { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
            {  { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
            {   { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
            {  { -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f } }
        };

        std::vector<ui16> indices = { 0, 1, 2, 2, 3, 0 };

        fmt::println("- Creating vertex buffer");
        auto vertex_buffer = vk::vertex_buffer_builder_t::prepare(device.getmut())
                                 .unwrap()
                                 .vertices<vertex_t>(vertices)
                                 .buffer_usage_flag(vk::buffer_usage_flag::transfer_destination)
                                 .memory_flags(vk::memory_flag::dedicated_memory)
                                 .build()
                                 .unwrap();

        fmt::println("- Creating index buffer");
        auto index_buffer = vk::index_buffer_builder_t::prepare(device.getmut())
                                .unwrap()
                                .indices(std::span<const ui16> { indices })
                                .buffer_usage_flag(vk::buffer_usage_flag::transfer_destination)
                                .memory_flags(vk::memory_flag::dedicated_memory)
                                .build()
                                .unwrap();

        fmt::println("- Creating staging buffer");
        auto staging_buffer = vk::staging_buffer_builder_t::prepare(device.getmut(), vertex_buffer.size)
                                  .unwrap()
                                  .build()
                                  .unwrap();

        fmt::println("- Copying vertices to staging buffer");
        staging_buffer.transfer(vertices.data(), sizeof(vertex_t) * vertices.size()).unwrap();

        fmt::println("- Copying staging buffer to vertex buffer");
        auto cpy_cmd = transfer_cmd_pool->alloc_cmds(1).unwrap().get(0).unwrap();

        cpy_cmd.begin_one_time().unwrap();
        cpy_cmd.copy_buffer(staging_buffer.buffer, vertex_buffer.buffer, vertex_buffer.size);
        cpy_cmd.end().unwrap();

        fmt::println("- Submitting copy command buffer");
        vk::submit_helper_t::prepare()
            .cmd_buffer(&cpy_cmd.handle)
            .submit(transfer_qf->queues.front())
            .unwrap();

        device->wait().unwrap();

        fmt::println("- Copying indices to staging buffer");
        staging_buffer.transfer(indices.data(), sizeof(ui16) * indices.size()).unwrap();

        fmt::println("- Copying staging buffer to index buffer");
        cpy_cmd = transfer_cmd_pool->alloc_cmds(1).unwrap().get(0).unwrap();

        cpy_cmd.begin_one_time().unwrap();
        cpy_cmd.copy_buffer(staging_buffer.buffer, index_buffer.buffer, index_buffer.size);
        cpy_cmd.end().unwrap();

        fmt::println("- Submitting copy command buffer");
        vk::submit_helper_t::prepare()
            .cmd_buffer(&cpy_cmd.handle)
            .submit(transfer_qf->queues.front())
            .unwrap();

        device->wait().unwrap();

        ui32 frame = 0;

        fmt::println("- Main loop");
        while (!window->should_close())
        {
            glfw_driver->poll_events();

            if (window->minimized())
            {
                using namespace std::literals;
                std::this_thread::sleep_for(orb::milliseconds_t(100));
                continue;
            }

            auto fence     = fences[frame];
            auto img_avail = img_avail_sems.view(frame, 1);

            // Wait fences
            fence.wait().unwrap();

            // Acquire the next swapchain image
            auto res = vk::acquire_img(*swapchain, img_avail.handles.back(), nullptr);

            if (res.require_sc_rebuild())
            {
                device->wait().unwrap();
                swapchain->rebuild().unwrap();

                views = create_views();
                fbs   = create_fbs();
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

            auto render_finished = render_finished_sems.view(img_index, 1);

            // Render to the framebuffer
            render_pass->begin_info.framebuffer       = fbs.handles[img_index];
            render_pass->begin_info.renderArea.extent = swapchain->extent;

            // Begin command buffer recording
            auto cmd = draw_cmds.get(frame).unwrap();
            cmd.begin_one_time().unwrap();

            // Begin the render pass
            render_pass->begin(cmd.handle);

            // Bind the graphics pipeline
            vkCmdBindPipeline(cmd.handle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->handle);
            std::array<VkDeviceSize, 1> offsets = { 0 };
            vkCmdBindVertexBuffers(cmd.handle, 0, 1, &vertex_buffer.buffer, offsets.data());
            vkCmdBindIndexBuffer(cmd.handle, index_buffer.buffer, 0, index_buffer.index_type);

            // Set viewport and scissor
            auto& viewport        = pipeline->viewports.back();
            auto& scissor         = pipeline->scissors.back();
            viewport.width        = static_cast<f32>(swapchain->width);
            viewport.height       = static_cast<f32>(swapchain->height);
            scissor.extent.width  = swapchain->width;
            scissor.extent.height = swapchain->height;
            vkCmdSetViewport(cmd.handle, 0, 1, &viewport);
            vkCmdSetScissor(cmd.handle, 0, 1, &scissor);

            // Draw quad
            vkCmdDrawIndexed(cmd.handle, static_cast<ui32>(indices.size()), 1, 0, 0, 0);

            // End the render pass
            render_pass->end(cmd.handle);

            // End command buffer recording
            cmd.end().unwrap();

            // Submit render
            vk::submit_helper_t::prepare()
                .wait_semaphores(img_avail)
                .signal_semaphores(render_finished.handles)
                .cmd_buffer(&cmd.handle)
                .submit(graphics_qf->queues.front(), fence.handle)
                .unwrap();

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
