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

        auto                    vs_content = vs_path.read_file().unwrap();
        auto                    fs_content = fs_path.read_file().unwrap();
        shaderc::Compiler       compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
        options.SetGenerateDebugInfo();
        options.SetTargetSpirv(shaderc_spirv_version_1_3);
        options.SetSourceLanguage(shaderc_source_language_glsl);
        options.SetOptimizationLevel(shaderc_optimization_level_zero);
        options.SetWarningsAsErrors();

        println("- Preprocessing shaders");
        auto vs_preprocess_res = compiler.PreprocessGlsl(vs_content, shaderc_shader_kind::shaderc_glsl_vertex_shader, "main", options);
        auto fs_preprocess_res = compiler.PreprocessGlsl(fs_content, shaderc_shader_kind::shaderc_glsl_fragment_shader, "main", options);

        if (vs_preprocess_res.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            println("Error: could not compile vertex shader: {}", vs_preprocess_res.GetErrorMessage());
            return 1;
        }

        if (vs_preprocess_res.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            println("Error: could not compile fragment shader: {}", fs_preprocess_res.GetErrorMessage());
            return 1;
        }

        println("- Compiling shaders");
        auto vs_compile_res = compiler.CompileGlslToSpv(vs_preprocess_res.cbegin(),
                                                        shaderc_shader_kind::shaderc_glsl_vertex_shader,
                                                        "main",
                                                        options);

        auto fs_compile_res = compiler.CompileGlslToSpv(fs_preprocess_res.cbegin(),
                                                        shaderc_shader_kind::shaderc_glsl_fragment_shader,
                                                        "main",
                                                        options);

        constexpr auto create_shader_module = [](VkDevice device, const auto& source) -> VkShaderModule {
            VkShaderModuleCreateInfo create_info {};
            create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            create_info.codeSize = (source.cend() - source.cbegin()) * sizeof(unsigned int);
            create_info.pCode    = reinterpret_cast<const uint32_t*>(source.cbegin());

            VkShaderModule shader_module {};
            vkCreateShaderModule(device, &create_info, nullptr, &shader_module);
            return shader_module;
        };

        println("- Creating shader modules");
        auto vs_shader_module = create_shader_module(device->handle, vs_compile_res);
        auto fs_shader_moduel = create_shader_module(device->handle, fs_compile_res);

        constexpr auto create_shader_stage = [](VkShaderModule module, VkShaderStageFlagBits stage) -> VkPipelineShaderStageCreateInfo {
            VkPipelineShaderStageCreateInfo create_info {};
            create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            create_info.stage  = stage;
            create_info.module = module;
            create_info.pName  = "main";
            return create_info;
        };

        println("- Creating shader stages");
        auto vs_shader_stage = create_shader_stage(vs_shader_module, VK_SHADER_STAGE_VERTEX_BIT);
        auto fs_shader_stage = create_shader_stage(fs_shader_moduel, VK_SHADER_STAGE_FRAGMENT_BIT);

        constexpr auto create_dynamic_state = []() -> VkPipelineDynamicStateCreateInfo {
            static std::array<VkDynamicState, 2> dynamic_states { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
            VkPipelineDynamicStateCreateInfo     create_info {};
            create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            create_info.dynamicStateCount = dynamic_states.size();
            create_info.pDynamicStates    = dynamic_states.data();
            return create_info;
        };

        println("- Creating dynamic state");
        auto dynamic_state = create_dynamic_state();

        // Vertex input, for now we don't have any vertex data
        constexpr auto create_vertex_input = []() -> VkPipelineVertexInputStateCreateInfo {
            VkPipelineVertexInputStateCreateInfo create_info {};
            create_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            create_info.vertexBindingDescriptionCount   = 0;
            create_info.vertexAttributeDescriptionCount = 0;
            return create_info;
        };

        println("- Creating vertex input");
        auto vertex_input = create_vertex_input();

        // Input assembly
        constexpr auto create_input_assembly = []() -> VkPipelineInputAssemblyStateCreateInfo {
            VkPipelineInputAssemblyStateCreateInfo create_info {};
            create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            create_info.primitiveRestartEnable = VK_FALSE;
            return create_info;
        };

        println("- Creating input assembly");
        auto input_assembly = create_input_assembly();

        // Viewport and scissor
        constexpr auto create_viewport = [](ui32 w, ui32 h) -> VkViewport {
            VkViewport viewport {};
            viewport.x        = 0.0f;
            viewport.y        = 0.0f;
            viewport.width    = static_cast<float>(w);
            viewport.height   = static_cast<float>(h);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            return viewport;
        };

        constexpr auto create_scissor = [](ui32 w, ui32 h) -> VkRect2D {
            VkRect2D scissor {};
            scissor.offset = { .x = 0, .y = 0 };
            scissor.extent = { .width = w, .height = h };
            return scissor;
        };

        println("- Creating viewport and scissor");
        auto viewport = create_viewport(swapchain->width, swapchain->height);
        auto scissor  = create_scissor(swapchain->width, swapchain->height);

        constexpr auto create_viewport_state = [](const VkViewport& viewport, const VkRect2D& scissor)
            -> VkPipelineViewportStateCreateInfo {
            VkPipelineViewportStateCreateInfo create_info {};
            create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            create_info.viewportCount = 1;
            create_info.pViewports    = &viewport;
            create_info.scissorCount  = 1;
            create_info.pScissors     = &scissor;
            return create_info;
        };

        println("- Creating viewport state");
        auto viewport_state = create_viewport_state(viewport, scissor);

        constexpr auto create_rasterizer = []() -> VkPipelineRasterizationStateCreateInfo {
            VkPipelineRasterizationStateCreateInfo create_info {};
            create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            create_info.depthClampEnable        = VK_FALSE;
            create_info.rasterizerDiscardEnable = VK_FALSE;
            create_info.polygonMode             = VK_POLYGON_MODE_FILL;
            create_info.lineWidth               = 1.0f;
            create_info.cullMode                = VK_CULL_MODE_FRONT_BIT;
            create_info.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            create_info.depthBiasEnable         = VK_FALSE;
            return create_info;
        };

        println("- Creating rasterizer");
        auto rasterizer = create_rasterizer();

        constexpr auto create_multisampling = []() -> VkPipelineMultisampleStateCreateInfo {
            VkPipelineMultisampleStateCreateInfo create_info {};
            create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            create_info.sampleShadingEnable   = VK_FALSE;
            create_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
            create_info.minSampleShading      = 1.0f;
            create_info.pSampleMask           = nullptr;
            create_info.alphaToCoverageEnable = VK_FALSE;
            create_info.alphaToOneEnable      = VK_FALSE;
            return create_info;
        };

        println("- Creating multisampling");
        auto multisampling = create_multisampling();

        // Color blending
        constexpr auto create_color_blend_attachment = []() -> VkPipelineColorBlendAttachmentState {
            VkPipelineColorBlendAttachmentState create_info {};

            create_info.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                                       | VK_COLOR_COMPONENT_G_BIT
                                       | VK_COLOR_COMPONENT_B_BIT
                                       | VK_COLOR_COMPONENT_A_BIT;

            create_info.blendEnable         = VK_FALSE;
            create_info.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            create_info.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
            create_info.colorBlendOp        = VK_BLEND_OP_ADD;
            create_info.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            create_info.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            create_info.alphaBlendOp        = VK_BLEND_OP_ADD;

            return create_info;
        };

        println("- Creating color blend attachment");
        auto color_blend_attachment = create_color_blend_attachment();

        // Color blending
        constexpr auto create_color_blending = [](const VkPipelineColorBlendAttachmentState& attachment)
            -> VkPipelineColorBlendStateCreateInfo {
            VkPipelineColorBlendStateCreateInfo create_info {};
            create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            create_info.logicOpEnable     = VK_FALSE;
            create_info.logicOp           = VK_LOGIC_OP_COPY;
            create_info.attachmentCount   = 1;
            create_info.pAttachments      = &attachment;
            create_info.blendConstants[0] = 0.0f;
            create_info.blendConstants[1] = 0.0f;
            create_info.blendConstants[2] = 0.0f;
            create_info.blendConstants[3] = 0.0f;
            return create_info;
        };

        println("- Creating color blending");
        auto color_blending = create_color_blending(color_blend_attachment);

        // Pipeline layout
        constexpr auto create_pipeline_layout = [](VkDevice device) -> VkPipelineLayout {
            VkPipelineLayoutCreateInfo create_info {};
            create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            create_info.setLayoutCount         = 0;
            create_info.pSetLayouts            = nullptr;
            create_info.pushConstantRangeCount = 0;
            create_info.pPushConstantRanges    = nullptr;

            VkPipelineLayout pipeline_layout {};
            vkCreatePipelineLayout(device, &create_info, nullptr, &pipeline_layout);
            return pipeline_layout;
        };

        println("- Creating pipeline layout");
        auto pipeline_layout = create_pipeline_layout(device->handle);

        // Create the graphics pipeline
        constexpr auto create_graphics_pipeline = [](VkDevice                                      device,
                                                     const VkPipelineShaderStageCreateInfo&        vs,
                                                     const VkPipelineShaderStageCreateInfo&        fs,
                                                     const VkPipelineVertexInputStateCreateInfo&   vertex_input,
                                                     const VkPipelineInputAssemblyStateCreateInfo& input_assembly,
                                                     const VkPipelineViewportStateCreateInfo&      viewport_state,
                                                     const VkPipelineRasterizationStateCreateInfo& rasterizer,
                                                     const VkPipelineMultisampleStateCreateInfo&   multisampling,
                                                     const VkPipelineColorBlendStateCreateInfo&    color_blending,
                                                     const VkPipelineDynamicStateCreateInfo&       dynamic_state,
                                                     VkPipelineLayout                              layout,
                                                     VkRenderPass                                  render_pass) -> VkPipeline {
            std::array<VkPipelineShaderStageCreateInfo, 2> stages { vs, fs };
            VkGraphicsPipelineCreateInfo                   create_info {};
            create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            create_info.stageCount          = stages.size();
            create_info.pStages             = stages.data();
            create_info.pVertexInputState   = &vertex_input;
            create_info.pInputAssemblyState = &input_assembly;
            create_info.pViewportState      = &viewport_state;
            create_info.pRasterizationState = &rasterizer;
            create_info.pMultisampleState   = &multisampling;
            create_info.pColorBlendState    = &color_blending;
            create_info.pDynamicState       = &dynamic_state;
            create_info.layout              = layout;
            create_info.renderPass          = render_pass;
            create_info.subpass             = 0;
            create_info.basePipelineHandle  = VK_NULL_HANDLE;
            create_info.basePipelineIndex   = -1;

            VkPipeline pipeline {};
            vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline);
            return pipeline;
        };

        println("- Creating graphics pipeline");
        auto pipeline = create_graphics_pipeline(device->handle,
                                                 vs_shader_stage,
                                                 fs_shader_stage,
                                                 vertex_input,
                                                 input_assembly,
                                                 viewport_state,
                                                 rasterizer,
                                                 multisampling,
                                                 color_blending,
                                                 dynamic_state,
                                                 pipeline_layout,
                                                 render_pass->handle);

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
            vk::wait_and_reset_fences(fences).throw_if_error();

            // Reset fences
            vk::reset_fences(fences).throw_if_error();

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

            uint32_t img_index = res.img_index();

            // Render to the framebuffer
            render_pass->begin_info.framebuffer       = fbs.handles[img_index];
            render_pass->begin_info.renderArea.extent = swapchain->extent;

            // Begin command buffer recording
            auto [cmd, begin_res] = cmd_buffers.begin_one_time(frame);

            // Begin the render pass
            vk::begin(*render_pass, cmd);

            // Bind the graphics pipeline
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

            // Set viewport and scissor
            viewport.width  = static_cast<float>(swapchain->width);
            viewport.height = static_cast<float>(swapchain->height);
            vkCmdSetViewport(cmd, 0, 1, &viewport);
            vkCmdSetScissor(cmd, 0, 1, &scissor);

            // Draw triangle
            vkCmdDraw(cmd, 3, 1, 0, 0);

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
