#pragma once

#include "orb/vk/vma.hpp"

#include <orb/assert.hpp>

#include <array>
#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan_core.h>

namespace orb::vk
{
    namespace khr_extensions
    {
        inline constexpr const char* device_properties_2     = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
        inline constexpr const char* portability_enumeration = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
        inline constexpr const char* surface                 = "VK_KHR_surface";
        inline constexpr const char* win32_surface           = "VK_KHR_win32_surface";
        inline constexpr const char* swapchain               = "VK_KHR_swapchain";
        inline constexpr const char* buffer_device_address   = "VK_KHR_buffer_device_address";
    } // namespace khr_extensions

    namespace extensions
    {
        inline constexpr const char* debug_report = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
        inline constexpr const char* debug_utils  = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    } // namespace extensions

    namespace validation_layers
    {
        inline constexpr const char* validation = "VK_LAYER_KHRONOS_validation";
    } // namespace validation_layers

    namespace gpu_types
    {
        using namespace std::literals;

        using enum_t = VkPhysicalDeviceType;

        inline constexpr auto       other       = VK_PHYSICAL_DEVICE_TYPE_OTHER;
        inline constexpr auto       integrated  = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
        inline constexpr auto       discrete    = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        inline constexpr auto       virtual_gpu = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
        inline constexpr auto       cpu         = VK_PHYSICAL_DEVICE_TYPE_CPU;
        inline constexpr std::array strings {
            "Other type"sv, "Integrated"sv, "Discrete"sv, "Virtual"sv, "CPU"sv
        };
    } // namespace gpu_types

    namespace queue_families
    {
        using enum_t = VkQueueFlagBits;

        inline constexpr auto graphics = VK_QUEUE_GRAPHICS_BIT;
        inline constexpr auto transfer = VK_QUEUE_TRANSFER_BIT;
        inline constexpr auto compute  = VK_QUEUE_COMPUTE_BIT;
        inline constexpr auto sparse   = VK_QUEUE_SPARSE_BINDING_BIT;
    } // namespace queue_families

    namespace instance_create
    {
        using enum_t = VkInstanceCreateFlagBits;

        inline constexpr auto portability = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    } // namespace instance_create

    namespace proc_addresses
    {
        inline auto create_deb_report_callback(VkInstance instance)
        {
            using create_debug_report_callback = PFN_vkCreateDebugReportCallbackEXT;
            auto fn                            = (create_debug_report_callback)vkGetInstanceProcAddr( // NOLINT
                instance,
                "vkCreateDebugReportCallbackEXT");
            orbassert(fn != nullptr, "Coult not retrieve vkCreateDebugReportCallbackEXT function");
            return fn;
        }

        inline auto create_deb_utils(VkInstance instance)
        {
            using create_debug_utils = PFN_vkCreateDebugUtilsMessengerEXT;
            auto fn                  = (create_debug_utils)vkGetInstanceProcAddr( // NOLINT
                instance,
                "vkCreateDebugUtilsMessengerEXT");
            orbassert(fn != nullptr, "Coult not retrieve vkCreateDebugUtilsMessengerEXT function");
            return fn;
        }

        using set_debug_name_fn_t = PFN_vkSetDebugUtilsObjectNameEXT;
        inline auto set_debug_name(VkInstance instance)
        {
            auto fn = (set_debug_name_fn_t)vkGetInstanceProcAddr( // NOLINT
                instance,
                "vkSetDebugUtilsObjectNameEXT");
            orbassert(fn != nullptr, "Coult not retrieve vkSetDebugUtilsObjectNameEXT function");
            return fn;
        }

        inline auto destroy_deb_report_callback(VkInstance instance)
        {
            using destroy_debug_report_callback = PFN_vkDestroyDebugReportCallbackEXT;
            auto fn                             = (destroy_debug_report_callback)vkGetInstanceProcAddr( // NOLINT
                instance,
                "vkDestroyDebugReportCallbackEXT");
            orbassert(fn != nullptr, "Coult not retrieve vkDestroyDebugReportCallbackEXT function");
            return fn;
        }

        inline auto destroy_deb_utils(VkInstance instance)
        {
            using destroy_debug_utils = PFN_vkDestroyDebugUtilsMessengerEXT;
            auto fn                   = (destroy_debug_utils)vkGetInstanceProcAddr( // NOLINT
                instance,
                "vkDestroyDebugUtilsMessengerEXT");
            orbassert(fn != nullptr, "Coult not retrieve vkDestroyDebugUtilsMessengerEXT function");
            return fn;
        }
    } // namespace proc_addresses

    namespace desc_types
    {
        using enum_t = VkDescriptorType;

        inline constexpr auto sampler                    = VK_DESCRIPTOR_TYPE_SAMPLER;
        inline constexpr auto combined_image_sampler     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        inline constexpr auto sampled_image              = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        inline constexpr auto storage_image              = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        inline constexpr auto uniform_texel_buffer       = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        inline constexpr auto storage_texel_buffer       = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        inline constexpr auto uniform_buffer             = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        inline constexpr auto storage_buffer             = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        inline constexpr auto uniform_buffer_dynamic     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        inline constexpr auto storage_buffer_dynamic     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        inline constexpr auto input_attachment           = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        inline constexpr auto inline_uniform_block       = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;
        inline constexpr auto acceleration_structure_khr = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
        inline constexpr auto acceleration_structure_nv  = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
        inline constexpr auto sample_weight_image_qcom   = VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM;
        inline constexpr auto block_match_image_qcom     = VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM;
        inline constexpr auto mutable_ext                = VK_DESCRIPTOR_TYPE_MUTABLE_EXT;
        inline constexpr auto inline_uniform_block_ext   = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;
        inline constexpr auto mutable_valve              = VK_DESCRIPTOR_TYPE_MUTABLE_VALVE;
    } // namespace desc_types

    namespace attachment_load_ops
    {
        using enum_t = VkAttachmentLoadOp;

        inline constexpr auto load      = VK_ATTACHMENT_LOAD_OP_LOAD;
        inline constexpr auto clear     = VK_ATTACHMENT_LOAD_OP_CLEAR;
        inline constexpr auto dont_care = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        inline constexpr auto none      = VK_ATTACHMENT_LOAD_OP_NONE_EXT;
    } // namespace attachment_load_ops

    namespace attachment_store_ops
    {
        using enum_t = VkAttachmentStoreOp;

        inline constexpr auto store     = VK_ATTACHMENT_STORE_OP_STORE;
        inline constexpr auto dont_care = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        inline constexpr auto none      = VK_ATTACHMENT_STORE_OP_NONE;
    }; // namespace attachment_store_ops

    namespace pipeline_bind_points
    {
        using enum_t = VkPipelineBindPoint;

        inline constexpr auto graphics        = VK_PIPELINE_BIND_POINT_GRAPHICS;
        inline constexpr auto compute         = VK_PIPELINE_BIND_POINT_COMPUTE;
        inline constexpr auto ray_tracing_khr = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
        inline constexpr auto ray_tracing_nv  = VK_PIPELINE_BIND_POINT_RAY_TRACING_NV;
    }; // namespace pipeline_bind_points

    namespace cmd_buffer_levels
    {
        using enum_t = VkCommandBufferLevel;

        inline constexpr auto primary   = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        inline constexpr auto secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    }; // namespace cmd_buffer_levels

    namespace access_flags
    {
        using enum_t = VkAccessFlagBits;

        inline constexpr auto indirect_command_read                     = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
        inline constexpr auto index_read                                = VK_ACCESS_INDEX_READ_BIT;
        inline constexpr auto vertex_attribute_read                     = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
        inline constexpr auto uniform_read                              = VK_ACCESS_UNIFORM_READ_BIT;
        inline constexpr auto input_attachment_read                     = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        inline constexpr auto shader_read                               = VK_ACCESS_SHADER_READ_BIT;
        inline constexpr auto shader_write                              = VK_ACCESS_SHADER_WRITE_BIT;
        inline constexpr auto color_attachment_read                     = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        inline constexpr auto color_attachment_write                    = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        inline constexpr auto depth_stencil_attachment_read             = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        inline constexpr auto depth_stencil_attachment_write            = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        inline constexpr auto transfer_read                             = VK_ACCESS_TRANSFER_READ_BIT;
        inline constexpr auto transfer_write                            = VK_ACCESS_TRANSFER_WRITE_BIT;
        inline constexpr auto host_read                                 = VK_ACCESS_HOST_READ_BIT;
        inline constexpr auto host_write                                = VK_ACCESS_HOST_WRITE_BIT;
        inline constexpr auto memory_read                               = VK_ACCESS_MEMORY_READ_BIT;
        inline constexpr auto memory_write                              = VK_ACCESS_MEMORY_WRITE_BIT;
        inline constexpr auto none                                      = VK_ACCESS_NONE;
        inline constexpr auto transform_feedback_write_ext              = VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT;
        inline constexpr auto transform_feedback_counter_read_ext       = VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT;
        inline constexpr auto transform_feedback_counter_write_ext      = VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT;
        inline constexpr auto conditional_rendering_read_ext            = VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT;
        inline constexpr auto color_attachment_read_noncoherent_ext     = VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT;
        inline constexpr auto acceleration_structure_read_khr           = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;
        inline constexpr auto acceleration_structure_write_khr          = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
        inline constexpr auto fragment_density_map_read_ext             = VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT;
        inline constexpr auto fragment_shading_rate_attachment_read_khr = VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR;
        inline constexpr auto command_preprocess_read_nv                = VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV;
        inline constexpr auto command_preprocess_write_nv               = VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV;
        inline constexpr auto shading_rate_image_read_nv                = VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV;
        inline constexpr auto acceleration_structure_read_nv            = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV;
        inline constexpr auto acceleration_structure_write_nv           = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV;
        inline constexpr auto none_khr                                  = VK_ACCESS_NONE_KHR;
    } // namespace access_flags

    namespace image_aspect_flags
    {
        using enum_t = VkImageAspectFlagBits;

        inline constexpr auto color              = VK_IMAGE_ASPECT_COLOR_BIT;
        inline constexpr auto depth              = VK_IMAGE_ASPECT_DEPTH_BIT;
        inline constexpr auto stencil            = VK_IMAGE_ASPECT_STENCIL_BIT;
        inline constexpr auto metadata           = VK_IMAGE_ASPECT_METADATA_BIT;
        inline constexpr auto plane_0            = VK_IMAGE_ASPECT_PLANE_0_BIT;
        inline constexpr auto plane_1            = VK_IMAGE_ASPECT_PLANE_1_BIT;
        inline constexpr auto plane_2            = VK_IMAGE_ASPECT_PLANE_2_BIT;
        inline constexpr auto none               = VK_IMAGE_ASPECT_NONE;
        inline constexpr auto memory_plane_0_ext = VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT;
        inline constexpr auto memory_plane_1_ext = VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT;
        inline constexpr auto memory_plane_2_ext = VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT;
        inline constexpr auto memory_plane_3_ext = VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT;
        inline constexpr auto plane_0_khr        = VK_IMAGE_ASPECT_PLANE_0_BIT_KHR;
        inline constexpr auto plane_1_khr        = VK_IMAGE_ASPECT_PLANE_1_BIT_KHR;
        inline constexpr auto plane_2_khr        = VK_IMAGE_ASPECT_PLANE_2_BIT_KHR;
        inline constexpr auto none_khr           = VK_IMAGE_ASPECT_NONE_KHR;
    } // namespace image_aspect_flags

    namespace pipeline_stage_flags
    {
        using enum_t = VkPipelineStageFlagBits;

        inline constexpr auto top_of_pipe                          = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        inline constexpr auto draw_indirect                        = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
        inline constexpr auto vertex_input                         = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
        inline constexpr auto vertex_shader                        = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
        inline constexpr auto tessellation_control_shader          = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
        inline constexpr auto tessellation_evaluation_shader       = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
        inline constexpr auto geometry_shader                      = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
        inline constexpr auto fragment_shader                      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        inline constexpr auto early_fragment_tests                 = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        inline constexpr auto late_fragment_tests                  = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        inline constexpr auto color_attachment_output              = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        inline constexpr auto compute_shader                       = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        inline constexpr auto transfer                             = VK_PIPELINE_STAGE_TRANSFER_BIT;
        inline constexpr auto bottom_of_pipe                       = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        inline constexpr auto host                                 = VK_PIPELINE_STAGE_HOST_BIT;
        inline constexpr auto all_graphics                         = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
        inline constexpr auto all_commands                         = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        inline constexpr auto none                                 = VK_PIPELINE_STAGE_NONE;
        inline constexpr auto transform_feedback_ext               = VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT;
        inline constexpr auto conditional_rendering_ext            = VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT;
        inline constexpr auto acceleration_structure_build_khr     = VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
        inline constexpr auto ray_tracing_shader_khr               = VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR;
        inline constexpr auto fragment_density_process_ext         = VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT;
        inline constexpr auto fragment_shading_rate_attachment_khr = VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
        inline constexpr auto command_preprocess_nv                = VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV;
        inline constexpr auto task_shader_ext                      = VK_PIPELINE_STAGE_TASK_SHADER_BIT_EXT;
        inline constexpr auto mesh_shader_ext                      = VK_PIPELINE_STAGE_MESH_SHADER_BIT_EXT;
        inline constexpr auto shading_rate_image_nv                = VK_PIPELINE_STAGE_SHADING_RATE_IMAGE_BIT_NV;
        inline constexpr auto ray_tracing_shader_nv                = VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV;
        inline constexpr auto acceleration_structure_build_nv      = VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV;
        inline constexpr auto task_shader_nv                       = VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV;
        inline constexpr auto mesh_shader_nv                       = VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV;
        inline constexpr auto none_khr                             = VK_PIPELINE_STAGE_NONE_KHR;
    } // namespace pipeline_stage_flags

    namespace vkres
    {
        using enum_t = VkResult;

        template <enum_t res>
        inline constexpr std::string_view repr = "unknown error";

#define define_vkres(orbvar, vkvar)       \
    inline constexpr auto orbvar = vkvar; \
    template <>                           \
    inline constexpr std::string_view repr<orbvar> = #orbvar;

