#pragma once

#include "orb/vk/device.hpp"

#include <orb/box.hpp>
#include <orb/files.hpp>
#include <orb/result.hpp>

#include <string_view>

namespace orb::vk
{
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

        auto option_binding_base_for_stage(shaderc_shader_kind  kind,
                                           shaderc_uniform_kind uniform_kind,
                                           ui32                 base) -> spirv_compiler_t&
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

        auto option_hlsl_register_set_and_binding_for_stage(shaderc_shader_kind kind,
                                                            const std::string&  reg,
                                                            const std::string&  set,
                                                            const std::string&  binding) -> spirv_compiler_t&
        {
            m_options.SetHlslRegisterSetAndBindingForStage(kind, reg, set, binding);
            return *this;
        }

        auto option_hlsl_register_set_and_binding(const std::string& reg,
                                                  const std::string& set,
                                                  const std::string& binding) -> spirv_compiler_t&
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
                             const char*         entry_point = "main")
            -> shaderc::CompilationResult<char>
        {
            return m_compiler.PreprocessGlsl(source.data(),
                                             source.size(),
                                             kind,
                                             entry_point,
                                             m_options);
        }

        auto compile(const shaderc::CompilationResult<char>& source,
                     shaderc_shader_kind                     kind,
                     const char*                             entry_point = "main")
            -> shaderc::CompilationResult<ui32>
        {
            return m_compiler.CompileGlslToSpv(source.cbegin(),
                                               kind,
                                               entry_point,
                                               m_options);
        }

    private:
        shaderc::Compiler       m_compiler;
        shaderc::CompileOptions m_options;
    };

    struct shader_module_t
    {
        VkShaderModule handle = nullptr;
        VkDevice       device = nullptr;

        shader_module_t() = default;

        shader_module_t(const shader_module_t&)                    = delete;
        auto operator=(const shader_module_t&) -> shader_module_t& = delete;

        shader_module_t(shader_module_t&& other) noexcept
            : handle(other.handle), device(other.device)
        {
            other.handle = nullptr;
            other.device = nullptr;
        }

        auto operator=(shader_module_t&& other) noexcept -> shader_module_t&
        {
            handle = other.handle;
            device = other.device;

            other.handle = nullptr;
            other.device = nullptr;

            return *this;
        }

        ~shader_module_t()
        {
            destroy();
        }

        void destroy()
        {
            if (!handle) return;

            vkDestroyShaderModule(device, handle, nullptr);
            handle = nullptr;
        }
    };

    class shader_module_builder_t
    {
    public:
        [[nodiscard]] static auto prepare(weak<device_t>         device,
                            weak<spirv_compiler_t> compiler) -> result<shader_module_builder_t>
        {
            shader_module_builder_t builder {};
            builder.m_device   = device;
            builder.m_compiler = compiler;
            return builder;
        }

        auto kind(shader_kind kind) -> shader_module_builder_t&
        {
            m_kind = kind;
            return *this;
        }

        auto entry_point(const char* entry_point) -> shader_module_builder_t&
        {
            m_entry_point = entry_point;
            return *this;
        }

        auto content(std::string content) -> shader_module_builder_t&
        {
            m_content = std::move(content);
            return *this;
        }

        [[nodiscard]] auto build() -> result<shader_module_t>
        {
            auto preprocess_res = m_compiler->preprocess_glsl(m_content,
                                                              vkenum(m_kind),
                                                              m_entry_point);

            if (preprocess_res.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                return error_t { "Could not preprocess shader: {}", preprocess_res.GetErrorMessage() };
            }

            auto compile_res = m_compiler->compile(preprocess_res,
                                                   vkenum(m_kind),
                                                   m_entry_point);

            if (compile_res.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                return error_t { "Could not compile shader: {}", compile_res.GetErrorMessage() };
            }

            shader_module_t module;
            module.device = m_device->handle;

            auto create_info     = structs::create::shader_module();
            create_info.codeSize = (compile_res.cend() - compile_res.cbegin()) * sizeof(unsigned int);
            create_info.pCode    = reinterpret_cast<const uint32_t*>(compile_res.cbegin());

            if (auto res = vkCreateShaderModule(module.device, &create_info, nullptr, &module.handle); res != vkres::ok)
            {
                return error_t { "Could not create shader module: {}", vkres::get_repr(res) };
            }

            return module;
        }

    private:
        weak<device_t>         m_device   = nullptr;
        weak<spirv_compiler_t> m_compiler = nullptr;

        shader_kind m_kind { shader_kind::glsl_infer };
        std::string          m_content;
        const char*          m_entry_point = "main";
    };
} // namespace orb::vk
