#pragma once

#include "orb/vk/device.hpp"
#include "orb/vk/render_pass.hpp"
#include "orb/vk/shaders.hpp"

#include <orb/result.hpp>

namespace orb::vk
{
    class color_blending_builder_t;
    class pipeline_builder_t;

    class pipeline_layout_builder_t
    {
    public:
        auto prepare_pipeline() -> pipeline_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkPipelineLayoutCreateInfo m_create_info {};

        pipeline_builder_t* m_next_builder = nullptr;
    };

    class desc_set_layout_builder_t
    {
    public:
        auto binding(ui32              binding,
                     descriptor_type   type,
                     ui32              count,
                     shader_stage_flag stage_flags)
            -> desc_set_layout_builder_t&
        {
            auto& binding_desc = m_bindings.emplace_back();

            binding_desc.binding         = binding;
            binding_desc.descriptorType  = vkenum(type);
            binding_desc.descriptorCount = count;
            binding_desc.stageFlags      = vkenum(stage_flags);

            return *this;
        }

        auto pipeline_layout() -> pipeline_layout_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkDescriptorSetLayoutCreateInfo m_create_info = structs::create::desc_set_layout();

        std::vector<VkDescriptorSetLayoutBinding> m_bindings;

        weak<device_t>             m_device       = nullptr;
        pipeline_layout_builder_t* m_next_builder = nullptr;
    };

    class color_blend_attachment_builder_t
    {
    public:
        auto color_write_mask(color_component component)
            -> color_blend_attachment_builder_t&
        {
            m_attachment_state->colorWriteMask |= vkenum(component);
            return *this;
        }

        auto blend_enable(bool enable)
            -> color_blend_attachment_builder_t&
        {
            m_attachment_state->blendEnable = enable;
            return *this;
        }

        auto src_color_blend_factor(blend_factor factor)
            -> color_blend_attachment_builder_t&
        {
            m_attachment_state->srcColorBlendFactor = vkenum(factor);
            return *this;
        }

        auto dst_color_blend_factor(blend_factor factor)
            -> color_blend_attachment_builder_t&
        {
            m_attachment_state->dstColorBlendFactor = vkenum(factor);
            return *this;
        }

        auto color_blend_op(blend_op op)
            -> color_blend_attachment_builder_t&
        {
            m_attachment_state->colorBlendOp = vkenum(op);
            return *this;
        }

        auto src_alpha_blend_factor(blend_factor factor)
            -> color_blend_attachment_builder_t&
        {
            m_attachment_state->srcAlphaBlendFactor = vkenum(factor);
            return *this;
        }

        auto dst_alpha_blend_factor(blend_factor factor)
            -> color_blend_attachment_builder_t&
        {
            m_attachment_state->dstAlphaBlendFactor = vkenum(factor);
            return *this;
        }

        auto alpha_blend_op(blend_op op)
            -> color_blend_attachment_builder_t&
        {
            m_attachment_state->alphaBlendOp = vkenum(op);
            return *this;
        }

        auto end_attachment()
            -> color_blending_builder_t&
        {
            return *m_color_blending_builder;
        }

    private:
        friend color_blending_builder_t;

        VkPipelineColorBlendAttachmentState* m_attachment_state {};
        color_blending_builder_t*            m_color_blending_builder = nullptr;
    };

    class color_blending_builder_t
    {
    public:
        auto new_color_blend_attachment() -> color_blend_attachment_builder_t&
        {
            auto& next_attachment                         = m_attachments.emplace_back();
            m_attachment_builder.m_attachment_state       = &next_attachment;
            m_attachment_builder.m_color_blending_builder = this;

            next_attachment.colorWriteMask = vkenum(color_component::r)
                                           | vkenum(color_component::g)
                                           | vkenum(color_component::b)
                                           | vkenum(color_component::a);

            next_attachment.blendEnable         = false;
            next_attachment.srcColorBlendFactor = vkenum(blend_factor::one);
            next_attachment.dstColorBlendFactor = vkenum(blend_factor::zero);
            next_attachment.colorBlendOp        = vkenum(blend_op::add);
            next_attachment.srcAlphaBlendFactor = vkenum(blend_factor::one);
            next_attachment.dstAlphaBlendFactor = vkenum(blend_factor::zero);
            next_attachment.alphaBlendOp        = vkenum(blend_op::add);

            return m_attachment_builder;
        }

        auto desc_set_layout() -> desc_set_layout_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkPipelineColorBlendStateCreateInfo m_create_info {};