        define_vkres(ok, VK_SUCCESS);
        define_vkres(not_ready, VK_NOT_READY);
        define_vkres(timeout, VK_TIMEOUT);
        define_vkres(event_set, VK_EVENT_SET);
        define_vkres(event_reset, VK_EVENT_RESET);
        define_vkres(incomplete, VK_INCOMPLETE);
        define_vkres(err_out_of_host_memory, VK_ERROR_OUT_OF_HOST_MEMORY);
        define_vkres(err_out_of_device_memory, VK_ERROR_OUT_OF_DEVICE_MEMORY);
        define_vkres(err_initialization_failed, VK_ERROR_INITIALIZATION_FAILED);
        define_vkres(err_device_lost, VK_ERROR_DEVICE_LOST);
        define_vkres(err_memory_map_failed, VK_ERROR_MEMORY_MAP_FAILED);
        define_vkres(err_layer_not_present, VK_ERROR_LAYER_NOT_PRESENT);
        define_vkres(err_extension_not_present, VK_ERROR_EXTENSION_NOT_PRESENT);
        define_vkres(err_feature_not_present, VK_ERROR_FEATURE_NOT_PRESENT);
        define_vkres(err_incompatible_driver, VK_ERROR_INCOMPATIBLE_DRIVER);
        define_vkres(err_too_many_objects, VK_ERROR_TOO_MANY_OBJECTS);
        define_vkres(err_format_not_supported, VK_ERROR_FORMAT_NOT_SUPPORTED);
        define_vkres(err_fragmented_pool, VK_ERROR_FRAGMENTED_POOL);
        define_vkres(err_unknown, VK_ERROR_UNKNOWN);
        define_vkres(err_out_of_pool_memory, VK_ERROR_OUT_OF_POOL_MEMORY);
        define_vkres(err_invalid_external_handle, VK_ERROR_INVALID_EXTERNAL_HANDLE);
        define_vkres(err_fragmentation, VK_ERROR_FRAGMENTATION);
        define_vkres(err_invalid_opaque_capture_address, VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
        define_vkres(pipeline_compile_required, VK_PIPELINE_COMPILE_REQUIRED);
        define_vkres(err_surface_lost_khr, VK_ERROR_SURFACE_LOST_KHR);
        define_vkres(err_native_window_in_use_khr, VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
        define_vkres(suboptimal_khr, VK_SUBOPTIMAL_KHR);
        define_vkres(err_out_of_date_khr, VK_ERROR_OUT_OF_DATE_KHR);
        define_vkres(err_incompatible_display_khr, VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
        define_vkres(err_validation_failed_ext, VK_ERROR_VALIDATION_FAILED_EXT);
        define_vkres(err_invalid_shader_nv, VK_ERROR_INVALID_SHADER_NV);
        define_vkres(err_image_usage_not_supported_khr, VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR);
        define_vkres(err_video_picture_layout_not_supported_khr, VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR);
        define_vkres(err_video_profile_operation_not_supported_khr, VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR);
        define_vkres(err_video_profile_format_not_supported_khr, VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR);
        define_vkres(err_video_profile_codec_not_supported_khr, VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR);
        define_vkres(err_video_std_version_not_supported_khr, VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR);
        define_vkres(err_invalid_drm_format_modifier_plane_layout_ext, VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
        define_vkres(err_not_permitted_khr, VK_ERROR_NOT_PERMITTED_KHR);
        define_vkres(err_full_screen_exclusive_mode_lost_ext, VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
        define_vkres(thread_idle_khr, VK_THREAD_IDLE_KHR);
        define_vkres(thread_done_khr, VK_THREAD_DONE_KHR);
        define_vkres(operation_deferred_khr, VK_OPERATION_DEFERRED_KHR);
        define_vkres(operation_not_deferred_khr, VK_OPERATION_NOT_DEFERRED_KHR);
        define_vkres(err_compression_exhausted_ext, VK_ERROR_COMPRESSION_EXHAUSTED_EXT);

        inline constexpr auto get_repr(vkres::enum_t res) -> std::string_view
        {
            switch (res)
            {
            case vkres::ok: return repr<vkres::ok>;
            case vkres::not_ready: return repr<vkres::not_ready>;
            case vkres::timeout: return repr<vkres::timeout>;
            case vkres::event_set: return repr<vkres::event_set>;
            case vkres::event_reset: return repr<vkres::event_reset>;
            case vkres::incomplete: return repr<vkres::incomplete>;
            case vkres::err_out_of_host_memory: return repr<vkres::err_out_of_host_memory>;
            case vkres::err_out_of_device_memory: return repr<vkres::err_out_of_device_memory>;
            case vkres::err_initialization_failed: return repr<vkres::err_initialization_failed>;
            case vkres::err_device_lost: return repr<vkres::err_device_lost>;
            case vkres::err_memory_map_failed: return repr<vkres::err_memory_map_failed>;
            case vkres::err_layer_not_present: return repr<vkres::err_layer_not_present>;
            case vkres::err_extension_not_present: return repr<vkres::err_extension_not_present>;
            case vkres::err_feature_not_present: return repr<vkres::err_feature_not_present>;
            case vkres::err_incompatible_driver: return repr<vkres::err_incompatible_driver>;
            case vkres::err_too_many_objects: return repr<vkres::err_too_many_objects>;
            case vkres::err_format_not_supported: return repr<vkres::err_format_not_supported>;
            case vkres::err_fragmented_pool: return repr<vkres::err_fragmented_pool>;
            case vkres::err_unknown: return repr<vkres::err_unknown>;
            case vkres::err_out_of_pool_memory: return repr<vkres::err_out_of_pool_memory>;
            case vkres::err_invalid_external_handle: return repr<vkres::err_invalid_external_handle>;
            case vkres::err_fragmentation: return repr<vkres::err_fragmentation>;
            case vkres::err_invalid_opaque_capture_address: return repr<vkres::err_invalid_opaque_capture_address>;
            case vkres::pipeline_compile_required: return repr<vkres::pipeline_compile_required>;
            case vkres::err_surface_lost_khr: return repr<vkres::err_surface_lost_khr>;
            case vkres::err_native_window_in_use_khr: return repr<vkres::err_native_window_in_use_khr>;
            case vkres::suboptimal_khr: return repr<vkres::suboptimal_khr>;
            case vkres::err_out_of_date_khr: return repr<vkres::err_out_of_date_khr>;
            case vkres::err_incompatible_display_khr: return repr<vkres::err_incompatible_display_khr>;
            case vkres::err_validation_failed_ext: return repr<vkres::err_validation_failed_ext>;
            case vkres::err_invalid_shader_nv: return repr<vkres::err_invalid_shader_nv>;
            case vkres::err_image_usage_not_supported_khr: return repr<vkres::err_image_usage_not_supported_khr>;
            case vkres::err_video_picture_layout_not_supported_khr: return repr<vkres::err_video_picture_layout_not_supported_khr>;
            case vkres::err_video_profile_operation_not_supported_khr: return repr<vkres::err_video_profile_operation_not_supported_khr>;
            case vkres::err_video_profile_format_not_supported_khr: return repr<vkres::err_video_profile_format_not_supported_khr>;
            case vkres::err_video_profile_codec_not_supported_khr: return repr<vkres::err_video_profile_codec_not_supported_khr>;
            case vkres::err_video_std_version_not_supported_khr: return repr<vkres::err_video_std_version_not_supported_khr>;
            case vkres::err_invalid_drm_format_modifier_plane_layout_ext: return repr<vkres::err_invalid_drm_format_modifier_plane_layout_ext>;
            case vkres::err_not_permitted_khr: return repr<vkres::err_not_permitted_khr>;
            case vkres::err_full_screen_exclusive_mode_lost_ext: return repr<vkres::err_full_screen_exclusive_mode_lost_ext>;
            case vkres::thread_idle_khr: return repr<vkres::thread_idle_khr>;
            case vkres::thread_done_khr: return repr<vkres::thread_done_khr>;
            case vkres::operation_deferred_khr: return repr<vkres::operation_deferred_khr>;
            case vkres::operation_not_deferred_khr: return repr<vkres::operation_not_deferred_khr>;
            case vkres::err_compression_exhausted_ext: return repr<vkres::err_compression_exhausted_ext>;
            default: return "unknown error";
            }
        }

    } // namespace vkres

    namespace formats
    {
        using enum_t = VkFormat;

        inline constexpr auto undefined                  = VK_FORMAT_UNDEFINED;
        inline constexpr auto r4g4_unorm_pack8           = VK_FORMAT_R4G4_UNORM_PACK8;
        inline constexpr auto r4g4b4a4_unorm_pack16      = VK_FORMAT_R4G4B4A4_UNORM_PACK16;
        inline constexpr auto b4g4r4a4_unorm_pack16      = VK_FORMAT_B4G4R4A4_UNORM_PACK16;
        inline constexpr auto r5g6b5_unorm_pack16        = VK_FORMAT_R5G6B5_UNORM_PACK16;
        inline constexpr auto b5g6r5_unorm_pack16        = VK_FORMAT_B5G6R5_UNORM_PACK16;
        inline constexpr auto r5g5b5a1_unorm_pack16      = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
        inline constexpr auto b5g5r5a1_unorm_pack16      = VK_FORMAT_B5G5R5A1_UNORM_PACK16;
        inline constexpr auto a1r5g5b5_unorm_pack16      = VK_FORMAT_A1R5G5B5_UNORM_PACK16;
        inline constexpr auto r8_unorm                   = VK_FORMAT_R8_UNORM;
        inline constexpr auto r8_snorm                   = VK_FORMAT_R8_SNORM;
        inline constexpr auto r8_uscaled                 = VK_FORMAT_R8_USCALED;
        inline constexpr auto r8_sscaled                 = VK_FORMAT_R8_SSCALED;
        inline constexpr auto r8_uint                    = VK_FORMAT_R8_UINT;
        inline constexpr auto r8_sint                    = VK_FORMAT_R8_SINT;
        inline constexpr auto r8_srgb                    = VK_FORMAT_R8_SRGB;
        inline constexpr auto r8g8_unorm                 = VK_FORMAT_R8G8_UNORM;
        inline constexpr auto r8g8_snorm                 = VK_FORMAT_R8G8_SNORM;
        inline constexpr auto r8g8_uscaled               = VK_FORMAT_R8G8_USCALED;
        inline constexpr auto r8g8_sscaled               = VK_FORMAT_R8G8_SSCALED;
        inline constexpr auto r8g8_uint                  = VK_FORMAT_R8G8_UINT;
        inline constexpr auto r8g8_sint                  = VK_FORMAT_R8G8_SINT;
        inline constexpr auto r8g8_srgb                  = VK_FORMAT_R8G8_SRGB;
        inline constexpr auto r8g8b8_unorm               = VK_FORMAT_R8G8B8_UNORM;
        inline constexpr auto r8g8b8_snorm               = VK_FORMAT_R8G8B8_SNORM;
        inline constexpr auto r8g8b8_uscaled             = VK_FORMAT_R8G8B8_USCALED;
        inline constexpr auto r8g8b8_sscaled             = VK_FORMAT_R8G8B8_SSCALED;
        inline constexpr auto r8g8b8_uint                = VK_FORMAT_R8G8B8_UINT;
        inline constexpr auto r8g8b8_sint                = VK_FORMAT_R8G8B8_SINT;
        inline constexpr auto r8g8b8_srgb                = VK_FORMAT_R8G8B8_SRGB;
        inline constexpr auto b8g8r8_unorm               = VK_FORMAT_B8G8R8_UNORM;
        inline constexpr auto b8g8r8_snorm               = VK_FORMAT_B8G8R8_SNORM;
        inline constexpr auto b8g8r8_uscaled             = VK_FORMAT_B8G8R8_USCALED;
        inline constexpr auto b8g8r8_sscaled             = VK_FORMAT_B8G8R8_SSCALED;
        inline constexpr auto b8g8r8_uint                = VK_FORMAT_B8G8R8_UINT;
        inline constexpr auto b8g8r8_sint                = VK_FORMAT_B8G8R8_SINT;
        inline constexpr auto b8g8r8_srgb                = VK_FORMAT_B8G8R8_SRGB;
        inline constexpr auto r8g8b8a8_unorm             = VK_FORMAT_R8G8B8A8_UNORM;
        inline constexpr auto r8g8b8a8_snorm             = VK_FORMAT_R8G8B8A8_SNORM;
        inline constexpr auto r8g8b8a8_uscaled           = VK_FORMAT_R8G8B8A8_USCALED;
        inline constexpr auto r8g8b8a8_sscaled           = VK_FORMAT_R8G8B8A8_SSCALED;
        inline constexpr auto r8g8b8a8_uint              = VK_FORMAT_R8G8B8A8_UINT;
        inline constexpr auto r8g8b8a8_sint              = VK_FORMAT_R8G8B8A8_SINT;
        inline constexpr auto r8g8b8a8_srgb              = VK_FORMAT_R8G8B8A8_SRGB;
        inline constexpr auto b8g8r8a8_unorm             = VK_FORMAT_B8G8R8A8_UNORM;
        inline constexpr auto b8g8r8a8_snorm             = VK_FORMAT_B8G8R8A8_SNORM;
        inline constexpr auto b8g8r8a8_uscaled           = VK_FORMAT_B8G8R8A8_USCALED;
        inline constexpr auto b8g8r8a8_sscaled           = VK_FORMAT_B8G8R8A8_SSCALED;
        inline constexpr auto b8g8r8a8_uint              = VK_FORMAT_B8G8R8A8_UINT;
        inline constexpr auto b8g8r8a8_sint              = VK_FORMAT_B8G8R8A8_SINT;
        inline constexpr auto b8g8r8a8_srgb              = VK_FORMAT_B8G8R8A8_SRGB;
        inline constexpr auto a8b8g8r8_unorm_pack32      = VK_FORMAT_A8B8G8R8_UNORM_PACK32;
        inline constexpr auto a8b8g8r8_snorm_pack32      = VK_FORMAT_A8B8G8R8_SNORM_PACK32;
        inline constexpr auto a8b8g8r8_uscaled_pack32    = VK_FORMAT_A8B8G8R8_USCALED_PACK32;
        inline constexpr auto a8b8g8r8_sscaled_pack32    = VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
        inline constexpr auto a8b8g8r8_uint_pack32       = VK_FORMAT_A8B8G8R8_UINT_PACK32;
        inline constexpr auto a8b8g8r8_sint_pack32       = VK_FORMAT_A8B8G8R8_SINT_PACK32;
        inline constexpr auto a8b8g8r8_srgb_pack32       = VK_FORMAT_A8B8G8R8_SRGB_PACK32;
        inline constexpr auto a2r10g10b10_unorm_pack32   = VK_FORMAT_A2R10G10B10_UNORM_PACK32;
        inline constexpr auto a2r10g10b10_snorm_pack32   = VK_FORMAT_A2R10G10B10_SNORM_PACK32;
        inline constexpr auto a2r10g10b10_uscaled_pack32 = VK_FORMAT_A2R10G10B10_USCALED_PACK32;
        inline constexpr auto a2r10g10b10_sscaled_pack32 = VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
        inline constexpr auto a2r10g10b10_uint_pack32    = VK_FORMAT_A2R10G10B10_UINT_PACK32;
        inline constexpr auto a2r10g10b10_sint_pack32    = VK_FORMAT_A2R10G10B10_SINT_PACK32;
        inline constexpr auto a2b10g10r10_unorm_pack32   = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        inline constexpr auto a2b10g10r10_snorm_pack32   = VK_FORMAT_A2B10G10R10_SNORM_PACK32;
        inline constexpr auto a2b10g10r10_uscaled_pack32 = VK_FORMAT_A2B10G10R10_USCALED_PACK32;
        inline constexpr auto a2b10g10r10_sscaled_pack32 = VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
        inline constexpr auto a2b10g10r10_uint_pack32    = VK_FORMAT_A2B10G10R10_UINT_PACK32;
        inline constexpr auto a2b10g10r10_sint_pack32    = VK_FORMAT_A2B10G10R10_SINT_PACK32;
        inline constexpr auto r16_unorm                  = VK_FORMAT_R16_UNORM;
        inline constexpr auto r16_snorm                  = VK_FORMAT_R16_SNORM;
        inline constexpr auto r16_uscaled                = VK_FORMAT_R16_USCALED;
        inline constexpr auto r16_sscaled                = VK_FORMAT_R16_SSCALED;
        inline constexpr auto r16_uint                   = VK_FORMAT_R16_UINT;
        inline constexpr auto r16_sint                   = VK_FORMAT_R16_SINT;
        inline constexpr auto r16_sfloat                 = VK_FORMAT_R16_SFLOAT;
        inline constexpr auto r16g16_unorm               = VK_FORMAT_R16G16_UNORM;
        inline constexpr auto r16g16_snorm               = VK_FORMAT_R16G16_SNORM;
        inline constexpr auto r16g16_uscaled             = VK_FORMAT_R16G16_USCALED;
        inline constexpr auto r16g16_sscaled             = VK_FORMAT_R16G16_SSCALED;
        inline constexpr auto r16g16_uint                = VK_FORMAT_R16G16_UINT;
        inline constexpr auto r16g16_sint                = VK_FORMAT_R16G16_SINT;
        inline constexpr auto r16g16_sfloat              = VK_FORMAT_R16G16_SFLOAT;
        inline constexpr auto r16g16b16_unorm            = VK_FORMAT_R16G16B16_UNORM;
        inline constexpr auto r16g16b16_snorm            = VK_FORMAT_R16G16B16_SNORM;
        inline constexpr auto r16g16b16_uscaled          = VK_FORMAT_R16G16B16_USCALED;
        inline constexpr auto r16g16b16_sscaled          = VK_FORMAT_R16G16B16_SSCALED;
        inline constexpr auto r16g16b16_uint             = VK_FORMAT_R16G16B16_UINT;
        inline constexpr auto r16g16b16_sint             = VK_FORMAT_R16G16B16_SINT;
        inline constexpr auto r16g16b16_sfloat           = VK_FORMAT_R16G16B16_SFLOAT;
        inline constexpr auto r16g16b16a16_unorm         = VK_FORMAT_R16G16B16A16_UNORM;
        inline constexpr auto r16g16b16a16_snorm         = VK_FORMAT_R16G16B16A16_SNORM;
        inline constexpr auto r16g16b16a16_uscaled       = VK_FORMAT_R16G16B16A16_USCALED;
        inline constexpr auto r16g16b16a16_sscaled       = VK_FORMAT_R16G16B16A16_SSCALED;
        inline constexpr auto r16g16b16a16_uint          = VK_FORMAT_R16G16B16A16_UINT;
        inline constexpr auto r16g16b16a16_sint          = VK_FORMAT_R16G16B16A16_SINT;
        inline constexpr auto r16g16b16a16_sfloat        = VK_FORMAT_R16G16B16A16_SFLOAT;
        inline constexpr auto r32_uint                   = VK_FORMAT_R32_UINT;
        inline constexpr auto r32_sint                   = VK_FORMAT_R32_SINT;
        inline constexpr auto r32_sfloat                 = VK_FORMAT_R32_SFLOAT;
        inline constexpr auto r32g32_uint                = VK_FORMAT_R32G32_UINT;
        inline constexpr auto r32g32_sint                = VK_FORMAT_R32G32_SINT;
        inline constexpr auto r32g32_sfloat              = VK_FORMAT_R32G32_SFLOAT;
        inline constexpr auto r32g32b32_uint             = VK_FORMAT_R32G32B32_UINT;
        inline constexpr auto r32g32b32_sint             = VK_FORMAT_R32G32B32_SINT;
        inline constexpr auto r32g32b32_sfloat           = VK_FORMAT_R32G32B32_SFLOAT;
        inline constexpr auto r32g32b32a32_uint          = VK_FORMAT_R32G32B32A32_UINT;
        inline constexpr auto r32g32b32a32_sint          = VK_FORMAT_R32G32B32A32_SINT;
        inline constexpr auto r32g32b32a32_sfloat        = VK_FORMAT_R32G32B32A32_SFLOAT;
        inline constexpr auto r64_uint                   = VK_FORMAT_R64_UINT;
        inline constexpr auto r64_sint                   = VK_FORMAT_R64_SINT;
        inline constexpr auto r64_sfloat                 = VK_FORMAT_R64_SFLOAT;
        inline constexpr auto r64g64_uint                = VK_FORMAT_R64G64_UINT;
        inline constexpr auto r64g64_sint                = VK_FORMAT_R64G64_SINT;
        inline constexpr auto r64g64_sfloat              = VK_FORMAT_R64G64_SFLOAT;
        inline constexpr auto r64g64b64_uint             = VK_FORMAT_R64G64B64_UINT;
        inline constexpr auto r64g64b64_sint             = VK_FORMAT_R64G64B64_SINT;
        inline constexpr auto r64g64b64_sfloat           = VK_FORMAT_R64G64B64_SFLOAT;
        inline constexpr auto r64g64b64a64_uint          = VK_FORMAT_R64G64B64A64_UINT;
        inline constexpr auto r64g64b64a64_sint          = VK_FORMAT_R64G64B64A64_SINT;
        inline constexpr auto r64g64b64a64_sfloat        = VK_FORMAT_R64G64B64A64_SFLOAT;
        inline constexpr auto b10g11r11_ufloat_pack32    = VK_FORMAT_B10G11R11_UFLOAT_PACK32;
        inline constexpr auto e5b9g9r9_ufloat_pack32     = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
        inline constexpr auto d16_unorm                  = VK_FORMAT_D16_UNORM;
        inline constexpr auto x8_d24_unorm_pack32        = VK_FORMAT_X8_D24_UNORM_PACK32;
        inline constexpr auto d32_sfloat                 = VK_FORMAT_D32_SFLOAT;
        inline constexpr auto s8_uint                    = VK_FORMAT_S8_UINT;
        inline constexpr auto d16_unorm_s8_uint          = VK_FORMAT_D16_UNORM_S8_UINT;
        inline constexpr auto d24_unorm_s8_uint          = VK_FORMAT_D24_UNORM_S8_UINT;
        inline constexpr auto d32_sfloat_s8_uint         = VK_FORMAT_D32_SFLOAT_S8_UINT;
        inline constexpr auto bc1_rgb_unorm_block        = VK_FORMAT_BC1_RGB_UNORM_BLOCK;
        inline constexpr auto bc1_rgb_srgb_block         = VK_FORMAT_BC1_RGB_SRGB_BLOCK;
        inline constexpr auto bc1_rgba_unorm_block       = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
        inline constexpr auto bc1_rgba_srgb_block        = VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
        inline constexpr auto bc2_unorm_block            = VK_FORMAT_BC2_UNORM_BLOCK;
        inline constexpr auto bc2_srgb_block             = VK_FORMAT_BC2_SRGB_BLOCK;
        inline constexpr auto bc3_unorm_block            = VK_FORMAT_BC3_UNORM_BLOCK;
        inline constexpr auto bc3_srgb_block             = VK_FORMAT_BC3_SRGB_BLOCK;
        inline constexpr auto bc4_unorm_block            = VK_FORMAT_BC4_UNORM_BLOCK;
        inline constexpr auto bc4_snorm_block            = VK_FORMAT_BC4_SNORM_BLOCK;
        inline constexpr auto bc5_unorm_block            = VK_FORMAT_BC5_UNORM_BLOCK;
        inline constexpr auto bc5_snorm_block            = VK_FORMAT_BC5_SNORM_BLOCK;
        inline constexpr auto bc6h_ufloat_block          = VK_FORMAT_BC6H_UFLOAT_BLOCK;
        inline constexpr auto bc6h_sfloat_block          = VK_FORMAT_BC6H_SFLOAT_BLOCK;
        inline constexpr auto bc7_unorm_block            = VK_FORMAT_BC7_UNORM_BLOCK;
        inline constexpr auto bc7_srgb_block             = VK_FORMAT_BC7_SRGB_BLOCK;
        inline constexpr auto etc2_r8g8b8_unorm_block    = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
        inline constexpr auto etc2_r8g8b8_srgb_block     = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
        inline constexpr auto etc2_r8g8b8a1_unorm_block  = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
        inline constexpr auto etc2_r8g8b8a1_srgb_block   = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
        inline constexpr auto etc2_r8g8b8a8_unorm_block  = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
        inline constexpr auto etc2_r8g8b8a8_srgb_block   = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
        inline constexpr auto eac_r11_unorm_block        = VK_FORMAT_EAC_R11_UNORM_BLOCK;
        inline constexpr auto eac_r11_snorm_block        = VK_FORMAT_EAC_R11_SNORM_BLOCK;
        inline constexpr auto eac_r11g11_unorm_block     = VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
        inline constexpr auto eac_r11g11_snorm_block     = VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
        inline constexpr auto astc_4x4_unorm_block       = VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
        inline constexpr auto astc_4x4_srgb_block        = VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
        inline constexpr auto astc_5x4_unorm_block       = VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
        inline constexpr auto astc_5x4_srgb_block        = VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
        inline constexpr auto astc_5x5_unorm_block       = VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
        inline constexpr auto astc_5x5_srgb_block        = VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
        inline constexpr auto astc_6x5_unorm_block       = VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
        inline constexpr auto astc_6x5_srgb_block        = VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
        inline constexpr auto astc_6x6_unorm_block       = VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
        inline constexpr auto astc_6x6_srgb_block        = VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
        inline constexpr auto astc_8x5_unorm_block       = VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
        inline constexpr auto astc_8x5_srgb_block        = VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
        inline constexpr auto astc_8x6_unorm_block       = VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
        inline constexpr auto astc_8x6_srgb_block        = VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
        inline constexpr auto astc_8x8_unorm_block       = VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
        inline constexpr auto astc_8x8_srgb_block        = VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
        inline constexpr auto astc_10x5_unorm_block      = VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
        inline constexpr auto astc_10x5_srgb_block       = VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
        inline constexpr auto astc_10x6_unorm_block      = VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
        inline constexpr auto astc_10x6_srgb_block       = VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
        inline constexpr auto astc_10x8_unorm_block      = VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
        inline constexpr auto astc_10x8_srgb_block       = VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
        inline constexpr auto astc_10x10_unorm_block     = VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
        inline constexpr auto astc_10x10_srgb_block      = VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
        inline constexpr auto astc_12x10_unorm_block     = VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
        inline constexpr auto astc_12x10_srgb_block      = VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
        inline constexpr auto astc_12x12_unorm_block     = VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
        inline constexpr auto astc_12x12_srgb_block      = VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
        inline constexpr auto g8b8g8r8_422_unorm         = VK_FORMAT_G8B8G8R8_422_UNORM;
        inline constexpr auto b8g8r8g8_422_unorm         = VK_FORMAT_B8G8R8G8_422_UNORM;
        inline constexpr auto g8_b8_r8_3plane_420_unorm  = VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM;
        inline constexpr auto g8_b8r8_2plane_420_unorm   = VK_FORMAT_G8_B8R8_2PLANE_420_UNORM;
        inline constexpr auto g8_b8_r8_3plane_422_unorm  = VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM;
        inline constexpr auto g8_b8r8_2plane_422_unorm   = VK_FORMAT_G8_B8R8_2PLANE_422_UNORM;
        inline constexpr auto g8_b8_r8_3plane_444_unorm  = VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM;
        inline constexpr auto r10x6_unorm_pack16         = VK_FORMAT_R10X6_UNORM_PACK16;
        inline constexpr auto r10x6g10x6_unorm_2pack16   = VK_FORMAT_R10X6G10X6_UNORM_2PACK16;
        inline constexpr auto r12x4_unorm_pack16         = VK_FORMAT_R12X4_UNORM_PACK16;
        inline constexpr auto r12x4g12x4_unorm_2pack16   = VK_FORMAT_R12X4G12X4_UNORM_2PACK16;

        inline constexpr auto r10x6g10x6b10x6a10x6_unorm_4pack16         = VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16;
        inline constexpr auto g10x6b10x6g10x6r10x6_422_unorm_4pack16     = VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
        inline constexpr auto b10x6g10x6r10x6g10x6_422_unorm_4pack16     = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_420_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
        inline constexpr auto g10x6_b10x6r10x6_2plane_420_unorm_3pack16  = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_422_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
        inline constexpr auto g10x6_b10x6r10x6_2plane_422_unorm_3pack16  = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_444_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
        inline constexpr auto r12x4g12x4b12x4a12x4_unorm_4pack16         = VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16;
        inline constexpr auto g12x4b12x4g12x4r12x4_422_unorm_4pack16     = VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
        inline constexpr auto b12x4g12x4r12x4g12x4_422_unorm_4pack16     = VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_420_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4r12x4_2plane_420_unorm_3pack16  = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_422_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4r12x4_2plane_422_unorm_3pack16  = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_444_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;

        inline constexpr auto g16b16g16r16_422_unorm        = VK_FORMAT_G16B16G16R16_422_UNORM;
        inline constexpr auto b16g16r16g16_422_unorm        = VK_FORMAT_B16G16R16G16_422_UNORM;
        inline constexpr auto g16_b16_r16_3plane_420_unorm  = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM;
        inline constexpr auto g16_b16r16_2plane_420_unorm   = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM;
        inline constexpr auto g16_b16_r16_3plane_422_unorm  = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM;
        inline constexpr auto g16_b16r16_2plane_422_unorm   = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM;
        inline constexpr auto g16_b16_r16_3plane_444_unorm  = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM;
        inline constexpr auto g8_b8r8_2plane_444_unorm      = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM;
        inline constexpr auto g16_b16r16_2plane_444_unorm   = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM;
        inline constexpr auto a4r4g4b4_unorm_pack16         = VK_FORMAT_A4R4G4B4_UNORM_PACK16;
        inline constexpr auto a4b4g4r4_unorm_pack16         = VK_FORMAT_A4B4G4R4_UNORM_PACK16;
        inline constexpr auto astc_4x4_sfloat_block         = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK;
        inline constexpr auto astc_5x4_sfloat_block         = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK;
        inline constexpr auto astc_5x5_sfloat_block         = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK;
        inline constexpr auto astc_6x5_sfloat_block         = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK;
        inline constexpr auto astc_6x6_sfloat_block         = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK;
        inline constexpr auto astc_8x5_sfloat_block         = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK;
        inline constexpr auto astc_8x6_sfloat_block         = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK;
        inline constexpr auto astc_8x8_sfloat_block         = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK;
        inline constexpr auto astc_10x5_sfloat_block        = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK;
        inline constexpr auto astc_10x6_sfloat_block        = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK;
        inline constexpr auto astc_10x8_sfloat_block        = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK;
        inline constexpr auto astc_10x10_sfloat_block       = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK;
        inline constexpr auto astc_12x10_sfloat_block       = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK;
        inline constexpr auto astc_12x12_sfloat_block       = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK;
        inline constexpr auto pvrtc1_2bpp_unorm_block_img   = VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;
        inline constexpr auto pvrtc1_4bpp_unorm_block_img   = VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;
        inline constexpr auto pvrtc2_2bpp_unorm_block_img   = VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG;
        inline constexpr auto pvrtc2_4bpp_unorm_block_img   = VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG;
        inline constexpr auto pvrtc1_2bpp_srgb_block_img    = VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG;
        inline constexpr auto pvrtc1_4bpp_srgb_block_img    = VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG;
        inline constexpr auto pvrtc2_2bpp_srgb_block_img    = VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG;
        inline constexpr auto pvrtc2_4bpp_srgb_block_img    = VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG;
        inline constexpr auto r16g16_s10_5_nv               = VK_FORMAT_R16G16_S10_5_NV;
        inline constexpr auto astc_4x4_sfloat_block_ext     = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_5x4_sfloat_block_ext     = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_5x5_sfloat_block_ext     = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_6x5_sfloat_block_ext     = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_6x6_sfloat_block_ext     = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_8x5_sfloat_block_ext     = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_8x6_sfloat_block_ext     = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_8x8_sfloat_block_ext     = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_10x5_sfloat_block_ext    = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_10x6_sfloat_block_ext    = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_10x8_sfloat_block_ext    = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_10x10_sfloat_block_ext   = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_12x10_sfloat_block_ext   = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT;
        inline constexpr auto astc_12x12_sfloat_block_ext   = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT;
        inline constexpr auto g8b8g8r8_422_unorm_khr        = VK_FORMAT_G8B8G8R8_422_UNORM_KHR;
        inline constexpr auto b8g8r8g8_422_unorm_khr        = VK_FORMAT_B8G8R8G8_422_UNORM_KHR;
        inline constexpr auto g8_b8_r8_3plane_420_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR;
        inline constexpr auto g8_b8r8_2plane_420_unorm_khr  = VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR;
        inline constexpr auto g8_b8_r8_3plane_422_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR;
        inline constexpr auto g8_b8r8_2plane_422_unorm_khr  = VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR;
        inline constexpr auto g8_b8_r8_3plane_444_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR;
        inline constexpr auto r10x6_unorm_pack16_khr        = VK_FORMAT_R10X6_UNORM_PACK16_KHR;
        inline constexpr auto r10x6g10x6_unorm_2pack16_khr  = VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR;

        inline constexpr auto g10x6_b10x6r10x6_2plane_444_unorm_3pack16      = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4r12x4_2plane_444_unorm_3pack16      = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16;
        inline constexpr auto r10x6g10x6b10x6a10x6_unorm_4pack16_khr         = VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR;
        inline constexpr auto g10x6b10x6g10x6r10x6_422_unorm_4pack16_khr     = VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR;
        inline constexpr auto b10x6g10x6r10x6g10x6_422_unorm_4pack16_khr     = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_420_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6r10x6_2plane_420_unorm_3pack16_khr  = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_422_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6r10x6_2plane_422_unorm_3pack16_khr  = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_444_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR;
        inline constexpr auto r12x4_unorm_pack16_khr                         = VK_FORMAT_R12X4_UNORM_PACK16_KHR;
        inline constexpr auto r12x4g12x4_unorm_2pack16_khr                   = VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR;
        inline constexpr auto r12x4g12x4b12x4a12x4_unorm_4pack16_khr         = VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR;
        inline constexpr auto g12x4b12x4g12x4r12x4_422_unorm_4pack16_khr     = VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR;
        inline constexpr auto b12x4g12x4r12x4g12x4_422_unorm_4pack16_khr     = VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_420_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR;
        inline constexpr auto g12x4_b12x4r12x4_2plane_420_unorm_3pack16_khr  = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_422_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR;
        inline constexpr auto g12x4_b12x4r12x4_2plane_422_unorm_3pack16_khr  = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_444_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6r10x6_2plane_444_unorm_3pack16_ext  = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT;
        inline constexpr auto g12x4_b12x4r12x4_2plane_444_unorm_3pack16_ext  = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT;

        inline constexpr auto g16b16g16r16_422_unorm_khr       = VK_FORMAT_G16B16G16R16_422_UNORM_KHR;
        inline constexpr auto b16g16r16g16_422_unorm_khr       = VK_FORMAT_B16G16R16G16_422_UNORM_KHR;
        inline constexpr auto g16_b16_r16_3plane_420_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR;
        inline constexpr auto g16_b16r16_2plane_420_unorm_khr  = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR;
        inline constexpr auto g16_b16_r16_3plane_422_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR;
        inline constexpr auto g16_b16r16_2plane_422_unorm_khr  = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR;
        inline constexpr auto g16_b16_r16_3plane_444_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR;
        inline constexpr auto g8_b8r8_2plane_444_unorm_ext     = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT;
        inline constexpr auto g16_b16r16_2plane_444_unorm_ext  = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT;
        inline constexpr auto a4r4g4b4_unorm_pack16_ext        = VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT;
        inline constexpr auto a4b4g4r4_unorm_pack16_ext        = VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT;
    } // namespace formats

    namespace present_modes
    {
        using enum_t = VkPresentModeKHR;

        inline constexpr auto immediate_khr                 = VK_PRESENT_MODE_IMMEDIATE_KHR;
        inline constexpr auto mailbox_khr                   = VK_PRESENT_MODE_MAILBOX_KHR;
        inline constexpr auto fifo_khr                      = VK_PRESENT_MODE_FIFO_KHR;
        inline constexpr auto fifo_relaxed_khr              = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        inline constexpr auto shared_demand_refresh_khr     = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
        inline constexpr auto shared_continuous_refresh_khr = VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;
    } // namespace present_modes

    namespace color_spaces
    {
        using enum_t = VkColorSpaceKHR;

        inline constexpr auto srgb_nonlinear_khr          = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        inline constexpr auto display_p3_nonlinear_ext    = VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT;
        inline constexpr auto extended_srgb_linear_ext    = VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
        inline constexpr auto display_p3_linear_ext       = VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT;
        inline constexpr auto dci_p3_nonlinear_ext        = VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT;
        inline constexpr auto bt709_linear_ext            = VK_COLOR_SPACE_BT709_LINEAR_EXT;
        inline constexpr auto bt709_nonlinear_ext         = VK_COLOR_SPACE_BT709_NONLINEAR_EXT;
        inline constexpr auto bt2020_linear_ext           = VK_COLOR_SPACE_BT2020_LINEAR_EXT;
        inline constexpr auto hdr10_st2084_ext            = VK_COLOR_SPACE_HDR10_ST2084_EXT;
        inline constexpr auto dolbyvision_ext             = VK_COLOR_SPACE_DOLBYVISION_EXT;
        inline constexpr auto hdr10_hlg_ext               = VK_COLOR_SPACE_HDR10_HLG_EXT;
        inline constexpr auto adobergb_linear_ext         = VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT;
        inline constexpr auto adobergb_nonlinear_ext      = VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT;
        inline constexpr auto pass_through_ext            = VK_COLOR_SPACE_PASS_THROUGH_EXT;
        inline constexpr auto extended_srgb_nonlinear_ext = VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT;
        inline constexpr auto display_native_amd          = VK_COLOR_SPACE_DISPLAY_NATIVE_AMD;
        inline constexpr auto dci_p3_linear_ext           = VK_COLOR_SPACE_DCI_P3_LINEAR_EXT;
    } // namespace color_spaces

    namespace image_usage_flags
    {
        using enum_t = VkImageUsageFlagBits;

        inline constexpr auto transfer_src                 = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        inline constexpr auto transfer_dst                 = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        inline constexpr auto sampled                      = VK_IMAGE_USAGE_SAMPLED_BIT;
        inline constexpr auto storage                      = VK_IMAGE_USAGE_STORAGE_BIT;
        inline constexpr auto color_attachment             = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        inline constexpr auto depth_stencil_attachment     = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        inline constexpr auto transient_attachment         = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
        inline constexpr auto input_attachment             = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
        inline constexpr auto video_decode_dst_khr         = VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR;
        inline constexpr auto video_decode_src_khr         = VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR;
        inline constexpr auto video_decode_dpb_khr         = VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR;
        inline constexpr auto fragment_density_map_ext     = VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;
        inline constexpr auto invocation_mask_huawei       = VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI;
        inline constexpr auto sample_weight_qcom           = VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM;
        inline constexpr auto sample_block_match_qcom      = VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM;
        inline constexpr auto shading_rate_image_nv        = VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV;
        inline constexpr auto attachment_feedback_loop_ext = VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT;

        inline constexpr auto fragment_shading_rate_attachment_khr = VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_dst_khr = VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_src_khr = VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_dpb_khr = VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR;
#endif
    } // namespace image_usage_flags

    namespace sharing_modes
    {
        using enum_t = VkSharingMode;

        inline constexpr auto exclusive  = VK_SHARING_MODE_EXCLUSIVE;
        inline constexpr auto concurrent = VK_SHARING_MODE_CONCURRENT;
    } // namespace sharing_modes

    namespace surface_transform_flag
    {
        using enum_t = VkSurfaceTransformFlagBitsKHR;

        inline constexpr auto identity_khr          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        inline constexpr auto rotate_90_khr         = VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
        inline constexpr auto rotate_180_khr        = VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
        inline constexpr auto rotate_270_khr        = VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
        inline constexpr auto horizontal_mirror_khr = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;
        inline constexpr auto inherit_khr           = VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;

        inline constexpr auto horizontal_mirror_rotate_90_khr  = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;
        inline constexpr auto horizontal_mirror_rotate_180_khr = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;
        inline constexpr auto horizontal_mirror_rotate_270_khr = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;
    } // namespace surface_transform_flag

    namespace composite_alpha_flag
    {
        using enum_t = VkCompositeAlphaFlagBitsKHR;

        inline constexpr auto opaque_khr          = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        inline constexpr auto pre_multiplied_khr  = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
        inline constexpr auto post_multiplied_khr = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
        inline constexpr auto inherit_khr         = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    } // namespace composite_alpha_flag

    namespace image_tilings
    {
        using enum_t = VkImageTiling;

        inline constexpr auto optimal                 = VK_IMAGE_TILING_OPTIMAL;
        inline constexpr auto linear                  = VK_IMAGE_TILING_LINEAR;
        inline constexpr auto drm_format_modifier_ext = VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT;
    } // namespace image_tilings

    namespace image_types
    {
        using enum_t = VkImageType;

        inline constexpr auto _1d = VK_IMAGE_TYPE_1D;
        inline constexpr auto _2d = VK_IMAGE_TYPE_2D;
        inline constexpr auto _3d = VK_IMAGE_TYPE_3D;
    } // namespace image_types

    namespace image_view_types
    {
        using enum_t = VkImageViewType;

        inline constexpr auto type_1d         = VK_IMAGE_VIEW_TYPE_1D;
        inline constexpr auto type_2d         = VK_IMAGE_VIEW_TYPE_2D;
        inline constexpr auto type_3d         = VK_IMAGE_VIEW_TYPE_3D;
        inline constexpr auto type_cube       = VK_IMAGE_VIEW_TYPE_CUBE;
        inline constexpr auto type_1d_array   = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        inline constexpr auto type_2d_array   = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        inline constexpr auto type_cube_array = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
    } // namespace image_view_types

    namespace component_swizzles
    {
        using enum_t = VkComponentSwizzle;

        inline constexpr auto identity = VK_COMPONENT_SWIZZLE_IDENTITY;
        inline constexpr auto zero     = VK_COMPONENT_SWIZZLE_ZERO;
        inline constexpr auto one      = VK_COMPONENT_SWIZZLE_ONE;
        inline constexpr auto r        = VK_COMPONENT_SWIZZLE_R;
        inline constexpr auto g        = VK_COMPONENT_SWIZZLE_G;
        inline constexpr auto b        = VK_COMPONENT_SWIZZLE_B;
        inline constexpr auto a        = VK_COMPONENT_SWIZZLE_A;
    } // namespace component_swizzles

    namespace sample_count_flags
    {
        using enum_t = VkSampleCountFlagBits;

        inline constexpr auto _1  = VK_SAMPLE_COUNT_1_BIT;
        inline constexpr auto _2  = VK_SAMPLE_COUNT_2_BIT;
        inline constexpr auto _4  = VK_SAMPLE_COUNT_4_BIT;
        inline constexpr auto _8  = VK_SAMPLE_COUNT_8_BIT;
        inline constexpr auto _16 = VK_SAMPLE_COUNT_16_BIT;
        inline constexpr auto _32 = VK_SAMPLE_COUNT_32_BIT;
        inline constexpr auto _64 = VK_SAMPLE_COUNT_64_BIT;
    } // namespace sample_count_flags

    namespace descriptor_pool_create_flags
    {
        using enum_t = VkDescriptorPoolCreateFlagBits;

        inline constexpr auto free_descriptor_set_bit   = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        inline constexpr auto update_after_bind_bit     = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
        inline constexpr auto host_only_bit_ext         = VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT;
        inline constexpr auto update_after_bind_bit_ext = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
        inline constexpr auto host_only_bit_valve       = VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_VALVE;
    } // namespace descriptor_pool_create_flags

    namespace image_layouts
    {
        using enum_t = VkImageLayout;

        inline constexpr auto undefined                                      = VK_IMAGE_LAYOUT_UNDEFINED;
        inline constexpr auto general                                        = VK_IMAGE_LAYOUT_GENERAL;
        inline constexpr auto color_attachment_optimal                       = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        inline constexpr auto depth_stencil_attachment_optimal               = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        inline constexpr auto depth_stencil_read_only_optimal                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        inline constexpr auto shader_read_only_optimal                       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        inline constexpr auto transfer_src_optimal                           = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        inline constexpr auto transfer_dst_optimal                           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        inline constexpr auto preinitialized                                 = VK_IMAGE_LAYOUT_PREINITIALIZED;
        inline constexpr auto depth_read_only_stencil_attachment_optimal     = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
        inline constexpr auto depth_attachment_stencil_read_only_optimal     = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
        inline constexpr auto depth_attachment_optimal                       = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        inline constexpr auto depth_read_only_optimal                        = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
        inline constexpr auto stencil_attachment_optimal                     = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
        inline constexpr auto stencil_read_only_optimal                      = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
        inline constexpr auto read_only_optimal                              = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
        inline constexpr auto attachment_optimal                             = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
        inline constexpr auto present_src_khr                                = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        inline constexpr auto video_decode_dst_khr                           = VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR;
        inline constexpr auto video_decode_src_khr                           = VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR;
        inline constexpr auto video_decode_dpb_khr                           = VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR;
        inline constexpr auto shared_present_khr                             = VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
        inline constexpr auto fragment_density_map_optimal_ext               = VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT;
        inline constexpr auto fragment_shading_rate_attachment_optimal_khr   = VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
        inline constexpr auto attachment_feedback_loop_optimal_ext           = VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;
        inline constexpr auto depth_read_only_stencil_attachment_optimal_khr = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR;
        inline constexpr auto depth_attachment_stencil_read_only_optimal_khr = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR;
        inline constexpr auto shading_rate_optimal_nv                        = VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV;
        inline constexpr auto depth_attachment_optimal_khr                   = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR;
        inline constexpr auto depth_read_only_optimal_khr                    = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR;
        inline constexpr auto stencil_attachment_optimal_khr                 = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR;
        inline constexpr auto stencil_read_only_optimal_khr                  = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR;
        inline constexpr auto read_only_optimal_khr                          = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR;
        inline constexpr auto attachment_optimal_khr                         = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_dst_khr = VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_src_khr = VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_dpb_khr = VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR;
#endif
    } // namespace image_layouts

    namespace command_pool_create_flags
    {
        using enum_t = VkCommandPoolCreateFlagBits;

        inline constexpr auto transient_bit            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        inline constexpr auto reset_command_buffer_bit = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        inline constexpr auto protected_bit            = VK_COMMAND_POOL_CREATE_PROTECTED_BIT;
    } // namespace command_pool_create_flags

    namespace memory_property_flags
    {
        using enum_t = VkMemoryPropertyFlagBits;

        inline constexpr auto device_local_bit        = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        inline constexpr auto host_visible_bit        = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        inline constexpr auto host_coherent_bit       = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        inline constexpr auto host_cached_bit         = VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
        inline constexpr auto lazily_allocated_bit    = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
        inline constexpr auto protected_bit           = VK_MEMORY_PROPERTY_PROTECTED_BIT;
        inline constexpr auto device_coherent_bit_amd = VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD;
        inline constexpr auto device_uncached_bit_amd = VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD;
        inline constexpr auto rdma_capable_bit_nv     = VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV;
    } // namespace memory_property_flags

    namespace memory_usages
    {
        using enum_t = VmaMemoryUsage;

        inline constexpr auto unknown                 = VMA_MEMORY_USAGE_UNKNOWN;
        inline constexpr auto gpu_lazily_allocated    = VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
        inline constexpr auto automatic               = VMA_MEMORY_USAGE_AUTO;
        inline constexpr auto automatic_prefer_device = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        inline constexpr auto automatic_prefer_host   = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
    } // namespace memory_usages

    namespace memory_flags
    {
        using enum_t = VmaAllocationCreateFlagBits;

        /** \brief Set this flag if the allocation should have its own memory block.

        Use it for special, big resources, like fullscreen images used as attachments.

        If you use this flag while creating a buffer or an image, `VkMemoryDedicatedAllocateInfo`
        structure is applied if possible.
        */
        inline constexpr auto dedicated_memory = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

        /** \brief Set this flag to only try to allocate from existing `VkDeviceMemory` blocks and never create new such block.

        If new allocation cannot be placed in any of the existing blocks, allocation
        fails with `VK_ERROR_OUT_OF_DEVICE_MEMORY` error.

        You should not use #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT and
        #VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT at the same time. It makes no sense.
        */
        inline constexpr auto never_allocate = VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT;

        /** \brief Set this flag to use a memory that will be persistently mapped and retrieve pointer to it.

        Pointer to mapped memory will be returned through VmaAllocationInfo::pMappedData.

        It is valid to use this flag for allocation made from memory type that is not
        `HOST_VISIBLE`. This flag is then ignored and memory is not mapped. This is
        useful if you need an allocation that is efficient to use on GPU
        (`DEVICE_LOCAL`) and still want to map it directly if possible on platforms that
        support it (e.g. Intel GPU).
        */
        inline constexpr auto mapped = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        /** \deprecated Preserved for backward compatibility. Consider using vmaSetAllocationName() instead.

        Set this flag to treat VmaAllocationCreateInfo::pUserData as pointer to a
        null-terminated string. Instead of copying pointer value, a local copy of the
        string is made and stored in allocation's `pName`. The string is automatically
        freed together with the allocation. It is also used in vmaBuildStatsString().
        */
        inline constexpr auto user_data_copy_string = VMA_ALLOCATION_CREATE_USER_DATA_COPY_STRING_BIT;

        /** Allocation will be created from upper stack in a double stack pool.

        This flag is only allowed for custom pools created with #VMA_POOL_CREATE_LINEAR_ALGORITHM_BIT flag.
        */
        inline constexpr auto upper_address = VMA_ALLOCATION_CREATE_UPPER_ADDRESS_BIT;

        /** Create both buffer/image and allocation, but don't bind them together.
        It is useful when you want to bind yourself to do some more advanced binding, e.g. using some extensions.
        The flag is meaningful only with functions that bind by default: vmaCreateBuffer(), vmaCreateImage().
        Otherwise it is ignored.

        If you want to make sure the new buffer/image is not tied to the new memory allocation
        through `VkMemoryDedicatedAllocateInfoKHR` structure in case the allocation ends up in its own memory block,
        use also flag #VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT.
        */
        inline constexpr auto dont_bind = VMA_ALLOCATION_CREATE_DONT_BIND_BIT;

        /** Create allocation only if additional device memory required for it, if any, won't exceed
        memory budget. Otherwise return `VK_ERROR_OUT_OF_DEVICE_MEMORY`.
        */
        inline constexpr auto within_budget = VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT;

        /** \brief Set this flag if the allocated memory will have aliasing resources.

        Usage of this flag prevents supplying `VkMemoryDedicatedAllocateInfoKHR` when #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT is specified.
        Otherwise created dedicated memory will not be suitable for aliasing resources, resulting in Vulkan Validation Layer errors.
        */
        inline constexpr auto can_alias = VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT;

        /**
        Requests possibility to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT).

        - If you use #VMA_MEMORY_USAGE_AUTO or other `VMA_MEMORY_USAGE_AUTO*` value,
          you must use this flag to be able to map the allocation. Otherwise, mapping is incorrect.
        - If you use other value of #VmaMemoryUsage, this flag is ignored and mapping is always possible in memory types that are `HOST_VISIBLE`.
          This includes allocations created in \ref custom_memory_pools.

        Declares that mapped memory will only be written sequentially, e.g. using `memcpy()` or a loop writing number-by-number,
        never read or accessed randomly, so a memory type can be selected that is uncached and write-combined.

        \warning Violating this declaration may work correctly, but will likely be very slow.
        Watch out for implicit reads introduced by doing e.g. `pMappedData[i] += x;`
        Better prepare your data in a local variable and `memcpy()` it to the mapped pointer all at once.
        */
        inline constexpr auto host_access_sequential_write = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

        /**
        Requests possibility to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT).

        - If you use #VMA_MEMORY_USAGE_AUTO or other `VMA_MEMORY_USAGE_AUTO*` value,
          you must use this flag to be able to map the allocation. Otherwise, mapping is incorrect.
        - If you use other value of #VmaMemoryUsage, this flag is ignored and mapping is always possible in memory types that are `HOST_VISIBLE`.
          This includes allocations created in \ref custom_memory_pools.

        Declares that mapped memory can be read, written, and accessed in random order,
        so a `HOST_CACHED` memory type is preferred.
        */
        inline constexpr auto host_access_random = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;

        /**
        Together with #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
        it says that despite request for host access, a not-`HOST_VISIBLE` memory type can be selected
        if it may improve performance.

        By using this flag, you declare that you will check if the allocation ended up in a `HOST_VISIBLE` memory type
        (e.g. using vmaGetAllocationMemoryProperties()) and if not, you will create some "staging" buffer and
        issue an explicit transfer to write/read your data.
        To prepare for this possibility, don't forget to add appropriate flags like
        `VK_BUFFER_USAGE_TRANSFER_DST_BIT`, `VK_BUFFER_USAGE_TRANSFER_SRC_BIT` to the parameters of created buffer or image.
        */
        inline constexpr auto host_access_allow_transfer_instead = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT;

        /** Allocation strategy that chooses smallest possible free range for the allocation
        to minimize memory usage and fragmentation, possibly at the expense of allocation time.
        */
        inline constexpr auto strategy_min_memory = VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT;

        /** Allocation strategy that chooses first suitable free range for the allocation -
        not necessarily in terms of the smallest offset but the one that is easiest and fastest to find
        to minimize allocation time, possibly at the expense of allocation quality.
        */
        inline constexpr auto strategy_min_time = VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT;

        /** Allocation strategy that chooses always the lowest offset in available space.
        This is not the most efficient strategy but achieves highly packed data.
        Used internally by defragmentation, not recommended in typical usage.
        */
        inline constexpr auto strategy_min_offset = VMA_ALLOCATION_CREATE_STRATEGY_MIN_OFFSET_BIT;

        /** Alias to #VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT.
         */
        inline constexpr auto strategy_best_fit = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;

        /** Alias to #VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT.
         */
        inline constexpr auto strategy_first_fit = VMA_ALLOCATION_CREATE_STRATEGY_FIRST_FIT_BIT;

        /** A bit mask to extract only `STRATEGY` bits from entire set of flags.
         */
        inline constexpr auto strategy_mask = VMA_ALLOCATION_CREATE_STRATEGY_MASK;
    } // namespace memory_flags

    namespace filters
    {
        using enum_t = VkFilter;

        inline constexpr auto nearest   = VK_FILTER_NEAREST;
        inline constexpr auto linear    = VK_FILTER_LINEAR;
        inline constexpr auto cubic_ext = VK_FILTER_CUBIC_EXT;
        inline constexpr auto cubic_img = VK_FILTER_CUBIC_IMG;
    } // namespace filters

    namespace debug_utils_message_severity_flags
    {
        using enum_t                  = VkDebugUtilsMessageSeverityFlagBitsEXT;
        inline constexpr auto verbose = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        inline constexpr auto info    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
        inline constexpr auto warning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        inline constexpr auto error   = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    } // namespace debug_utils_message_severity_flags

    namespace debug_utils_message_type_flags
    {
        using enum_t                                 = VkDebugUtilsMessageTypeFlagBitsEXT;
        inline constexpr auto general                = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
        inline constexpr auto validation             = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        inline constexpr auto performance            = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        inline constexpr auto device_address_binding = VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    } // namespace debug_utils_message_type_flags

    namespace command_buffer_usage_flags
    {
        using enum_t                               = VkCommandBufferUsageFlags;
        inline constexpr auto one_time_submit      = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        inline constexpr auto render_pass_continue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
        inline constexpr auto simultaneous_use     = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    } // namespace command_buffer_usage_flags

    inline constexpr auto subpass_external = VK_SUBPASS_EXTERNAL;

    namespace subpass_description_flags
    {
        using enum_t = VkSubpassDescriptionFlags;

        inline constexpr auto per_view_attributes                           = VK_SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX;
        inline constexpr auto per_view_position_x_only                      = VK_SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX;
        inline constexpr auto fragment_region                               = VK_SUBPASS_DESCRIPTION_FRAGMENT_REGION_BIT_QCOM;
        inline constexpr auto shader_resolve                                = VK_SUBPASS_DESCRIPTION_SHADER_RESOLVE_BIT_QCOM;
        inline constexpr auto rasterization_order_attachment_color_access   = VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS_BIT_EXT;
        inline constexpr auto rasterization_order_attachment_depth_access   = VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_EXT;
        inline constexpr auto rasterization_order_attachment_stencil_access = VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_EXT;
        inline constexpr auto enable_legacy_dithering                       = VK_SUBPASS_DESCRIPTION_ENABLE_LEGACY_DITHERING_BIT_EXT;
    } // namespace subpass_description_flags

    namespace debug_report_object_type
    {
        using enum_t = VkDebugReportObjectTypeEXT;

        template <enum_t res>
        inline constexpr std::string_view repr = "unknown type";

#define define_repr_objtype(orbvar, vkvar) \
    inline constexpr auto orbvar = vkvar;  \
    template <>                            \
    inline constexpr std::string_view repr<orbvar> = #orbvar;

#define define_only_objtype(orbvar, vkvar) \
    inline constexpr auto orbvar = vkvar;

        define_repr_objtype(unknown, VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT);
        define_repr_objtype(instance, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT);
        define_repr_objtype(physical_device, VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT);
        define_repr_objtype(device, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT);
        define_repr_objtype(queue, VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT);
        define_repr_objtype(semaphore, VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT);
        define_repr_objtype(command_buffer, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT);
        define_repr_objtype(fence, VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT);
        define_repr_objtype(device_memory, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT);
        define_repr_objtype(buffer, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT);
        define_repr_objtype(image, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT);
        define_repr_objtype(event, VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT);
        define_repr_objtype(query_pool, VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT);
        define_repr_objtype(buffer_view, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT);
        define_repr_objtype(image_view, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT);
        define_repr_objtype(shader_module, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT);
        define_repr_objtype(pipeline_cache, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT);
        define_repr_objtype(pipeline_layout, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT);
        define_repr_objtype(render_pass, VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT);
        define_repr_objtype(pipeline, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT);
        define_repr_objtype(descriptor_set_layout, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT);
        define_repr_objtype(sampler, VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT);
        define_repr_objtype(descriptor_pool, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT);
        define_repr_objtype(descriptor_set, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT);
        define_repr_objtype(framebuffer, VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT);
        define_repr_objtype(command_pool, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT);
        define_repr_objtype(surface_khr, VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT);
        define_repr_objtype(swapchain_khr, VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT);
        define_repr_objtype(debug_report_callback, VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT);
        define_repr_objtype(display_khr, VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT);
        define_repr_objtype(display_mode_khr, VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT);
        define_repr_objtype(validation_cache, VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT);
        define_repr_objtype(sampler_ycbcr_conversion, VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_EXT);
        define_repr_objtype(descriptor_update_template, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_EXT);
        define_repr_objtype(cu_module_nvx, VK_DEBUG_REPORT_OBJECT_TYPE_CU_MODULE_NVX_EXT);
        define_repr_objtype(cu_function_nvx, VK_DEBUG_REPORT_OBJECT_TYPE_CU_FUNCTION_NVX_EXT);
        define_repr_objtype(acceleration_structure_khr, VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR_EXT);
        define_repr_objtype(acceleration_structure_nv, VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV_EXT);
        define_repr_objtype(buffer_collection_fuchsia, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA_EXT);
        define_only_objtype(debug_report, VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT);
        define_only_objtype(descriptor_update_template_khr, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_KHR_EXT);
        define_only_objtype(sampler_ycbcr_conversion_khr, VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR_EXT);

        inline constexpr auto get_repr(enum_t e) -> std::string_view
        {
            switch (e)
            {
            case unknown: return repr<unknown>;
            case instance: return repr<instance>;
            case physical_device: return repr<physical_device>;
            case device: return repr<device>;
            case queue: return repr<queue>;
            case semaphore: return repr<semaphore>;
            case command_buffer: return repr<command_buffer>;
            case fence: return repr<fence>;
            case device_memory: return repr<device_memory>;
            case buffer: return repr<buffer>;
            case image: return repr<image>;
            case event: return repr<event>;
            case query_pool: return repr<query_pool>;
            case buffer_view: return repr<buffer_view>;
            case image_view: return repr<image_view>;
            case shader_module: return repr<shader_module>;
            case pipeline_cache: return repr<pipeline_cache>;
            case pipeline_layout: return repr<pipeline_layout>;
            case render_pass: return repr<render_pass>;
            case pipeline: return repr<pipeline>;
            case descriptor_set_layout: return repr<descriptor_set_layout>;
            case sampler: return repr<sampler>;
            case descriptor_pool: return repr<descriptor_pool>;
            case descriptor_set: return repr<descriptor_set>;
            case framebuffer: return repr<framebuffer>;
            case command_pool: return repr<command_pool>;
            case surface_khr: return repr<surface_khr>;
            case swapchain_khr: return repr<swapchain_khr>;
            case debug_report_callback: return repr<debug_report_callback>;
            case display_khr: return repr<display_khr>;
            case display_mode_khr: return repr<display_mode_khr>;
            case validation_cache: return repr<validation_cache>;
            case sampler_ycbcr_conversion: return repr<sampler_ycbcr_conversion>;
            case descriptor_update_template: return repr<descriptor_update_template>;
            case cu_module_nvx: return repr<cu_module_nvx>;
            case cu_function_nvx: return repr<cu_function_nvx>;
            case acceleration_structure_khr: return repr<acceleration_structure_khr>;
            case acceleration_structure_nv: return repr<acceleration_structure_nv>;
            case buffer_collection_fuchsia: return repr<buffer_collection_fuchsia>;

            default: return repr<VK_DEBUG_REPORT_OBJECT_TYPE_MAX_ENUM_EXT>;
            }
        }
    } // namespace debug_report_object_type

    namespace obj_types
    {
        using enum_t = VkObjectType;

        inline constexpr auto unknown = VK_OBJECT_TYPE_UNKNOWN;
        template <typename T>
        inline constexpr enum_t obj_type = unknown;

#define define_obj_type(name, obj_type_name, vk_type_name) \
    inline constexpr auto name = obj_type_name;            \
    template <>                                            \
    inline constexpr auto obj_type<vk_type_name> = name;

        define_obj_type(instance, VK_OBJECT_TYPE_INSTANCE, VkInstance);
        define_obj_type(physical_device, VK_OBJECT_TYPE_PHYSICAL_DEVICE, VkPhysicalDevice);
        define_obj_type(device, VK_OBJECT_TYPE_DEVICE, VkDevice);
        define_obj_type(queue, VK_OBJECT_TYPE_QUEUE, VkQueue);
        define_obj_type(semaphore, VK_OBJECT_TYPE_SEMAPHORE, VkSemaphore);
        define_obj_type(command_buffer, VK_OBJECT_TYPE_COMMAND_BUFFER, VkCommandBuffer);
        define_obj_type(fence, VK_OBJECT_TYPE_FENCE, VkFence);
        define_obj_type(device_memory, VK_OBJECT_TYPE_DEVICE_MEMORY, VkDeviceMemory);
        define_obj_type(buffer, VK_OBJECT_TYPE_BUFFER, VkBuffer);
        define_obj_type(image, VK_OBJECT_TYPE_IMAGE, VkImage);
        define_obj_type(event, VK_OBJECT_TYPE_EVENT, VkEvent);
        define_obj_type(query_pool, VK_OBJECT_TYPE_QUERY_POOL, VkQueryPool);
        define_obj_type(buffer_view, VK_OBJECT_TYPE_BUFFER_VIEW, VkBufferView);
        define_obj_type(image_view, VK_OBJECT_TYPE_IMAGE_VIEW, VkImageView);
        define_obj_type(shader_module, VK_OBJECT_TYPE_SHADER_MODULE, VkShaderModule);
        define_obj_type(pipeline_cache, VK_OBJECT_TYPE_PIPELINE_CACHE, VkPipelineCache);
        define_obj_type(pipeline_layout, VK_OBJECT_TYPE_PIPELINE_LAYOUT, VkPipelineLayout);
        define_obj_type(render_pass, VK_OBJECT_TYPE_RENDER_PASS, VkRenderPass);
        define_obj_type(pipeline, VK_OBJECT_TYPE_PIPELINE, VkPipeline);
        define_obj_type(descriptor_set_layout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, VkDescriptorSetLayout);
        define_obj_type(sampler, VK_OBJECT_TYPE_SAMPLER, VkSampler);
        define_obj_type(descriptor_pool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, VkDescriptorPool);
        define_obj_type(descriptor_set, VK_OBJECT_TYPE_DESCRIPTOR_SET, VkDescriptorSet);
        define_obj_type(framebuffer, VK_OBJECT_TYPE_FRAMEBUFFER, VkFramebuffer);
        define_obj_type(command_pool, VK_OBJECT_TYPE_COMMAND_POOL, VkCommandPool);
        define_obj_type(sampler_ycbcr_conversion, VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION, VkSamplerYcbcrConversion);
        define_obj_type(descriptor_update_template, VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE, VkDescriptorUpdateTemplate);
        define_obj_type(private_data_slot, VK_OBJECT_TYPE_PRIVATE_DATA_SLOT, VkPrivateDataSlot);
        define_obj_type(surface_khr, VK_OBJECT_TYPE_SURFACE_KHR, VkSurfaceKHR);
        define_obj_type(swapchain_khr, VK_OBJECT_TYPE_SWAPCHAIN_KHR, VkSwapchainKHR);
        define_obj_type(display_khr, VK_OBJECT_TYPE_DISPLAY_KHR, VkDisplayKHR);
        define_obj_type(display_mode_khr, VK_OBJECT_TYPE_DISPLAY_MODE_KHR, VkDisplayModeKHR);
        define_obj_type(debug_report_callback_ext, VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT, VkDebugReportCallbackEXT);
        define_obj_type(video_session_khr, VK_OBJECT_TYPE_VIDEO_SESSION_KHR, VkVideoSessionKHR);
        define_obj_type(video_session_parameters_khr, VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR, VkVideoSessionParametersKHR);
        define_obj_type(cu_module_nvx, VK_OBJECT_TYPE_CU_MODULE_NVX, VkCuModuleNVX);
        define_obj_type(cu_function_nvx, VK_OBJECT_TYPE_CU_FUNCTION_NVX, VkCuFunctionNVX);
        define_obj_type(debug_utils_messenger_ext, VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT, VkDebugUtilsMessengerEXT);
        define_obj_type(acceleration_structure_khr, VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, VkAccelerationStructureKHR);
        define_obj_type(validation_cache_ext, VK_OBJECT_TYPE_VALIDATION_CACHE_EXT, VkValidationCacheEXT);
        define_obj_type(performance_configuration_intel, VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL, VkPerformanceConfigurationINTEL);
        define_obj_type(deferred_operation_khr, VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR, VkDeferredOperationKHR);
        define_obj_type(indirect_commands_layout_nv, VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV, VkIndirectCommandsLayoutNV);
        define_obj_type(micromap_ext, VK_OBJECT_TYPE_MICROMAP_EXT, VkMicromapEXT);
        define_obj_type(optical_flow_session_nv, VK_OBJECT_TYPE_OPTICAL_FLOW_SESSION_NV, VkOpticalFlowSessionNV);

        template <typename T>
        concept vk_type = obj_type<T> != unknown;
    } // namespace obj_types

    template <typename T>
    concept vk_type = obj_types::vk_type<T>;

    namespace shader_stage_flags
    {
        using enum_t = VkShaderStageFlagBits;

        inline constexpr auto vertex                 = VK_SHADER_STAGE_VERTEX_BIT;
        inline constexpr auto tesselation_control    = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        inline constexpr auto tesselation_evaluation = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        inline constexpr auto geometry               = VK_SHADER_STAGE_GEOMETRY_BIT;
        inline constexpr auto fragment               = VK_SHADER_STAGE_FRAGMENT_BIT;
        inline constexpr auto compute                = VK_SHADER_STAGE_COMPUTE_BIT;
        inline constexpr auto all_graphics           = VK_SHADER_STAGE_ALL_GRAPHICS;
        inline constexpr auto all                    = VK_SHADER_STAGE_ALL;
        inline constexpr auto raygen                 = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        inline constexpr auto any_hit                = VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
        inline constexpr auto closest_hit            = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        inline constexpr auto miss                   = VK_SHADER_STAGE_MISS_BIT_KHR;
        inline constexpr auto intersection           = VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
        inline constexpr auto callable               = VK_SHADER_STAGE_CALLABLE_BIT_KHR;
        inline constexpr auto task                   = VK_SHADER_STAGE_TASK_BIT_EXT;
        inline constexpr auto mesh                   = VK_SHADER_STAGE_MESH_BIT_EXT;
        inline constexpr auto subpass_shading_huawei = VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI;
        inline constexpr auto cluster_culling_huawei = VK_SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI;
        inline constexpr auto raygen_nv              = VK_SHADER_STAGE_RAYGEN_BIT_NV;
        inline constexpr auto any_hit_nv             = VK_SHADER_STAGE_ANY_HIT_BIT_NV;
        inline constexpr auto closest_hit_nv         = VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV;
        inline constexpr auto miss_nv                = VK_SHADER_STAGE_MISS_BIT_NV;
        inline constexpr auto intersection_nv        = VK_SHADER_STAGE_INTERSECTION_BIT_NV;
        inline constexpr auto callable_nv            = VK_SHADER_STAGE_CALLABLE_BIT_NV;
        inline constexpr auto task_nv                = VK_SHADER_STAGE_TASK_BIT_NV;
        inline constexpr auto mesh_nv                = VK_SHADER_STAGE_MESH_BIT_NV;
    } // namespace shader_stage_flags

    namespace dynamic_states
    {
        using enum_t = VkDynamicState;

        inline constexpr auto viewport                                = VK_DYNAMIC_STATE_VIEWPORT;
        inline constexpr auto scissor                                 = VK_DYNAMIC_STATE_SCISSOR;
        inline constexpr auto line_width                              = VK_DYNAMIC_STATE_LINE_WIDTH;
        inline constexpr auto depth_bias                              = VK_DYNAMIC_STATE_DEPTH_BIAS;
        inline constexpr auto blend_constants                         = VK_DYNAMIC_STATE_BLEND_CONSTANTS;
        inline constexpr auto depth_bounds                            = VK_DYNAMIC_STATE_DEPTH_BOUNDS;
        inline constexpr auto stencil_compare_mask                    = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
        inline constexpr auto stencil_write_mask                      = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
        inline constexpr auto stencil_reference                       = VK_DYNAMIC_STATE_STENCIL_REFERENCE;
        inline constexpr auto cull_mode                               = VK_DYNAMIC_STATE_CULL_MODE;
        inline constexpr auto front_face                              = VK_DYNAMIC_STATE_FRONT_FACE;
        inline constexpr auto primitive_topology                      = VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY;
        inline constexpr auto viewport_with_count                     = VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT;
        inline constexpr auto scissor_with_count                      = VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT;
        inline constexpr auto vertex_input_binding_stride             = VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE;
        inline constexpr auto depth_test_enable                       = VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE;
        inline constexpr auto depth_write_enable                      = VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE;
        inline constexpr auto depth_compare_op                        = VK_DYNAMIC_STATE_DEPTH_COMPARE_OP;
        inline constexpr auto depth_bounds_test_enable                = VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE;
        inline constexpr auto stencil_test_enable                     = VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE;
        inline constexpr auto stencil_op                              = VK_DYNAMIC_STATE_STENCIL_OP;
        inline constexpr auto rasterizer_discard_enable               = VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE;
        inline constexpr auto depth_bias_enable                       = VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE;
        inline constexpr auto primitive_restart_enable                = VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE;
        inline constexpr auto viewport_w_scaling_nv                   = VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV;
        inline constexpr auto discard_rectangle_ext                   = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT;
        inline constexpr auto sample_locations_ext                    = VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT;
        inline constexpr auto ray_tracing_pipeline_stack_size_khr     = VK_DYNAMIC_STATE_RAY_TRACING_PIPELINE_STACK_SIZE_KHR;
        inline constexpr auto viewport_shading_rate_palette_nv        = VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV;
        inline constexpr auto viewport_coarse_sample_order_nv         = VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV;
        inline constexpr auto exclusive_scissor_nv                    = VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV;
        inline constexpr auto fragment_shading_rate_khr               = VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR;
        inline constexpr auto line_stipple_ext                        = VK_DYNAMIC_STATE_LINE_STIPPLE_EXT;
        inline constexpr auto vertex_input_ext                        = VK_DYNAMIC_STATE_VERTEX_INPUT_EXT;
        inline constexpr auto patch_control_points_ext                = VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT;
        inline constexpr auto logic_op_ext                            = VK_DYNAMIC_STATE_LOGIC_OP_EXT;
        inline constexpr auto color_write_enable_ext                  = VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT;
        inline constexpr auto tessellation_domain_origin_ext          = VK_DYNAMIC_STATE_TESSELLATION_DOMAIN_ORIGIN_EXT;
        inline constexpr auto depth_clamp_enable_ext                  = VK_DYNAMIC_STATE_DEPTH_CLAMP_ENABLE_EXT;
        inline constexpr auto polygon_mode_ext                        = VK_DYNAMIC_STATE_POLYGON_MODE_EXT;
        inline constexpr auto rasterization_samples_ext               = VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT;
        inline constexpr auto sample_mask_ext                         = VK_DYNAMIC_STATE_SAMPLE_MASK_EXT;
        inline constexpr auto alpha_to_coverage_enable_ext            = VK_DYNAMIC_STATE_ALPHA_TO_COVERAGE_ENABLE_EXT;
        inline constexpr auto alpha_to_one_enable_ext                 = VK_DYNAMIC_STATE_ALPHA_TO_ONE_ENABLE_EXT;
        inline constexpr auto logic_op_enable_ext                     = VK_DYNAMIC_STATE_LOGIC_OP_ENABLE_EXT;
        inline constexpr auto color_blend_enable_ext                  = VK_DYNAMIC_STATE_COLOR_BLEND_ENABLE_EXT;
        inline constexpr auto color_blend_equation_ext                = VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT;
        inline constexpr auto color_write_mask_ext                    = VK_DYNAMIC_STATE_COLOR_WRITE_MASK_EXT;
        inline constexpr auto rasterization_stream_ext                = VK_DYNAMIC_STATE_RASTERIZATION_STREAM_EXT;
        inline constexpr auto conservative_rasterization_mode_ext     = VK_DYNAMIC_STATE_CONSERVATIVE_RASTERIZATION_MODE_EXT;
        inline constexpr auto extra_primitive_overestimation_size_ext = VK_DYNAMIC_STATE_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT;
        inline constexpr auto depth_clip_enable_ext                   = VK_DYNAMIC_STATE_DEPTH_CLIP_ENABLE_EXT;
        inline constexpr auto sample_locations_enable_ext             = VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_ENABLE_EXT;
        inline constexpr auto color_blend_advanced_ext                = VK_DYNAMIC_STATE_COLOR_BLEND_ADVANCED_EXT;
        inline constexpr auto provoking_vertex_mode_ext               = VK_DYNAMIC_STATE_PROVOKING_VERTEX_MODE_EXT;
        inline constexpr auto line_rasterization_mode_ext             = VK_DYNAMIC_STATE_LINE_RASTERIZATION_MODE_EXT;
        inline constexpr auto line_stipple_enable_ext                 = VK_DYNAMIC_STATE_LINE_STIPPLE_ENABLE_EXT;
        inline constexpr auto depth_clip_negative_one_to_one_ext      = VK_DYNAMIC_STATE_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT;
        inline constexpr auto viewport_w_scaling_enable_nv            = VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_ENABLE_NV;
        inline constexpr auto viewport_swizzle_nv                     = VK_DYNAMIC_STATE_VIEWPORT_SWIZZLE_NV;
        inline constexpr auto coverage_to_color_enable_nv             = VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_ENABLE_NV;
        inline constexpr auto coverage_to_color_location_nv           = VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_LOCATION_NV;
        inline constexpr auto coverage_modulation_mode_nv             = VK_DYNAMIC_STATE_COVERAGE_MODULATION_MODE_NV;
        inline constexpr auto coverage_modulation_table_enable_nv     = VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_ENABLE_NV;
        inline constexpr auto coverage_modulation_table_nv            = VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_NV;
        inline constexpr auto shading_rate_image_enable_nv            = VK_DYNAMIC_STATE_SHADING_RATE_IMAGE_ENABLE_NV;
        inline constexpr auto representative_fragment_test_enable_nv  = VK_DYNAMIC_STATE_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV;
        inline constexpr auto coverage_reduction_mode_nv              = VK_DYNAMIC_STATE_COVERAGE_REDUCTION_MODE_NV;
        inline constexpr auto cull_mode_ext                           = VK_DYNAMIC_STATE_CULL_MODE_EXT;
        inline constexpr auto front_face_ext                          = VK_DYNAMIC_STATE_FRONT_FACE_EXT;
        inline constexpr auto primitive_topology_ext                  = VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY_EXT;
        inline constexpr auto viewport_with_count_ext                 = VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT_EXT;
        inline constexpr auto scissor_with_count_ext                  = VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT_EXT;
        inline constexpr auto vertex_input_binding_stride_ext         = VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE_EXT;
        inline constexpr auto depth_test_enable_ext                   = VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE_EXT;
        inline constexpr auto depth_write_enable_ext                  = VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE_EXT;
        inline constexpr auto depth_compare_op_ext                    = VK_DYNAMIC_STATE_DEPTH_COMPARE_OP_EXT;
        inline constexpr auto depth_bounds_test_enable_ext            = VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE_EXT;
        inline constexpr auto stencil_test_enable_ext                 = VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE_EXT;
        inline constexpr auto stencil_op_ext                          = VK_DYNAMIC_STATE_STENCIL_OP_EXT;
        inline constexpr auto rasterizer_discard_enable_ext           = VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE_EXT;
        inline constexpr auto depth_bias_enable_ext                   = VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE_EXT;
        inline constexpr auto primitive_restart_enable_ext            = VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE_EXT;
    } // namespace dynamic_states

    namespace primitive_topologies
    {
        using enum_t = VkPrimitiveTopology;

        inline constexpr auto point_list                    = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        inline constexpr auto line_list                     = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        inline constexpr auto line_strip                    = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        inline constexpr auto triangle_list                 = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inline constexpr auto triangle_strip                = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        inline constexpr auto triangle_fan                  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        inline constexpr auto line_list_with_adjacency      = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
        inline constexpr auto line_strip_with_adjacency     = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
        inline constexpr auto triangle_list_with_adjacency  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
        inline constexpr auto triangle_strip_with_adjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
        inline constexpr auto patch_list                    = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
    } // namespace primitive_topologies

    namespace polygon_modes
    {
        using enum_t = VkPolygonMode;

        inline constexpr auto fill              = VK_POLYGON_MODE_FILL;
        inline constexpr auto line              = VK_POLYGON_MODE_LINE;
        inline constexpr auto point             = VK_POLYGON_MODE_POINT;
        inline constexpr auto fill_rectangle_nv = VK_POLYGON_MODE_FILL_RECTANGLE_NV;
    } // namespace polygon_modes

    namespace cull_modes
    {
        using enum_t = VkCullModeFlagBits;

        inline constexpr auto none           = VK_CULL_MODE_NONE;
        inline constexpr auto front          = VK_CULL_MODE_FRONT_BIT;
        inline constexpr auto back           = VK_CULL_MODE_BACK_BIT;
        inline constexpr auto front_and_back = VK_CULL_MODE_FRONT_AND_BACK;
    } // namespace cull_modes

    namespace front_faces
    {
        using enum_t = VkFrontFace;

        inline constexpr auto counter_clockwise = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        inline constexpr auto clockwise         = VK_FRONT_FACE_CLOCKWISE;
    } // namespace front_faces

    namespace color_components
    {
        using enum_t = VkColorComponentFlagBits;

        inline constexpr auto r = VK_COLOR_COMPONENT_R_BIT;
        inline constexpr auto g = VK_COLOR_COMPONENT_G_BIT;
        inline constexpr auto b = VK_COLOR_COMPONENT_B_BIT;
        inline constexpr auto a = VK_COLOR_COMPONENT_A_BIT;
    } // namespace color_components

    namespace blend_factors
    {
        using enum_t = VkBlendFactor;

        inline constexpr auto zero                     = VK_BLEND_FACTOR_ZERO;
        inline constexpr auto one                      = VK_BLEND_FACTOR_ONE;
        inline constexpr auto src_color                = VK_BLEND_FACTOR_SRC_COLOR;
        inline constexpr auto one_minus_src_color      = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        inline constexpr auto dst_color                = VK_BLEND_FACTOR_DST_COLOR;
        inline constexpr auto one_minus_dst_color      = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        inline constexpr auto src_alpha                = VK_BLEND_FACTOR_SRC_ALPHA;
        inline constexpr auto one_minus_src_alpha      = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        inline constexpr auto dst_alpha                = VK_BLEND_FACTOR_DST_ALPHA;
        inline constexpr auto one_minus_dst_alpha      = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        inline constexpr auto constant_color           = VK_BLEND_FACTOR_CONSTANT_COLOR;
        inline constexpr auto one_minus_constant_color = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        inline constexpr auto constant_alpha           = VK_BLEND_FACTOR_CONSTANT_ALPHA;
        inline constexpr auto one_minus_constant_alpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        inline constexpr auto src_alpha_saturate       = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
        inline constexpr auto src1_color               = VK_BLEND_FACTOR_SRC1_COLOR;
        inline constexpr auto one_minus_src1_color     = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        inline constexpr auto src1_alpha               = VK_BLEND_FACTOR_SRC1_ALPHA;
        inline constexpr auto one_minus_src1_alpha     = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
    } // namespace blend_factors

    namespace blend_ops
    {
        using enum_t = VkBlendOp;

        inline constexpr auto add                    = VK_BLEND_OP_ADD;
        inline constexpr auto subtract               = VK_BLEND_OP_SUBTRACT;
        inline constexpr auto reverse_subtract       = VK_BLEND_OP_REVERSE_SUBTRACT;
        inline constexpr auto min                    = VK_BLEND_OP_MIN;
        inline constexpr auto max                    = VK_BLEND_OP_MAX;
        inline constexpr auto zero_ext               = VK_BLEND_OP_ZERO_EXT;
        inline constexpr auto src_ext                = VK_BLEND_OP_SRC_EXT;
        inline constexpr auto dst_ext                = VK_BLEND_OP_DST_EXT;
        inline constexpr auto src_over_ext           = VK_BLEND_OP_SRC_OVER_EXT;
        inline constexpr auto dst_over_ext           = VK_BLEND_OP_DST_OVER_EXT;
        inline constexpr auto src_in_ext             = VK_BLEND_OP_SRC_IN_EXT;
        inline constexpr auto dst_in_ext             = VK_BLEND_OP_DST_IN_EXT;
        inline constexpr auto src_out_ext            = VK_BLEND_OP_SRC_OUT_EXT;
        inline constexpr auto dst_out_ext            = VK_BLEND_OP_DST_OUT_EXT;
        inline constexpr auto src_atop_ext           = VK_BLEND_OP_SRC_ATOP_EXT;
        inline constexpr auto dst_atop_ext           = VK_BLEND_OP_DST_ATOP_EXT;
        inline constexpr auto xor_ext                = VK_BLEND_OP_XOR_EXT;
        inline constexpr auto multiply_ext           = VK_BLEND_OP_MULTIPLY_EXT;
        inline constexpr auto screen_ext             = VK_BLEND_OP_SCREEN_EXT;
        inline constexpr auto overlay_ext            = VK_BLEND_OP_OVERLAY_EXT;
        inline constexpr auto darken_ext             = VK_BLEND_OP_DARKEN_EXT;
        inline constexpr auto lighten_ext            = VK_BLEND_OP_LIGHTEN_EXT;
        inline constexpr auto colordodge_ext         = VK_BLEND_OP_COLORDODGE_EXT;
        inline constexpr auto colorburn_ext          = VK_BLEND_OP_COLORBURN_EXT;
        inline constexpr auto hardlight_ext          = VK_BLEND_OP_HARDLIGHT_EXT;
        inline constexpr auto softlight_ext          = VK_BLEND_OP_SOFTLIGHT_EXT;
        inline constexpr auto difference_ext         = VK_BLEND_OP_DIFFERENCE_EXT;
        inline constexpr auto exclusion_ext          = VK_BLEND_OP_EXCLUSION_EXT;
        inline constexpr auto invert_ext             = VK_BLEND_OP_INVERT_EXT;
        inline constexpr auto invert_rgb_ext         = VK_BLEND_OP_INVERT_RGB_EXT;
        inline constexpr auto lineardodge_ext        = VK_BLEND_OP_LINEARDODGE_EXT;
        inline constexpr auto linearburn_ext         = VK_BLEND_OP_LINEARBURN_EXT;
        inline constexpr auto vividlight_ext         = VK_BLEND_OP_VIVIDLIGHT_EXT;
        inline constexpr auto linearlight_ext        = VK_BLEND_OP_LINEARLIGHT_EXT;
        inline constexpr auto pinlight_ext           = VK_BLEND_OP_PINLIGHT_EXT;
        inline constexpr auto hardmix_ext            = VK_BLEND_OP_HARDMIX_EXT;
        inline constexpr auto hsl_hue_ext            = VK_BLEND_OP_HSL_HUE_EXT;
        inline constexpr auto hsl_saturation_ext     = VK_BLEND_OP_HSL_SATURATION_EXT;
        inline constexpr auto hsl_color_ext          = VK_BLEND_OP_HSL_COLOR_EXT;
        inline constexpr auto hsl_luminosity_ext     = VK_BLEND_OP_HSL_LUMINOSITY_EXT;
        inline constexpr auto plus_ext               = VK_BLEND_OP_PLUS_EXT;
        inline constexpr auto plus_clamped_ext       = VK_BLEND_OP_PLUS_CLAMPED_EXT;
        inline constexpr auto plus_clamped_alpha_ext = VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT;
        inline constexpr auto plus_darker_ext        = VK_BLEND_OP_PLUS_DARKER_EXT;
        inline constexpr auto minus_ext              = VK_BLEND_OP_MINUS_EXT;
        inline constexpr auto minus_clamped_ext      = VK_BLEND_OP_MINUS_CLAMPED_EXT;
        inline constexpr auto contrast_ext           = VK_BLEND_OP_CONTRAST_EXT;
        inline constexpr auto invert_ovg_ext         = VK_BLEND_OP_INVERT_OVG_EXT;
        inline constexpr auto red_ext                = VK_BLEND_OP_RED_EXT;
        inline constexpr auto green_ext              = VK_BLEND_OP_GREEN_EXT;
        inline constexpr auto blue_ext               = VK_BLEND_OP_BLUE_EXT;
    } // namespace blend_ops

    namespace shader_kinds
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
    } // namespace shader_kinds

    namespace vertex_input_rates
    {
        using enum_t = VkVertexInputRate;

        inline constexpr auto vertex   = VK_VERTEX_INPUT_RATE_VERTEX;
        inline constexpr auto instance = VK_VERTEX_INPUT_RATE_INSTANCE;
    } // namespace vertex_input_rates

    namespace vertex_formats
    {
        using enum_t = VkFormat;

        inline constexpr auto float_t  = VK_FORMAT_R32_SFLOAT;
        inline constexpr auto vec2_t   = VK_FORMAT_R32G32_SFLOAT;
        inline constexpr auto vec3_t   = VK_FORMAT_R32G32B32_SFLOAT;
        inline constexpr auto vec4_t   = VK_FORMAT_R32G32B32A32_SFLOAT;
        inline constexpr auto int_t    = VK_FORMAT_R32_SINT;
        inline constexpr auto ivec2_t  = VK_FORMAT_R32G32_SINT;
        inline constexpr auto ivec3_t  = VK_FORMAT_R32G32B32_SINT;
        inline constexpr auto ivec4_t  = VK_FORMAT_R32G32B32A32_SINT;
        inline constexpr auto uint_t   = VK_FORMAT_R32_UINT;
        inline constexpr auto uvec2_t  = VK_FORMAT_R32G32_UINT;
        inline constexpr auto uvec3_t  = VK_FORMAT_R32G32B32_UINT;
        inline constexpr auto uvec4_t  = VK_FORMAT_R32G32B32A32_UINT;
        inline constexpr auto short_t  = VK_FORMAT_R16_SINT;
        inline constexpr auto svec2_t  = VK_FORMAT_R16G16_SINT;
        inline constexpr auto svec3_t  = VK_FORMAT_R16G16B16_SINT;
        inline constexpr auto svec4_t  = VK_FORMAT_R16G16B16A16_SINT;
        inline constexpr auto ushort_t = VK_FORMAT_R16_UINT;
        inline constexpr auto usvec2_t = VK_FORMAT_R16G16_UINT;
        inline constexpr auto usvec3_t = VK_FORMAT_R16G16B16_UINT;
        inline constexpr auto usvec4_t = VK_FORMAT_R16G16B16A16_UINT;
        inline constexpr auto byte_t   = VK_FORMAT_R8_SINT;
        inline constexpr auto bvec2_t  = VK_FORMAT_R8G8_SINT;
        inline constexpr auto bvec3_t  = VK_FORMAT_R8G8B8_SINT;
        inline constexpr auto bvec4_t  = VK_FORMAT_R8G8B8A8_SINT;
        inline constexpr auto ubyte_t  = VK_FORMAT_R8_UINT;
        inline constexpr auto ubvec2_t = VK_FORMAT_R8G8_UINT;
        inline constexpr auto ubvec3_t = VK_FORMAT_R8G8B8_UINT;
        inline constexpr auto ubvec4_t = VK_FORMAT_R8G8B8A8_UINT;
        inline constexpr auto double_t = VK_FORMAT_R64_SFLOAT;
        inline constexpr auto dvec2_t  = VK_FORMAT_R64G64_SFLOAT;
        inline constexpr auto dvec3_t  = VK_FORMAT_R64G64B64_SFLOAT;
        inline constexpr auto dvec4_t  = VK_FORMAT_R64G64B64A64_SFLOAT;
    } // namespace vertex_formats

    namespace vma_mem_usages
    {
        using enum_t = VmaMemoryUsage;

        /** No intended memory usage specified.
        Use other members of VmaAllocationCreateInfo to specify your requirements.
        */
        inline constexpr auto unknown = VMA_MEMORY_USAGE_UNKNOWN;
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Prefers `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
        */
        inline constexpr auto gpu_only = VMA_MEMORY_USAGE_GPU_ONLY;
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT` and `VK_MEMORY_PROPERTY_HOST_COHERENT_BIT`.
        */
        inline constexpr auto cpu_only = VMA_MEMORY_USAGE_CPU_ONLY;
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT`, prefers `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
        */
        inline constexpr auto cpu_to_gpu = VMA_MEMORY_USAGE_CPU_TO_GPU;
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT`, prefers `VK_MEMORY_PROPERTY_HOST_CACHED_BIT`.
        */
        inline constexpr auto gpu_to_cpu = VMA_MEMORY_USAGE_GPU_TO_CPU;
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Prefers not `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
        */
        inline constexpr auto cpu_copy = VMA_MEMORY_USAGE_CPU_COPY;
        /**
        Lazily allocated GPU memory having `VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT`.
        Exists mostly on mobile platforms. Using it on desktop PC or other GPUs with no such memory type present will fail the allocation.

        Usage: Memory for transient attachment images (color attachments, depth attachments etc.), created with `VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT`.

        Allocations with this usage are always created as dedicated - it implies #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT.
        */
        inline constexpr auto gpu_lazily_allocated = VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED;
        /**
        Selects best memory type automatically.
        This flag is recommended for most common use cases.

        When using this flag, if you want to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT),
        you must pass one of the flags: #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
        in VmaAllocationCreateInfo::flags.

        It can be used only with functions that let the library know `VkBufferCreateInfo` or `VkImageCreateInfo`, e.g.
        vmaCreateBuffer(), vmaCreateImage(), vmaFindMemoryTypeIndexForBufferInfo(), vmaFindMemoryTypeIndexForImageInfo()
        and not with generic memory allocation functions.
        */
        inline constexpr auto usage_auto = VMA_MEMORY_USAGE_AUTO;
        /**
        Selects best memory type automatically with preference for GPU (device) memory.

        When using this flag, if you want to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT),
        you must pass one of the flags: #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
        in VmaAllocationCreateInfo::flags.

        It can be used only with functions that let the library know `VkBufferCreateInfo` or `VkImageCreateInfo`, e.g.
        vmaCreateBuffer(), vmaCreateImage(), vmaFindMemoryTypeIndexForBufferInfo(), vmaFindMemoryTypeIndexForImageInfo()
        and not with generic memory allocation functions.
        */
        inline constexpr auto auto_prefer_device = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        /**
        Selects best memory type automatically with preference for CPU (host) memory.

        When using this flag, if you want to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT),
        you must pass one of the flags: #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
        in VmaAllocationCreateInfo::flags.

        It can be used only with functions that let the library know `VkBufferCreateInfo` or `VkImageCreateInfo`, e.g.
        vmaCreateBuffer(), vmaCreateImage(), vmaFindMemoryTypeIndexForBufferInfo(), vmaFindMemoryTypeIndexForImageInfo()
        and not with generic memory allocation functions.
        */
        inline constexpr auto auto_prefer_host = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
    } // namespace vma_mem_usages

    namespace vma_alloc_flags
    {
        using enum_t = VmaAllocationCreateFlagBits;

        /** \brief Set this flag if the allocation should have its own memory block.

        Use it for special, big resources, like fullscreen images used as attachments.

        If you use this flag while creating a buffer or an image, `VkMemoryDedicatedAllocateInfo`
        structure is applied if possible.
        */
        inline constexpr auto dedicated_memory = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

        /** \brief Set this flag to only try to allocate from existing `VkDeviceMemory` blocks and never create new such block.

        If new allocation cannot be placed in any of the existing blocks, allocation
        fails with `VK_ERROR_OUT_OF_DEVICE_MEMORY` error.

        You should not use #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT and
        #VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT at the same time. It makes no sense.
        */
        inline constexpr auto never_allocate = VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT;
        /** \brief Set this flag to use a memory that will be persistently mapped and retrieve pointer to it.

        Pointer to mapped memory will be returned through VmaAllocationInfo::pMappedData.

        It is valid to use this flag for allocation made from memory type that is not
        `HOST_VISIBLE`. This flag is then ignored and memory is not mapped. This is
        useful if you need an allocation that is efficient to use on GPU
        (`DEVICE_LOCAL`) and still want to map it directly if possible on platforms that
        support it (e.g. Intel GPU).
        */
        inline constexpr auto mapped = VMA_ALLOCATION_CREATE_MAPPED_BIT;
        /** \deprecated Preserved for backward compatibility. Consider using vmaSetAllocationName() instead.

        Set this flag to treat VmaAllocationCreateInfo::pUserData as pointer to a
        null-terminated string. Instead of copying pointer value, a local copy of the
        string is made and stored in allocation's `pName`. The string is automatically
        freed together with the allocation. It is also used in vmaBuildStatsString().
        */
        inline constexpr auto user_data_copy_string = VMA_ALLOCATION_CREATE_USER_DATA_COPY_STRING_BIT;
        /** Allocation will be created from upper stack in a double stack pool.

        This flag is only allowed for custom pools created with #VMA_POOL_CREATE_LINEAR_ALGORITHM_BIT flag.
        */
        inline constexpr auto upper_address = VMA_ALLOCATION_CREATE_UPPER_ADDRESS_BIT;
        /** Create both buffer/image and allocation, but don't bind them together.
        It is useful when you want to bind yourself to do some more advanced binding, e.g. using some extensions.
        The flag is meaningful only with functions that bind by default: vmaCreateBuffer(), vmaCreateImage().
        Otherwise it is ignored.

        If you want to make sure the new buffer/image is not tied to the new memory allocation
        through `VkMemoryDedicatedAllocateInfoKHR` structure in case the allocation ends up in its own memory block,
        use also flag #VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT.
        */
        inline constexpr auto dont_bind = VMA_ALLOCATION_CREATE_DONT_BIND_BIT;
        /** Create allocation only if additional device memory required for it, if any, won't exceed
        memory budget. Otherwise return `VK_ERROR_OUT_OF_DEVICE_MEMORY`.
        */
        inline constexpr auto within_budget = VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT;
        /** \brief Set this flag if the allocated memory will have aliasing resources.

        Usage of this flag prevents supplying `VkMemoryDedicatedAllocateInfoKHR` when #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT is specified.
        Otherwise created dedicated memory will not be suitable for aliasing resources, resulting in Vulkan Validation Layer errors.
        */
        inline constexpr auto can_alias = VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT;
        /**
        Requests possibility to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT).

        - If you use #VMA_MEMORY_USAGE_AUTO or other `VMA_MEMORY_USAGE_AUTO*` value,
          you must use this flag to be able to map the allocation. Otherwise, mapping is incorrect.
        - If you use other value of #VmaMemoryUsage, this flag is ignored and mapping is always possible in memory types that are `HOST_VISIBLE`.
          This includes allocations created in \ref custom_memory_pools.

        Declares that mapped memory will only be written sequentially, e.g. using `memcpy()` or a loop writing number-by-number,
        never read or accessed randomly, so a memory type can be selected that is uncached and write-combined.

        \warning Violating this declaration may work correctly, but will likely be very slow.
        Watch out for implicit reads introduced by doing e.g. `pMappedData[i] += x;`
        Better prepare your data in a local variable and `memcpy()` it to the mapped pointer all at once.
        */
        inline constexpr auto host_access_sequential_write = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        /**
        Requests possibility to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT).

        - If you use #VMA_MEMORY_USAGE_AUTO or other `VMA_MEMORY_USAGE_AUTO*` value,
          you must use this flag to be able to map the allocation. Otherwise, mapping is incorrect.
        - If you use other value of #VmaMemoryUsage, this flag is ignored and mapping is always possible in memory types that are `HOST_VISIBLE`.
          This includes allocations created in \ref custom_memory_pools.

        Declares that mapped memory can be read, written, and accessed in random order,
        so a `HOST_CACHED` memory type is preferred.
        */
        inline constexpr auto host_access_random = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
        /**
        Together with #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
        it says that despite request for host access, a not-`HOST_VISIBLE` memory type can be selected
        if it may improve performance.

        By using this flag, you declare that you will check if the allocation ended up in a `HOST_VISIBLE` memory type
        (e.g. using vmaGetAllocationMemoryProperties()) and if not, you will create some "staging" buffer and
        issue an explicit transfer to write/read your data.
        To prepare for this possibility, don't forget to add appropriate flags like
        `VK_BUFFER_USAGE_TRANSFER_DST_BIT`, `VK_BUFFER_USAGE_TRANSFER_SRC_BIT` to the parameters of created buffer or image.
        */
        inline constexpr auto host_access_allow_transfer_instead = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT;
        /** Allocation strategy that chooses smallest possible free range for the allocation
        to minimize memory usage and fragmentation, possibly at the expense of allocation time.
        */
        inline constexpr auto strategy_min_memory = VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT;
        /** Allocation strategy that chooses first suitable free range for the allocation -
        not necessarily in terms of the smallest offset but the one that is easiest and fastest to find
        to minimize allocation time, possibly at the expense of allocation quality.
        */
        inline constexpr auto strategy_min_time = VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT;
        /** Allocation strategy that chooses always the lowest offset in available space.
        This is not the most efficient strategy but achieves highly packed data.
        Used internally by defragmentation, not recommended in typical usage.
        */
        inline constexpr auto strategy_min_offset = VMA_ALLOCATION_CREATE_STRATEGY_MIN_OFFSET_BIT;
        /** Alias to #VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT.
         */
        inline constexpr auto strategy_best_fit = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
        /** Alias to #VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT.
         */
        inline constexpr auto strategy_first_fit = VMA_ALLOCATION_CREATE_STRATEGY_FIRST_FIT_BIT;
        /** A bit mask to extract only `STRATEGY` bits from entire set of flags.
         */
        inline constexpr auto strategy_mask = VMA_ALLOCATION_CREATE_STRATEGY_MASK;
    } // namespace vma_alloc_flags

    namespace buffer_usage_flags
    {
        using enum_t = VkBufferUsageFlags;

        inline constexpr auto transfer_source      = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        inline constexpr auto transfer_destination = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        inline constexpr auto uniform_texel_buffer = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
        inline constexpr auto storage_texel_buffer = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
        inline constexpr auto uniform_buffer       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        inline constexpr auto storage_buffer       = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        inline constexpr auto index_buffer         = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        inline constexpr auto vertex_buffer        = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        inline constexpr auto indirect_buffer      = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    } // namespace buffer_usage_flags

    namespace structs
    {
        namespace create
        {
            [[nodiscard]] inline auto instance() -> VkInstanceCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                    .pNext = nullptr,
                };
            }

            [[nodiscard]] inline auto application_info() -> VkApplicationInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pNext = nullptr,
                };
            }

            [[nodiscard]] inline auto debug_report_callback() -> VkDebugReportCallbackCreateInfoEXT
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
                    .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT
                           | VK_DEBUG_REPORT_WARNING_BIT_EXT
                           | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
                    .pUserData = nullptr,
                };
            }

