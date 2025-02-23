#include <span>
#include <thread>

#include <orb/eval.hpp>
#include <orb/files.hpp>
#include <orb/flux.hpp>
#include <orb/print_time.hpp>
#include <orb/renderer.hpp>

using namespace orb;

static constexpr ui32 max_frames_in_flight = 2;

namespace
{
    /* @brief Initializes GLFW and creates the window
     *
     * @return The GLFW window
     */
    [[nodiscard]] auto create_glfw_window() -> box<glfw::window_t>
    {
        glfw::driver_t::initialize().throw_if_error();
        auto w = glfw::driver_t::create_window_for_vk().unwrap();

        return w;
    }

    /** @brief Creates the vulkan instance.
     *
     * @return The vulkan instance.
     */
    [[nodiscard]] auto create_vk_instance() -> box<vk::instance_t>
    {
        constexpr auto portability = vk::khr_extensions::portability_enumeration;

        // Initialize the vulkan instance
        auto instance_builder = vk::instance_builder_t::prepare().unwrap();

        if (instance_builder.is_ext_available(portability))
        {
            instance_builder.add_extension(portability);
            instance_builder.create_info.flags |= vk::instance_create::portability;
        }

        auto instance = instance_builder.add_glfw_required_extensions()
                            .add_extension(vk::khr_extensions::device_properties_2)
                            .add_extension(vk::extensions::debug_utils)
                            .debug_layer(vk::validation_layers::validation)
                            .build()
                            .unwrap();

        return instance;
    }

