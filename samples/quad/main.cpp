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
                                 .prefer_type(vk::gpu_types::discrete)
                                 .prefer_type(vk::gpu_types::integrated)
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

        println("- Selected graphics queue family {} with {} queues",
                graphics_qf->index,
                graphics_qf->properties.queueCount);

        println("- Selected transfer queue family {} with {} queues",
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

                                             .usage(vk::image_usage_flags::color_attachment)
                                             .color_space(vk::color_spaces::srgb_nonlinear_khr)
                                             .format(vk::formats::b8g8r8a8_srgb)
                                             .format(vk::formats::r8g8b8a8_srgb)
                                             .format(vk::formats::b8g8r8_srgb)
                                             .format(vk::formats::r8g8b8_srgb)

                                             .present_mode(vk::present_modes::mailbox_khr)
                                             .present_mode(vk::present_modes::immediate_khr)
                                             .present_mode(vk::present_modes::fifo_khr)

                                             .build()
                                             .unwrap();

        vk::attachments_t attachments;
        vk::subpasses_t   subpasses;

        attachments.add({
            .img_format        = swapchain->format.format,
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

        auto render_pass = vk::render_pass_builder_t::prepare(device->handle)
                               .unwrap()
                               .clear_color({ 0.0f, 0.0f, 0.0f, 1.0f })
                               .build(subpasses, attachments)
                               .unwrap();

        const auto create_views = [&] {
            return vk::views_builder_t::prepare(device->handle)
                .unwrap()
                .images(swapchain->images)
                .aspect_mask(vk::image_aspect_flags::color)
                .format(vk::formats::b8g8r8a8_srgb)
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

        println("- Reading shader files");
        auto vs_content = vs_path.read_file().unwrap();
        auto fs_content = fs_path.read_file().unwrap();

        println("- Creating shader modules");
        auto vs_shader_module = vk::shader_module_builder_t::prepare(device.getmut(), &compiler)
                                    .unwrap()
                                    .kind(vk::shader_kinds::glsl_vertex)
                                    .entry_point("main")
                                    .content(std::move(vs_content))
                                    .build()
                                    .unwrap();

        auto fs_shader_module = vk::shader_module_builder_t::prepare(device.getmut(), &compiler)
                                    .unwrap()
                                    .kind(vk::shader_kinds::glsl_fragment)
                                    .entry_point("main")
                                    .content(std::move(fs_content))
                                    .build()
                                    .unwrap();

        struct Vertex
        {
            std::array<float, 2> pos;
            std::array<float, 3> col;
        };

        println("- Creating graphics pipeline");
        auto pipeline = vk::pipeline_builder_t ::prepare(device.getmut())
                            .unwrap()
                            ->shader_stages()
                            .stage(vs_shader_module, vk::shader_stage_flags::vertex, "main")
                            .stage(fs_shader_module, vk::shader_stage_flags::fragment, "main")
                            .dynamic_states()
                            .dynamic_state(vk::dynamic_states::viewport)
                            .dynamic_state(vk::dynamic_states::scissor)
                            .vertex_input()
                            .binding<Vertex>(0, vk::vertex_input_rates::vertex)
                            .attribute(0, offsetof(Vertex, pos), vk::vertex_formats::vec2_t)
                            .attribute(1, offsetof(Vertex, col), vk::vertex_formats::vec3_t)
                            .input_assembly()
                            .viewport_states()
                            .viewport(0.0f, 0.0f, (f32)swapchain->width, (f32)swapchain->height, 0.0f, 1.0f)
                            .scissor(0.0f, 0.0f, swapchain->width, swapchain->height)
                            .rasterizer()
                            .multisample()
                            .color_blending()
                            .new_color_blend_attachment()
                            .end_attachment()
                            .layout()
                            .prepare_pipeline()
                            .render_pass(render_pass.getmut())
                            .subpass(0)
                            .build()
                            .unwrap();

        println("- Creating descriptor pool");
        auto desc_pool = vk::desc_pool_builder_t::prepare(device.getmut())
                             .unwrap()
                             .pool(vk::desc_types::sampler, 100)
                             .flag(vk::descriptor_pool_create_flags::free_descriptor_set_bit)
                             .build()
                             .unwrap();

        println("- Creating synchronization objects");
        // Synchronization
        auto sync_objects = vk::sync_objects_builder_t::prepare(device.getmut())
                                .unwrap()
                                .semaphores(max_frames_in_flight * 2)
                                .fences(max_frames_in_flight)
                                .build()
                                .unwrap();

        println("- Creating command pool and command buffers");
        auto graphics_cmd_pool = vk::cmd_pool_builder_t::prepare(device.getmut(), graphics_qf->index)
                                     .unwrap()
                                     .flag(vk::command_pool_create_flags::reset_command_buffer_bit)
                                     .build()
                                     .unwrap();

        auto transfer_cmd_pool = vk::cmd_pool_builder_t::prepare(device.getmut(), transfer_qf->index)
                                     .unwrap()
                                     .flag(vk::command_pool_create_flags::reset_command_buffer_bit)
                                     .build()
                                     .unwrap();

        println("- Creating command buffers");
        auto draw_cmds = graphics_cmd_pool->alloc_cmds(max_frames_in_flight).unwrap();

        std::vector<Vertex> vertices = {
            { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
            {  { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
            {   { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
            {  { -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f } }
        };

        std::vector<ui16> indices = { 0, 1, 2, 2, 3, 0 };

        println("- Creating vertex buffer");
        auto vertex_buffer = vk::vertex_buffer_builder_t::prepare(device.getmut())
                                 .unwrap()
                                 .vertices<Vertex>(vertices)
                                 .buffer_usage_flag(vk::buffer_usage_flags::transfer_destination)
                                 .memory_flags(vk::vma_alloc_flags::dedicated_memory)
                                 .build()
                                 .unwrap();

        println("- Creating index buffer");
        auto index_buffer = vk::index_buffer_builder_t::prepare(device.getmut())
                                .unwrap()
                                .indices(std::span<const ui16> { indices })
                                .buffer_usage_flag(vk::buffer_usage_flags::transfer_destination)
                                .memory_flags(vk::vma_alloc_flags::dedicated_memory)
                                .build()
                                .unwrap();

        println("- Creating staging buffer");
        auto staging_buffer = vk::staging_buffer_builder_t::prepare(device.getmut(), vertex_buffer.size)
                                  .unwrap()
                                  .build()
                                  .unwrap();

        println("- Copying vertices to staging buffer");
        staging_buffer.transfer(vertices.data(), sizeof(Vertex) * vertices.size()).unwrap();

        println("- Copying staging buffer to vertex buffer");
        auto cpy_cmd = transfer_cmd_pool->alloc_cmds(1).unwrap().get(0).unwrap();

        cpy_cmd.begin_one_time().unwrap();
        cpy_cmd.copy_buffer(staging_buffer.buffer, vertex_buffer.buffer, vertex_buffer.size);
        cpy_cmd.end().unwrap();

        println("- Submitting copy command buffer");
        vk::submit_helper_t::prepare()
            .cmd_buffer(&cpy_cmd.handle)
            .wait_stage(vk::pipeline_stage_flags::transfer)
            .submit(transfer_qf->queues.front())
            .unwrap();

        device->wait().unwrap();

        println("- Copying indices to staging buffer");
        staging_buffer.transfer(indices.data(), sizeof(ui16) * indices.size()).unwrap();

        println("- Copying staging buffer to index buffer");
        cpy_cmd = transfer_cmd_pool->alloc_cmds(1).unwrap().get(0).unwrap();

        cpy_cmd.begin_one_time().unwrap();
        println("Index buffer size: {}", index_buffer.size);
        cpy_cmd.copy_buffer(staging_buffer.buffer, index_buffer.buffer, index_buffer.size);
        cpy_cmd.end().unwrap();

        println("- Submitting copy command buffer");
        vk::submit_helper_t::prepare()
            .cmd_buffer(&cpy_cmd.handle)
            .wait_stage(vk::pipeline_stage_flags::transfer)
            .submit(transfer_qf->queues.front())
            .unwrap();

        device->wait().unwrap();

        ui32 frame = 0;

        println("- Main loop");
        while (!window->should_close())
        {
            glfw_driver->poll_events();

            if (window->minimized())
            {
                using namespace std::literals;
                std::this_thread::sleep_for(orb::milliseconds_t(100));
                continue;
            }

            auto fences               = sync_objects.fences(frame, 1);
            auto img_avail_sems       = sync_objects.semaphores(frame, 1);
            auto render_finished_sems = sync_objects.semaphores(frame + max_frames_in_flight, 1);

            // Wait fences
            fences.wait().unwrap();

            // Acquire the next swapchain image
            auto res = vk::acquire_img(*swapchain, img_avail_sems.handles.back(), nullptr);

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
                println("Acquire img error");
                return 1;
            }

            // Reset fences
            fences.reset().unwrap();

            uint32_t img_index = res.img_index();

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
                .wait_semaphores(img_avail_sems.handles)
                .signal_semaphores(render_finished_sems.handles)
                .cmd_buffer(&cmd.handle)
                .wait_stage(vk::pipeline_stage_flags::color_attachment_output)
                .submit(graphics_qf->queues.front(), fences.handles.back())
                .unwrap();

            // Present the rendered image
            auto present_res = vk::present_helper_t::prepare()
                                   .swapchain(*swapchain)
                                   .wait_semaphores(render_finished_sems.handles)
                                   .img_index(img_index)
                                   .present(graphics_qf->queues.front());

            if (present_res.require_sc_rebuild())
            {
                continue;
            }
            else if (present_res.is_error())
            {
                println("Frame present error: {}", vk::vkres::get_repr(present_res.error()));
                return 1;
            }

            frame = (frame + 1) % max_frames_in_flight;
        }

        device->wait().unwrap();
    }
    catch (const orb::exception& e)
    {
        println("Fatal error: {}", e.what());
        return 1;
    }

    return 0;
}