        std::vector<VkPipelineColorBlendAttachmentState> m_attachments;
        color_blend_attachment_builder_t                 m_attachment_builder;
        desc_set_layout_builder_t*                       m_next_builder = nullptr;
    };

    class multisample_builder_t
    {
    public:
        auto sample_shading(bool enable) -> multisample_builder_t&
        {
            m_create_info.sampleShadingEnable = enable;
            return *this;
        }

        auto rasterization_samples(sample_count_flag counts) -> multisample_builder_t&
        {
            m_create_info.rasterizationSamples = vkenum(counts);
            return *this;
        }

        auto min_sample_shading(f32 min) -> multisample_builder_t&
        {
            m_create_info.minSampleShading = min;
            return *this;
        }

        auto sample_mask(VkSampleMask* mask) -> multisample_builder_t&
        {
            m_create_info.pSampleMask = mask;
            return *this;
        }

        auto alpha_to_coverage(bool enable) -> multisample_builder_t&
        {
            m_create_info.alphaToCoverageEnable = enable;
            return *this;
        }

        auto alpha_to_one(bool enable) -> multisample_builder_t&
        {
            m_create_info.alphaToOneEnable = enable;
            return *this;
        }

        auto color_blending() -> color_blending_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkPipelineMultisampleStateCreateInfo m_create_info {};

        color_blending_builder_t* m_next_builder = nullptr;
    };

    class rasterizer_builder_t
    {
    public:
        auto depth_clamp(bool clamp) -> rasterizer_builder_t&
        {
            m_create_info.depthClampEnable = clamp;
            return *this;
        }

        auto rasterizer_discard(bool discard) -> rasterizer_builder_t&
        {
            m_create_info.rasterizerDiscardEnable = discard;
            return *this;
        }

        auto polygon_mode(polygon_mode mode) -> rasterizer_builder_t&
        {
            m_create_info.polygonMode = vkenum(mode);
            return *this;
        }

        auto line_width(f32 width) -> rasterizer_builder_t&
        {
            m_create_info.lineWidth = width;
            return *this;
        }

        auto cull_mode(cull_mode mode) -> rasterizer_builder_t&
        {
            m_create_info.cullMode = vkenum(mode);
            return *this;
        }
        auto front_face(front_face face) -> rasterizer_builder_t&
        {
            m_create_info.frontFace = vkenum(face);
            return *this;
        }

        auto depth_bias(bool enable) -> rasterizer_builder_t&
        {
            m_create_info.depthBiasEnable = enable;
            return *this;
        }

        auto depth_bias_constant_factor(f32 factor) -> rasterizer_builder_t&
        {
            m_create_info.depthBiasConstantFactor = factor;
            return *this;
        }

        auto depth_bias_clamp(f32 clamp) -> rasterizer_builder_t&
        {
            m_create_info.depthBiasClamp = clamp;
            return *this;
        }

        auto depth_bias_slope_factor(f32 factor) -> rasterizer_builder_t&
        {
            m_create_info.depthBiasSlopeFactor = factor;
            return *this;
        }

        auto multisample() -> multisample_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkPipelineRasterizationStateCreateInfo m_create_info {};

        multisample_builder_t* m_next_builder = nullptr;
    };

    class viewport_state_builder_t
    {
    public:
        auto viewport(f32 x, f32 y, f32 width, f32 height, f32 min_depth, f32 max_depth) -> viewport_state_builder_t&
        {
            auto& viewport = m_viewports.emplace_back();

            viewport.x        = x;
            viewport.y        = y;
            viewport.width    = width;
            viewport.height   = height;
            viewport.minDepth = min_depth;
            viewport.maxDepth = max_depth;

            return *this;
        }

        auto scissor(i32 x, i32 y, ui32 width, ui32 height) -> viewport_state_builder_t&
        {
            auto& scissor = m_scissors.emplace_back();

            scissor.offset = { .x = x, .y = y };
            scissor.extent = { .width = width, .height = height };

            return *this;
        }

        auto rasterizer() -> rasterizer_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkPipelineViewportStateCreateInfo m_create_info {};

        std::vector<VkViewport> m_viewports;
        std::vector<VkRect2D>   m_scissors;

        rasterizer_builder_t* m_next_builder = nullptr;
    };

    class input_assembly_builder_t
    {
    public:
        auto primitive_restart(bool enable) -> input_assembly_builder_t&
        {
            m_create_info.primitiveRestartEnable = enable;
            return *this;
        }

        auto topology(primitive_topology topology) -> input_assembly_builder_t&
        {
            m_create_info.topology = vkenum(topology);
            return *this;
        }

        auto viewport_states() -> viewport_state_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkPipelineInputAssemblyStateCreateInfo m_create_info {};