            [[nodiscard]] inline auto debug_utils() -> VkDebugUtilsMessengerCreateInfoEXT
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,

                    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,

                    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

                    .pUserData = nullptr,
                };
            }

            [[nodiscard]] inline auto device_queue() -> VkDeviceQueueCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                };
            }

            [[nodiscard]] inline auto device() -> VkDeviceCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                };
            }

            [[nodiscard]] inline auto desc_pool() -> VkDescriptorPoolCreateInfo
            {
                return {
                    .sType   = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                    .maxSets = 1,
                };
            }

            [[nodiscard]] inline auto desc_set_layout() -> VkDescriptorSetLayoutCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                };
            }

            [[nodiscard]] inline auto desc_sets(VkDescriptorPool pool) -> VkDescriptorSetAllocateInfo
            {
                return {
                    .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                    .descriptorPool = pool,
                };
            }

            [[nodiscard]] inline auto swapchain() -> VkSwapchainCreateInfoKHR
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                };
            }

            [[nodiscard]] inline auto image() -> VkImageCreateInfo
            {
                return {
                    .sType     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                    .imageType = image_types::_2d,
                };
            }

            [[nodiscard]] inline auto image_view() -> VkImageViewCreateInfo
            {
                return {
                    .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                    .viewType   = image_view_types::type_2d,
                    .components = {
                                   .r = component_swizzles::r,
                                   .g = component_swizzles::g,
                                   .b = component_swizzles::b,
                                   .a = component_swizzles::a,
                                   },
                    .subresourceRange = { image_aspect_flags::color, 0, 1, 0, 1 },
                };
            }

            [[nodiscard]] inline auto framebuffer() -> VkFramebufferCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                };
            }

            [[nodiscard]] inline auto descriptor_pool() -> VkDescriptorPoolCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                };
            }

            [[nodiscard]] inline auto render_pass() -> VkRenderPassCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                };
            }

            [[nodiscard]] inline auto semaphore() -> VkSemaphoreCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                };
            }

            [[nodiscard]] inline auto fence() -> VkFenceCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                    .flags = VK_FENCE_CREATE_SIGNALED_BIT,
                };
            }

            [[nodiscard]] inline auto cmd_buffer() -> VkCommandBufferAllocateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                };
            }

            [[nodiscard]] inline auto cmd_pool() -> VkCommandPoolCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                };
            }

            [[nodiscard]] inline auto allocator() -> VmaAllocatorCreateInfo
            {
                return {};
            }

            [[nodiscard]] inline auto allocation() -> VmaAllocationCreateInfo
            {
                return {};
            }

            [[nodiscard]] inline auto shader_module() -> VkShaderModuleCreateInfo
            {
                return {
                    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                };
            }
        } // namespace create

        [[nodiscard]] inline auto cmd_buffer_begin() -> VkCommandBufferBeginInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            };
        }

        [[nodiscard]] inline auto one_time_cmd_buffer_begin() -> VkCommandBufferBeginInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            };
        }

        [[nodiscard]] inline auto render_pass_begin() -> VkRenderPassBeginInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            };
        }

        [[nodiscard]] inline auto submit() -> VkSubmitInfo
        {
            return {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            };
        }

        [[nodiscard]] inline auto present() -> VkPresentInfoKHR
        {
            return {
                .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            };
        }
    } // namespace structs

} // namespace orb::vk
