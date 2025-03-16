#include <array>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include <orb/eval.hpp>
#include <orb/flux.hpp>
#include <orb/glfw.hpp>
#include <orb/time.hpp>
#include <orb/vk/all.hpp>

using namespace orb;

namespace
{
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
     * @param surface The vulkan surface
     * @return The vulkan swapchain
     */
    [[nodiscard]] auto create_vk_swapchain(vk::instance_t& instance,
                                           vk::device_t&   device,
                                           vk::gpu_t&      gpu,
                                           glfw::window_t& window,
                                           vk::surface_t&  surface) -> box<vk::swapchain_t>
    {
        auto swapchain = vk::swapchain_builder_t::prepare(&instance,
                                                          &gpu,
                                                          &device,
                                                          &window,
                                                          &surface)
                             .unwrap()
                             .fb_dimensions_from_window()
                             .present_queue_family_index(0)

                             .usage(vk::image_usage_flags::transfer_dst)
                             .usage(vk::image_usage_flags::color_attachment)
                             .color_space(vk::color_spaces::srgb_nonlinear_khr)
                             .format(vk::formats::b8g8r8a8_unorm)
                             .format(vk::formats::r8g8b8a8_unorm)
                             .format(vk::formats::b8g8r8_unorm)
                             .format(vk::formats::r8g8b8_unorm)

                             .present_mode(vk::present_modes::mailbox_khr)
                             .present_mode(vk::present_modes::immediate_khr)
                             .present_mode(vk::present_modes::fifo_khr)

                             .build()
                             .unwrap();

        return swapchain;
    }

    /* @brief Creates the ImGui render pass
     *
     * @param device The vulkan device
     * @param sc_img_format The swapchain image format
     * @return The ImGui render pass
     */
    [[nodiscard]] auto create_imgui_pass(VkDevice device, VkFormat sc_img_format) -> box<vk::render_pass_t>
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

        auto imgui_pass = vk::render_pass_builder_t::prepare(device)
                              .unwrap()
                              .clear_color({ 0.0f, 0.0f, 0.0f, 1.0f })
                              .build(subpasses, attachments)
                              .unwrap();
        imgui_pass->bind_color();

        return imgui_pass;
    }

    [[nodiscard]] auto create_swapchain_views(vk::device_t& device, std::span<VkImage> images) -> vk::views_t
    {
        auto builder = vk::views_builder_t::prepare(device.handle)
                           .unwrap();

        for (const auto& img : images)
        {
            builder.image(img);
        }

        auto views = builder
                         .aspect_mask(vk::image_aspect_flags::color)
                         .format(vk::formats::b8g8r8a8_unorm)
                         .build()
                         .unwrap();
        return views;
    }

    [[nodiscard]] auto create_imgui_framebuffers(vk::device_t& device,
                                                 VkRenderPass  pass,
                                                 vk::views_t&  views,
                                                 ui32          w,
                                                 ui32          h) -> vk::framebuffers_t
    {
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

        return imgui_fbs;
    }

} // namespace

auto main() -> int
{
    static constexpr ui32 max_frames_in_flight = 2;

    try
    {
        box<glfw::driver_t> glfw_driver = glfw::driver_t::create().unwrap();

        weak<glfw::window_t> window   = glfw_driver->create_window_for_vk().unwrap();
        box<vk::instance_t>  instance = create_vk_instance();

        vk::surface_t          surface    = vk::surface_builder_t::prepare(instance->handle, window).build().unwrap();
        box<vk::gpu_t>         gpu        = create_vk_gpu(*instance);
        box<vk::device_t>      device     = create_vk_device(*instance, *gpu);
        box<vk::swapchain_t>   swapchain  = create_vk_swapchain(*instance, *device, *gpu, *window, surface);
        box<vk::render_pass_t> imgui_pass = create_imgui_pass(device->handle, swapchain->format.format);
        vk::views_t            views      = create_swapchain_views(*device, swapchain->images);
        vk::framebuffers_t     imgui_fbs  = create_imgui_framebuffers(*device,
                                                                 imgui_pass->handle,
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

        auto cmd_buffers = cmd_pool->alloc_cmds(max_frames_in_flight).unwrap();

        auto imgui_driver = vk::imgui_driver_builder_t::prepare(window,
                                                                instance.getmut(),
                                                                gpu.getmut(),
                                                                device.getmut(),
                                                                swapchain.getmut(),
                                                                &desc_pool,
                                                                imgui_pass.getmut())
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
            auto render_finished = sync_objects.semaphores(frame + max_frames_in_flight, 1);

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
                views     = create_swapchain_views(*device, swapchain->images);
                imgui_fbs = create_imgui_framebuffers(*device,
                                                      imgui_pass->handle,
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
            fence.reset().unwrap();

            uint32_t img_index = res.img_index();

            // Render to the imgui pass framebuffer
            imgui_pass->begin_info.framebuffer       = imgui_fbs.handles[img_index];
            imgui_pass->begin_info.renderArea.extent = swapchain->extent;

            // Begin command buffer recording
            auto cmd = cmd_buffers.get(frame).unwrap();
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
                .wait_stage(vk::pipeline_stage_flags::color_attachment_output)
                .submit(device->queues.front(), fence.handles.back())
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

        device->wait().unwrap();
    }
    catch (const orb::exception& e)
    {
        println("Fatal error: {}", e.what());
        return 1;
    }

    return 0;
}