        viewport_state_builder_t* m_next_builder = nullptr;
    };

    class vertex_input_builder_t
    {
    public:
        template <typename TVertexStruct>
        auto binding(ui32 binding, vertex_input_rate rate)
            -> vertex_input_builder_t&
        {
            auto& binding_desc = m_bindings.emplace_back();

            binding_desc.binding   = binding;
            binding_desc.stride    = sizeof(TVertexStruct);
            binding_desc.inputRate = vkenum(rate);

            return *this;
        }

        auto attribute(ui32 location, ui32 offset, vertex_format format)
            -> vertex_input_builder_t&
        {
            auto& attribute_desc = m_attributes.emplace_back();

            attribute_desc.location = location;
            attribute_desc.binding  = m_bindings.size() - 1;
            attribute_desc.format   = vkenum(format);
            attribute_desc.offset   = offset;

            return *this;
        }

        auto input_assembly() -> input_assembly_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkPipelineVertexInputStateCreateInfo m_create_info {};

        std::vector<VkVertexInputBindingDescription>   m_bindings;
        std::vector<VkVertexInputAttributeDescription> m_attributes;

        input_assembly_builder_t* m_next_builder = nullptr;
    };

    class dynamic_states_builder_t
    {
    public:
        auto dynamic_state(dynamic_state state) -> dynamic_states_builder_t&
        {
            m_states.push_back(vkenum(state));
            return *this;
        }

        auto vertex_input() -> vertex_input_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;

        VkPipelineDynamicStateCreateInfo m_create_info {};

        std::vector<VkDynamicState> m_states;
        vertex_input_builder_t*     m_next_builder = nullptr;
    };

    class shader_stages_builder_t
    {
    public:
        auto stage(shader_module_t&  module,
                   shader_stage_flag stage_flags,
                   const char*       name) -> shader_stages_builder_t&
        {
            auto& stage  = m_stages.emplace_back();
            stage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stage.module = module.handle;
            stage.stage  = vkenum(stage_flags);
            stage.pName  = name;

            return *this;
        }

        auto dynamic_states() -> dynamic_states_builder_t&
        {
            return *m_next_builder;
        }

    private:
        friend pipeline_builder_t;
        dynamic_states_builder_t*                    m_next_builder = nullptr;
        std::vector<VkPipelineShaderStageCreateInfo> m_stages;
    };

    struct graphics_pipeline_t
    {
        VkDevice              device          = nullptr;
        VkPipeline            handle          = nullptr;
        VkDescriptorSetLayout desc_set_layout = nullptr;
        VkPipelineLayout      layout          = nullptr;

        std::vector<VkViewport> viewports;
        std::vector<VkRect2D>   scissors;

        graphics_pipeline_t() = default;

        graphics_pipeline_t(const graphics_pipeline_t&)                    = delete;
        auto operator=(const graphics_pipeline_t&) -> graphics_pipeline_t& = delete;

        graphics_pipeline_t(graphics_pipeline_t&& other) noexcept
        {
            destroy();

            device          = other.device;
            handle          = other.handle;
            desc_set_layout = other.desc_set_layout;
            layout          = other.layout;

            viewports = std::move(other.viewports);
            scissors  = std::move(other.scissors);

            other.handle          = nullptr;
            other.desc_set_layout = nullptr;
            other.layout          = nullptr;
        }

        auto operator=(graphics_pipeline_t&& other) noexcept -> graphics_pipeline_t&
        {
            destroy();

            device          = other.device;
            handle          = other.handle;
            desc_set_layout = other.desc_set_layout;
            layout          = other.layout;

            viewports = std::move(other.viewports);
            scissors  = std::move(other.scissors);

            other.handle          = nullptr;
            other.desc_set_layout = nullptr;
            other.layout          = nullptr;

            return *this;
        }

        ~graphics_pipeline_t()
        {
            destroy();
        }

        void destroy()
        {
            if (desc_set_layout)
            {
                vkDestroyDescriptorSetLayout(device, desc_set_layout, nullptr);
                desc_set_layout = nullptr;
            }

            if (layout)
            {
                vkDestroyPipelineLayout(device, layout, nullptr);
                layout = nullptr;
            }

            if (handle)
            {
                vkDestroyPipeline(device, handle, nullptr);
                handle = nullptr;
            }
        }
    };

