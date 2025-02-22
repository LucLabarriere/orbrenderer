#pragma once

#include "orb/box.hpp"
#include "orb/files.hpp"
#include "orb/result.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/vk_structs.hpp"

#include <shaderc/shaderc.hpp>
#include <string_view>

namespace orb::vk::shaders
{
    namespace kinds
    {
        using enum_t = shaderc_shader_kind;

        inline constexpr auto vertex          = shaderc_shader_kind::shaderc_vertex_shader;
        inline constexpr auto fragment        = shaderc_shader_kind::shaderc_fragment_shader;
        inline constexpr auto compute         = shaderc_shader_kind::shaderc_compute_shader;
        inline constexpr auto geometry        = shaderc_shader_kind::shaderc_geometry_shader;
        inline constexpr auto tess_control    = shaderc_shader_kind::shaderc_tess_control_shader;
        inline constexpr auto tess_evaluation = shaderc_shader_kind::shaderc_tess_evaluation_shader;
        inline constexpr auto raygen          = shaderc_shader_kind::shaderc_raygen_shader;
        inline constexpr auto anyhit          = shaderc_shader_kind::shaderc_anyhit_shader;
        inline constexpr auto closesthit      = shaderc_shader_kind::shaderc_closesthit_shader;
        inline constexpr auto miss            = shaderc_shader_kind::shaderc_miss_shader;
        inline constexpr auto intersection    = shaderc_shader_kind::shaderc_intersection_shader;
        inline constexpr auto callable        = shaderc_shader_kind::shaderc_callable_shader;
        inline constexpr auto task            = shaderc_shader_kind::shaderc_task_shader;
        inline constexpr auto mesh            = shaderc_shader_kind::shaderc_mesh_shader;

        inline constexpr auto glsl_vertex          = shaderc_shader_kind::shaderc_glsl_vertex_shader;
        inline constexpr auto glsl_fragment        = shaderc_shader_kind::shaderc_glsl_fragment_shader;
        inline constexpr auto glsl_compute         = shaderc_shader_kind::shaderc_glsl_compute_shader;
        inline constexpr auto glsl_geometry        = shaderc_shader_kind::shaderc_glsl_geometry_shader;
        inline constexpr auto glsl_tess_control    = shaderc_shader_kind::shaderc_glsl_tess_control_shader;
        inline constexpr auto glsl_tess_evaluation = shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader;
        inline constexpr auto glsl_raygen          = shaderc_shader_kind::shaderc_glsl_raygen_shader;
        inline constexpr auto glsl_anyhit          = shaderc_shader_kind::shaderc_glsl_anyhit_shader;
        inline constexpr auto glsl_closesthit      = shaderc_shader_kind::shaderc_glsl_closesthit_shader;
        inline constexpr auto glsl_miss            = shaderc_shader_kind::shaderc_glsl_miss_shader;
        inline constexpr auto glsl_intersection    = shaderc_shader_kind::shaderc_glsl_intersection_shader;
        inline constexpr auto glsl_callable        = shaderc_shader_kind::shaderc_glsl_callable_shader;
        inline constexpr auto glsl_task            = shaderc_shader_kind::shaderc_glsl_task_shader;
        inline constexpr auto glsl_mesh            = shaderc_shader_kind::shaderc_glsl_mesh_shader;
        inline constexpr auto glsl_infer           = shaderc_shader_kind::shaderc_glsl_infer_from_source;
    } // namespace kinds

    class spirv_compiler_t
    {
    public:
        auto option_source_language(shaderc_source_language lang) -> spirv_compiler_t&
        {
            m_options.SetSourceLanguage(lang);
            return *this;
        }

        auto option_target_env(shaderc_target_env env, shaderc_env_version version) -> spirv_compiler_t&
        {
            m_options.SetTargetEnvironment(env, version);
            return *this;
        }

        auto option_generate_debug_info() -> spirv_compiler_t&
        {
            m_options.SetGenerateDebugInfo();
            return *this;
        }

        auto option_target_spirv(shaderc_spirv_version version) -> spirv_compiler_t&
        {
            m_options.SetTargetSpirv(version);
            return *this;
        }

        auto option_optimization_level(shaderc_optimization_level level) -> spirv_compiler_t&
        {
            m_options.SetOptimizationLevel(level);
            return *this;
        }

        auto option_warnings_as_errors() -> spirv_compiler_t&
        {
            m_options.SetWarningsAsErrors();
            return *this;
        }

        auto option_suppress_warnings() -> spirv_compiler_t&
        {
            m_options.SetSuppressWarnings();
            return *this;
        }

        auto option_limit(shaderc_limit limit, int value) -> spirv_compiler_t&
        {
            m_options.SetLimit(limit, value);
            return *this;
        }

        auto option_auto_bind_uniforms(bool auto_bind) -> spirv_compiler_t&
        {
            m_options.SetAutoBindUniforms(auto_bind);
            return *this;
        }