    /** @brief Selects the GPU.
     *
     * @param instance The vulkan instance.
     * @return The selected GPU.
     */
    [[nodiscard]] auto create_vk_gpu(vk::instance_t& instance) -> box<vk::gpu_t>
    {
        auto gpu = orb::eval | [&] {
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
        vk::describe(*gpu);
        return gpu;
    }

    /* @brief Creates the vulkan device
     *
     * @param instance The vulkan instance
     * @param gpu The selected GPU
     * @return The vulkan device
     */
    [[nodiscard]] auto create_vk_device(vk::instance_t& instance, vk::gpu_t& gpu) -> box<vk::device_t>
    {
        vk::queue_family_t graphics_queue_family = orb::eval | [&]() -> vk::queue_family_t& {
            for (auto& qf : gpu.queue_families)
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

        auto device = vk::device_builder_t::prepare(instance.handle)
                          .unwrap()
                          .add_extension(vk::khr_extensions::swapchain)
                          .add_queues(graphics_queue_family, queue_priorities)
                          .build(gpu)
                          .unwrap();
        return device;
    }

    /* @brief Creates the vulkan swapchain
     *
     * @param instance The vulkan instance
     * @param device The vulkan device
     * @param gpu The selected GPU
     * @param window The GLFW window
     * @return The vulkan swapchain
     */
    [[nodiscard]] auto create_vk_swapchain(vk::instance_t& instance,
                                           vk::device_t&   device,
                                           vk::gpu_t&      gpu,
                                           glfw::window_t& window) -> box<vk::swapchain_t>
    {
        auto swapchain = vk::swapchain_builder_t::prepare(&instance,
                                                          &gpu,
                                                          &device,
                                                          &window)
                             .unwrap()
                             .fb_dimensions_from_window()
                             .present_queue_family_index(0)

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

        return swapchain;
    }

    /* @brief Creates the render pass
     *
     * @param device The vulkan device
     * @param sc_img_format The swapchain image format
     * @return The render pass
     */
    [[nodiscard]] auto create_render_pass(VkDevice device, VkFormat sc_img_format) -> box<vk::render_pass_t>
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

        auto render_pass = vk::render_pass_builder_t::prepare(device)
                               .unwrap()
                               .clear_color({ 0.0f, 0.0f, 0.0f, 1.0f })
                               .build(subpasses, attachments)
                               .unwrap();
        render_pass->bind_color();

        return render_pass;
    }

    [[nodiscard]] auto create_image_views(vk::device_t& device, std::span<VkImage> images) -> vk::views_t
    {
        auto builder = vk::views_builder_t::prepare(device.handle)
                           .unwrap();

        for (const auto& img : images)
        {
            builder.image(img);
        }

        auto views = builder
                         .aspect_mask(vk::image_aspect_flags::color)
                         .format(vk::formats::b8g8r8a8_srgb)
                         .build()
                         .unwrap();
        return views;
    }

    [[nodiscard]] auto create_framebuffers(vk::device_t& device,
                                           VkRenderPass  pass,
                                           vk::views_t&  views,
                                           ui32          w,
                                           ui32          h) -> vk::framebuffers_t
    {
        auto fbs_builder = vk::framebuffers_builder_t::prepare(&device, pass)
                               .unwrap()
                               .size(w, h);

        for (const auto& [img, view] : views.handles)
        {
            fbs_builder.attachment(view);
        }

        auto fbs = fbs_builder
                       .build()
                       .unwrap();

        return fbs;
    }

} // namespace

auto main() -> int
{
    try
    {
        box<glfw::window_t>  window    = create_glfw_window();
        box<vk::instance_t>  instance  = create_vk_instance();
        box<vk::gpu_t>       gpu       = create_vk_gpu(*instance);
        box<vk::device_t>    device    = create_vk_device(*instance, *gpu);
        box<vk::swapchain_t> swapchain = create_vk_swapchain(*instance, *device, *gpu, *window);

        vk::views_t            views       = create_image_views(*device, swapchain->images);
        box<vk::render_pass_t> render_pass = create_render_pass(device->handle, swapchain->format.format);
        vk::framebuffers_t     fbs         = create_framebuffers(*device,
                                                     render_pass->handle,
                                                     views,
                                                     swapchain->width,
                                                     swapchain->height);

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
        auto cmd_pool = vk::cmd_pool_builder_t::prepare(device.getmut(), gpu->queue_families.front().index)
                            .unwrap()
                            .flag(vk::command_pool_create_flags::reset_command_buffer_bit)
                            .build()
                            .unwrap();

        println("- Creating command buffers");
        auto cmd_buffers = vk::alloc_cmds(cmd_pool,
                                          max_frames_in_flight,
                                          vk::cmd_buffer_levels::primary)
                               .unwrap();

        println("- Creating vertex buffer");
        std::vector<Vertex> vertices = {
            { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
            {  { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
            { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
        };

        auto vertex_buffer = vk::vertex_buffer_builder_t::prepare(device.getmut())
                                 .unwrap()
                                 .vertices<Vertex>(vertices)
                                 .memory_flags(vk::vma_alloc_flags::host_access_sequential_write)
                                 .build()
                                 .unwrap();

        void* data {};
        vmaMapMemory(vertex_buffer.allocator, vertex_buffer.allocation, &data);
        std::memcpy(data, vertices.data(), sizeof(Vertex) * vertices.size());
        vmaUnmapMemory(vertex_buffer.allocator, vertex_buffer.allocation);

        ui32 frame = 0;

        while (!window->should_close())
        {
            glfw::driver_t::poll_events();

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
            vk::wait_fences(fences).throw_if_error();

            // Acquire the next swapchain image
            auto res = vk::acquire_img(*swapchain, img_avail_sems.handles.back(), nullptr);

            if (res.require_sc_rebuild())
            {
                vk::device_idle(*device);
                vk::destroy(fbs);
                vk::destroy(views);
                swapchain->rebuild().throw_if_error();

                views = create_image_views(*device, swapchain->images);
                fbs   = create_framebuffers(*device,
                                          render_pass->handle,
                                          views,
                                          swapchain->width,
                                          swapchain->height);
                continue;
            }
            else if (res.is_error())
            {
                println("Acquire img error");
                return 1;
            }

            // Reset fences
            vk::reset_fences(fences).throw_if_error();

            uint32_t img_index = res.img_index();

            // Render to the framebuffer
            render_pass->begin_info.framebuffer       = fbs.handles[img_index];
            render_pass->begin_info.renderArea.extent = swapchain->extent;

            // Begin command buffer recording
            auto [cmd, begin_res] = cmd_buffers.begin_one_time(frame);

            // Begin the render pass
            vk::begin(*render_pass, cmd);

            // Bind the graphics pipeline
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->handle);
            std::array<VkDeviceSize, 1> offsets = { 0 };
            vkCmdBindVertexBuffers(cmd, 0, 1, &vertex_buffer.buffer, offsets.data());

            // Set viewport and scissor
            auto& viewport        = pipeline->viewports.back();
            auto& scissor         = pipeline->scissors.back();
            viewport.width        = static_cast<f32>(swapchain->width);
            viewport.height       = static_cast<f32>(swapchain->height);
            scissor.extent.width  = swapchain->width;
            scissor.extent.height = swapchain->height;
            vkCmdSetViewport(cmd, 0, 1, &viewport);
            vkCmdSetScissor(cmd, 0, 1, &scissor);

            // Draw triangle
            vkCmdDraw(cmd, vertices.size(), 1, 0, 0);

            // End the render pass
            vk::end(*render_pass, cmd);

            // End command buffer recording
            vk::end(cmd);

            // Submit render
            vk::submit_helper_t::prepare()
                .wait_semaphores(img_avail_sems.handles)
                .signal_semaphores(render_finished_sems.handles)
                .cmd_buffer(&cmd)
                .wait_stage(vk::pipeline_stage_flags::color_attachment_output)
                .submit(device->queues.front(), fences.handles.back())
                .throw_if_error();

            // Present the rendered image
            auto present_res = vk::present_helper_t::prepare()
                                   .swapchain(*swapchain)
                                   .wait_semaphores(render_finished_sems.handles)
                                   .img_index(img_index)
                                   .present(device->queues.front());

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
            vk::device_idle(*device);
        }

        vk::device_idle(*device);

        // Terminate glfw
        window->destroy();
        println("- Destroyed window");

        vk::destroy(vertex_buffer);
        vk::destroy(views);
        vk::destroy(desc_pool);
        vk::destroy(fbs);
        vk::destroy(cmd_pool);
        vk::destroy(sync_objects);
        vk::destroy(*pipeline);
        vk::destroy(vs_shader_module);
        vk::destroy(fs_shader_module);
        vk::destroy(*render_pass);
        vk::destroy(*swapchain);
        vk::destroy(*device);
        vk::destroy(*instance);
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