    class pipeline_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t> device) -> result<box<pipeline_builder_t>>
        {
            auto builder      = make_box<pipeline_builder_t>();
            builder->m_device = device;

            {
                auto& dynamic_states             = builder->m_dynamic_states.m_create_info;
                dynamic_states.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamic_states.dynamicStateCount = 0;
                dynamic_states.pDynamicStates    = nullptr;
            }

            {
                auto& vertex_input                           = builder->m_vertex_input.m_create_info;
                vertex_input.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertex_input.vertexBindingDescriptionCount   = 0;
                vertex_input.vertexAttributeDescriptionCount = 0;
            }

            {
                auto& input_assembly                  = builder->m_input_assembly.m_create_info;
                input_assembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                input_assembly.topology               = vkenum(primitive_topology::triangle_list);
                input_assembly.primitiveRestartEnable = false;
            }

            {
                auto& viewport_state         = builder->m_viewport_state.m_create_info;
                viewport_state.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewport_state.viewportCount = 0;
                viewport_state.pViewports    = nullptr;
                viewport_state.scissorCount  = 0;
                viewport_state.pScissors     = nullptr;
            }

            {
                auto& rasterizer                   = builder->m_rasterizer.m_create_info;
                rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizer.depthClampEnable        = false;
                rasterizer.rasterizerDiscardEnable = false;
                rasterizer.polygonMode             = vkenum(polygon_mode::fill);
                rasterizer.lineWidth               = 1.0f;
                rasterizer.cullMode                = vkenum(cull_mode::back);
                rasterizer.frontFace               = vkenum(front_face::clockwise);
                rasterizer.depthBiasEnable         = false;
            }

            {
                auto& multisample                 = builder->m_multisample.m_create_info;
                multisample.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisample.sampleShadingEnable   = false;
                multisample.rasterizationSamples  = vkenum(sample_count_flag::_1);
                multisample.minSampleShading      = 1.0f;
                multisample.pSampleMask           = nullptr;
                multisample.alphaToCoverageEnable = false;
                multisample.alphaToOneEnable      = false;
            }

            {
                auto& color_blending             = builder->m_color_blending.m_create_info;
                color_blending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                color_blending.logicOpEnable     = false;
                color_blending.logicOp           = VK_LOGIC_OP_COPY;
                color_blending.blendConstants[0] = 0.0f;
                color_blending.blendConstants[1] = 0.0f;
                color_blending.blendConstants[2] = 0.0f;
                color_blending.blendConstants[3] = 0.0f;
            }

            {
                auto desc_set_layout  = builder->m_desc_set_layout.m_create_info;
                desc_set_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            }

            {
                auto& pipeline_layout                  = builder->m_pipeline_layout.m_create_info;
                pipeline_layout.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                pipeline_layout.setLayoutCount         = 0;
                pipeline_layout.pSetLayouts            = nullptr;
                pipeline_layout.pushConstantRangeCount = 0;
                pipeline_layout.pPushConstantRanges    = nullptr;
            }

            builder->m_create_info.subpass            = 0;
            builder->m_create_info.basePipelineHandle = nullptr;
            builder->m_create_info.basePipelineIndex  = -1;

            builder->m_shader_stages.m_next_builder   = &builder->m_dynamic_states;
            builder->m_dynamic_states.m_next_builder  = &builder->m_vertex_input;
            builder->m_vertex_input.m_next_builder    = &builder->m_input_assembly;
            builder->m_input_assembly.m_next_builder  = &builder->m_viewport_state;
            builder->m_viewport_state.m_next_builder  = &builder->m_rasterizer;
            builder->m_rasterizer.m_next_builder      = &builder->m_multisample;
            builder->m_multisample.m_next_builder     = &builder->m_color_blending;
            builder->m_color_blending.m_next_builder  = &builder->m_desc_set_layout;
            builder->m_desc_set_layout.m_next_builder = &builder->m_pipeline_layout;
            builder->m_pipeline_layout.m_next_builder = builder.getmut().raw();

            return builder;
        }

        auto shader_stages() -> shader_stages_builder_t&
        {
            return m_shader_stages;
        }

        auto render_pass(weak<render_pass_t> render_pass) -> pipeline_builder_t&
        {
            m_create_info.renderPass = render_pass->handle;
            return *this;
        }

        auto subpass(ui32 subpass) -> pipeline_builder_t&
        {
            m_create_info.subpass = subpass;
            return *this;
        }

        auto base_pipeline(VkPipeline base_pipeline) -> pipeline_builder_t&
        {
            m_create_info.basePipelineHandle = base_pipeline;
            return *this;
        }

        auto base_pipeline_index(i32 index) -> pipeline_builder_t&
        {
            m_create_info.basePipelineIndex = index;
            return *this;
        }

        [[nodiscard]] auto build() -> result<box<graphics_pipeline_t>>
        {
            auto pipeline    = make_box<graphics_pipeline_t>();
            pipeline->device = m_device->handle;

            m_desc_set_layout.m_create_info.bindingCount = m_desc_set_layout.m_bindings.size();
            m_desc_set_layout.m_create_info.pBindings    = m_desc_set_layout.m_bindings.data();

            auto desc_set_layout_create_res = vkCreateDescriptorSetLayout(m_device->handle,
                                                                          &m_desc_set_layout.m_create_info,
                                                                          nullptr,
                                                                          &pipeline->desc_set_layout);

            if (desc_set_layout_create_res != vkres::ok)
            {
                return error_t { "Could not create descriptor set layout: {}",
                                 vkres::get_repr(desc_set_layout_create_res) };
            }

            m_pipeline_layout.m_create_info.setLayoutCount = 1;
            m_pipeline_layout.m_create_info.pSetLayouts    = &pipeline->desc_set_layout;

            auto pipeline_layout_create_res = vkCreatePipelineLayout(m_device->handle,
                                                                     &m_pipeline_layout.m_create_info,
                                                                     nullptr,
                                                                     &pipeline->layout);
            if (pipeline_layout_create_res != vkres::ok)
            {
                return error_t { "Could not create pipeline layout: {}",
                                 vkres::get_repr(pipeline_layout_create_res) };
            }

            pipeline->viewports = std::move(m_viewport_state.m_viewports);
            pipeline->scissors  = std::move(m_viewport_state.m_scissors);

            m_viewport_state.m_create_info.pViewports    = pipeline->viewports.data();
            m_viewport_state.m_create_info.viewportCount = pipeline->viewports.size();
            m_viewport_state.m_create_info.pScissors     = pipeline->scissors.data();
            m_viewport_state.m_create_info.scissorCount  = pipeline->scissors.size();

            m_dynamic_states.m_create_info.pDynamicStates    = m_dynamic_states.m_states.data();
            m_dynamic_states.m_create_info.dynamicStateCount = m_dynamic_states.m_states.size();

            m_color_blending.m_create_info.pAttachments    = m_color_blending.m_attachments.data();
            m_color_blending.m_create_info.attachmentCount = m_color_blending.m_attachments.size();

            m_vertex_input.m_create_info.pVertexAttributeDescriptions    = m_vertex_input.m_attributes.data();
            m_vertex_input.m_create_info.vertexAttributeDescriptionCount = m_vertex_input.m_attributes.size();
            m_vertex_input.m_create_info.pVertexBindingDescriptions      = m_vertex_input.m_bindings.data();
            m_vertex_input.m_create_info.vertexBindingDescriptionCount   = m_vertex_input.m_bindings.size();

            m_create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            m_create_info.pVertexInputState   = &m_vertex_input.m_create_info;
            m_create_info.pInputAssemblyState = &m_input_assembly.m_create_info;
            m_create_info.pViewportState      = &m_viewport_state.m_create_info;
            m_create_info.pRasterizationState = &m_rasterizer.m_create_info;
            m_create_info.pMultisampleState   = &m_multisample.m_create_info;
            m_create_info.pColorBlendState    = &m_color_blending.m_create_info;
            m_create_info.pDynamicState       = &m_dynamic_states.m_create_info;
            m_create_info.stageCount          = m_shader_stages.m_stages.size();
            m_create_info.pStages             = m_shader_stages.m_stages.data();

            m_create_info.layout = pipeline->layout;

            auto pipeline_create_res = vkCreateGraphicsPipelines(pipeline->device,
                                                                 nullptr,
                                                                 1,
                                                                 &m_create_info,
                                                                 nullptr,
                                                                 &pipeline->handle);

            if (pipeline_create_res != vkres::ok)
            {
                return error_t { "Could not create graphics pipeline: {}",
                                 vkres::get_repr(pipeline_create_res) };
            }

            return pipeline;
        }

    private:
        weak<device_t> m_device = nullptr;

        VkGraphicsPipelineCreateInfo m_create_info {};

        shader_stages_builder_t   m_shader_stages;
        dynamic_states_builder_t  m_dynamic_states;
        vertex_input_builder_t    m_vertex_input;
        input_assembly_builder_t  m_input_assembly;
        viewport_state_builder_t  m_viewport_state;
        rasterizer_builder_t      m_rasterizer;
        multisample_builder_t     m_multisample;
        color_blending_builder_t  m_color_blending;
        desc_set_layout_builder_t m_desc_set_layout;
        pipeline_layout_builder_t m_pipeline_layout;
    };
} // namespace orb::vk