        auto option_auto_combined_image_sampler(bool auto_sampled) -> spirv_compiler_t&
        {
            m_options.SetAutoSampledTextures(auto_sampled);
            return *this;
        }

        auto option_invert_y(bool enable) -> spirv_compiler_t&
        {
            m_options.SetInvertY(enable);
            return *this;
        }

        auto option_nan_clamp(bool enable) -> spirv_compiler_t&
        {
            m_options.SetNanClamp(enable);
            return *this;
        }

        auto option_binding_base_for_stage(shaderc_shader_kind kind, shaderc_uniform_kind uniform_kind, ui32 base) -> spirv_compiler_t&
        {
            m_options.SetBindingBaseForStage(kind, uniform_kind, base);
            return *this;
        }

        auto option_hlsl_io_mapping(bool hlsl_iomap) -> spirv_compiler_t&
        {
            m_options.SetHlslIoMapping(hlsl_iomap);
            return *this;
        }

        auto option_hlsl_offsets(bool hlsl_offsets) -> spirv_compiler_t&
        {
            m_options.SetHlslOffsets(hlsl_offsets);
            return *this;
        }

        auto option_hlsl_register_set_and_binding_for_stage(shaderc_shader_kind kind, const std::string& reg, const std::string& set, const std::string& binding) -> spirv_compiler_t&
        {
            m_options.SetHlslRegisterSetAndBindingForStage(kind, reg, set, binding);
            return *this;
        }

        auto option_hlsl_register_set_and_binding(const std::string& reg, const std::string& set, const std::string& binding) -> spirv_compiler_t&
        {
            m_options.SetHlslRegisterSetAndBinding(reg, set, binding);
            return *this;
        }

        auto option_hlsl_functionality1(bool enable) -> spirv_compiler_t&
        {
            m_options.SetHlslFunctionality1(enable);
            return *this;
        }

        auto option_hlsl_16bit_types(bool enable) -> spirv_compiler_t&
        {
            m_options.SetHlsl16BitTypes(enable);
            return *this;
        }

        auto preprocess_glsl(std::string_view    source,
                             shaderc_shader_kind kind,
                             const char*         entry_point = "main") -> shaderc::CompilationResult<char>
        {
            return m_compiler.PreprocessGlsl(source.data(), source.size(), kind, entry_point, m_options);
        }

        auto compile(const shaderc::CompilationResult<char>& source,
                     shaderc_shader_kind                     kind,
                     const char*                             entry_point = "main") -> shaderc::CompilationResult<ui32>
        {
            return m_compiler.CompileGlslToSpv(source.cbegin(), kind, entry_point, m_options);
        }

    private:
        shaderc::Compiler       m_compiler;
        shaderc::CompileOptions m_options;
    };

    struct module_t
    {
        VkShaderModule handle = nullptr;
        VkDevice       device = nullptr;
    };

    class module_builder_t
    {
    public:
        static auto prepare(weak<device_t>         device,
                            weak<spirv_compiler_t> compiler) -> result<module_builder_t>
        {
            module_builder_t builder {};
            builder.m_device   = device;
            builder.m_compiler = compiler;
            return builder;
        }

        auto kind(kinds::enum_t kind) -> module_builder_t&
        {
            m_kind = kind;
            return *this;
        }

        auto entry_point(const char* entry_point) -> module_builder_t&
        {
            m_entry_point = entry_point;
            return *this;
        }

        auto content(std::string content) -> module_builder_t&
        {
            m_content = std::move(content);
            return *this;
        }

        auto build() -> result<module_t>
        {
            auto preprocess_res = m_compiler->preprocess_glsl(m_content,
                                                              m_kind,
                                                              m_entry_point);

            if (preprocess_res.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                return error_t { "Could not preprocess shader: {}", preprocess_res.GetErrorMessage() };
            }

            auto compile_res = m_compiler->compile(preprocess_res,
                                                   m_kind,
                                                   m_entry_point);

            if (compile_res.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                return error_t { "Could not compile shader: {}", compile_res.GetErrorMessage() };
            }

            module_t module;
            module.device = m_device->handle;

            auto create_info     = structs::create::shader_module();
            create_info.codeSize = (compile_res.cend() - compile_res.cbegin()) * sizeof(unsigned int);
            create_info.pCode    = reinterpret_cast<const uint32_t*>(compile_res.cbegin());

            vkCreateShaderModule(module.device, &create_info, nullptr, &module.handle);

            return module;
        }

    private:
        weak<device_t>         m_device   = nullptr;
        weak<spirv_compiler_t> m_compiler = nullptr;

        kinds::enum_t m_kind { kinds::glsl_infer };
        std::string   m_content;
        const char*   m_entry_point = "main";
    };
} // namespace orb::vk::shaders
