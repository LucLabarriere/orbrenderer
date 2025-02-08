#include <span>
#include <thread>

#include <shaderc/shaderc.hpp>

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
        begin_chrono();

        glfw::driver_t::initialize().throw_if_error();
        auto w = glfw::driver_t::create_window_for_vk().unwrap();

        print_chrono("- GLFW window create time: {}");
        return w;
    }

    /* @brief Creates the vulkan instance
     *
     * @return The vulkan instance
     */
    [[nodiscard]] auto create_vk_instance() -> box<vk::instance_t>
    {
        constexpr auto portability = vk::khr_extensions::portability_enumeration;
        begin_chrono();

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

        print_chrono("- Vulkan instance create time: {}");
        return instance;
    }

    /* @brief Selects the GPU
     *
     * @param instance The vulkan instance
     * @return The selected GPU
     */
    [[nodiscard]] auto create_vk_gpu(vk::instance_t& instance) -> box<vk::gpu_t>
    {
        begin_chrono();

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

        print_chrono("- GPU selection time: {}");

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
        begin_chrono();

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
        print_chrono("- Vulkan device create time: {}");
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
        begin_chrono();

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
        print_chrono("- Vulkan swapchain create time: {}");
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
        begin_chrono();
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

        print_chrono("- Render pass create time: {}");
        return render_pass;
    }

    [[nodiscard]] auto create_image_views(vk::device_t& device, std::span<VkImage> images) -> vk::views_t
    {
        begin_chrono();
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
        print_chrono("- Image views create time: {}");
        return views;
    }

    [[nodiscard]] auto create_framebuffers(vk::device_t& device,
                                           VkRenderPass  pass,
                                           vk::views_t&  views,
                                           ui32          w,
                                           ui32          h) -> vk::framebuffers_t
    {
        begin_chrono();

        auto imgui_fbs_builder = vk::framebuffers_builder_t::prepare(&device, pass)
                                     .unwrap()
                                     .size(w, h);

        for (const auto& [img, view] : views.handles)
        {
            imgui_fbs_builder.attachment(view);
        }

        auto imgui_fbs = imgui_fbs_builder
                             .build()
                             .unwrap();

        print_chrono("- ImGui framebuffers create time: {}");
        return imgui_fbs;
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

        const path vs_path { SAMPLE_DIR "main.vs.glsl" };
        const path fs_path { SAMPLE_DIR "main.fs.glsl" };

        auto                    vs_content = vs_path.read_file().unwrap();
        auto                    fs_content = fs_path.read_file().unwrap();
        println("vs_content: {}", vs_content);
        println("fs_content: {}", fs_content);
        shaderc::Compiler       compiler;
        shaderc::CompileOptions options;

        auto vs_compile_res = compiler.PreprocessGlsl(vs_content, shaderc_shader_kind::shaderc_glsl_vertex_shader, "main", options);
        auto fs_compile_res = compiler.PreprocessGlsl(fs_content, shaderc_shader_kind::shaderc_glsl_fragment_shader, "main", options);

        if (vs_compile_res.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            println("Error: could not compile vertex shader: {}", vs_compile_res.GetErrorMessage());
            return 1;
        }

        if (vs_compile_res.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            println("Error: could not compile fragment shader: {}", fs_compile_res.GetErrorMessage());
            return 1;
        }

        box<vk::render_pass_t> render_pass = create_render_pass(device->handle, swapchain->format.format);
        vk::views_t            views       = create_image_views(*device, swapchain->images);
        vk::framebuffers_t     fbs         = create_framebuffers(*device,
                                                     render_pass->handle,
                                                     views,
                                                     swapchain->width,
                                                     swapchain->height);

        auto desc_pool = vk::desc_pool_builder_t::prepare(device.getmut())
                             .unwrap()
                             .pool(vk::desc_types::sampler, 100)
                             .flag(vk::descriptor_pool_create_flags::free_descriptor_set_bit)
                             .build()
                             .unwrap();

        // Synchronization
        auto sync_objects = vk::sync_objects_builder_t::prepare(device.getmut())
                                .unwrap()
                                .semaphores(max_frames_in_flight * 2)
                                .fences(max_frames_in_flight)
                                .build()
                                .unwrap();

        auto cmd_pool = vk::cmd_pool_builder_t::prepare(device.getmut(), gpu->queue_families.front().index)
                            .unwrap()
                            .flag(vk::command_pool_create_flags::reset_command_buffer_bit)
                            .build()
                            .unwrap();

        auto cmd_buffers = vk::alloc_cmds(cmd_pool,
                                          max_frames_in_flight,
                                          vk::cmd_buffer_levels::primary)
                               .unwrap();

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

            auto fence           = sync_objects.subspan_fences(frame, 1);
            auto img_avail       = sync_objects.subspan_semaphores(frame, 1);
            auto render_finished = sync_objects.subspan_semaphores(frame + max_frames_in_flight, 1);

            // Wait fences
            vk::wait_fences(fence).throw_if_error();

            // Acquire the next swapchain image
            auto res = vk::acquire_img(*swapchain, img_avail.handles.back(), nullptr);

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
            vk::reset_fences(fence).throw_if_error();

            uint32_t img_index = res.img_index();

            // Render to the imgui pass framebuffer
            render_pass->begin_info.framebuffer       = fbs.handles[frame];
            render_pass->begin_info.renderArea.extent = swapchain->extent;

            // Begin command buffer recording
            auto [cmd, begin_res] = cmd_buffers.begin_one_time(frame);

            // Begin the render pass
            vk::begin(*render_pass, cmd);

            // Render ImGui
            vk::imgui::submit_render(cmd);

            // End the render pass
            vk::end(*render_pass, cmd);

            // End command buffer recording
            vk::end(cmd);

            // Submit render
            vk::submit_helper_t::prepare()
                .wait_semaphores(img_avail.handles)
                .signal_semaphores(render_finished.handles)
                .cmd_buffer(&cmd)
                .wait_stage(vk::pipeline_stage_flags::color_attachment_output)
                .submit(device->queues.front(), nullptr)
                .throw_if_error();

            // Present the rendered image
            auto present_res = vk::present_helper_t::prepare()
                                   .swapchain(*swapchain)
                                   .wait_semaphores(render_finished.handles)
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
        }

        vk::device_idle(*device);

        // Terminate glfw
        window->destroy();
        println("- Destroyed window");

        vk::imgui::terminate();

        vk::destroy(views);
        vk::destroy(desc_pool);
        vk::destroy(fbs);
        vk::destroy(cmd_pool);
        vk::destroy(sync_objects);
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
