#pragma once

#include <orb/assert.hpp>

#include <array>
#include <vulkan/vulkan_core.h>

namespace orb::vk
{
    namespace khr_extensions
    {
        inline constexpr const char* device_properties_2 = //
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;

        inline constexpr const char* portability_enumeration = //
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;

        inline constexpr const char* surface       = "VK_KHR_surface";
        inline constexpr const char* win32_surface = "VK_KHR_win32_surface";
        inline constexpr const char* swapchain     = "VK_KHR_swapchain";

    } // namespace khr_extensions

    namespace extensions
    {
        inline constexpr const char* debug_report = "VK_EXT_debug_report";
    }

    namespace validation_layers
    {
        inline constexpr const char* validation = "VK_LAYER_KHRONOS_validation";
    }

    namespace gpu_types

    {
        using namespace std::literals;
        using enum_t                            = VkPhysicalDeviceType;
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
        using enum_t                   = VkQueueFlagBits;
        inline constexpr auto graphics = VK_QUEUE_GRAPHICS_BIT;
        inline constexpr auto transfer = VK_QUEUE_TRANSFER_BIT;
        inline constexpr auto compute  = VK_QUEUE_COMPUTE_BIT;
    } // namespace queue_families

    namespace instance_create
    {
        using enum_t                      = VkInstanceCreateFlagBits;
        inline constexpr auto portability = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    } // namespace instance_create

    namespace proc_addresses
    {
        using create_debug_report_callback = PFN_vkCreateDebugReportCallbackEXT;
        inline auto create_deb_callback(VkInstance instance)
        {
            auto fn = (create_debug_report_callback)vkGetInstanceProcAddr( // NOLINT
                instance,
                "vkCreateDebugReportCallbackEXT");
            orbassert(fn != nullptr, "Coult not retrieve vkCreateDebugReportCallbackEXT function");
            return fn;
        }

        inline auto destroy_deb_callback(VkInstance instance)
        {
            using destroy_debug_report_callback = PFN_vkDestroyDebugReportCallbackEXT;
            auto fn = (destroy_debug_report_callback)vkGetInstanceProcAddr( // NOLINT
                instance,
                "vkDestroyDebugReportCallbackEXT");
            orbassert(fn != nullptr, "Coult not retrieve vkDestroyDebugReportCallbackEXT function");
            return fn;
        }
    } // namespace proc_addresses

    namespace desc_types
    {
        using enum_t                                     = VkDescriptorType;
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

    namespace attachment_load_op
    {
        using enum_t                    = VkAttachmentLoadOp;
        inline constexpr auto load      = VK_ATTACHMENT_LOAD_OP_LOAD;
        inline constexpr auto clear     = VK_ATTACHMENT_LOAD_OP_CLEAR;
        inline constexpr auto dont_care = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        inline constexpr auto none      = VK_ATTACHMENT_LOAD_OP_NONE_EXT;
    } // namespace attachment_load_op

    namespace attachment_store_op
    {
        using enum_t                    = VkAttachmentStoreOp;
        inline constexpr auto store     = VK_ATTACHMENT_STORE_OP_STORE;
        inline constexpr auto dont_care = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        inline constexpr auto none      = VK_ATTACHMENT_STORE_OP_NONE;
    }; // namespace attachment_store_op

    namespace pipeline_bind_point
    {
        using enum_t                          = VkPipelineBindPoint;
        inline constexpr auto graphics        = VK_PIPELINE_BIND_POINT_GRAPHICS;
        inline constexpr auto compute         = VK_PIPELINE_BIND_POINT_COMPUTE;
        inline constexpr auto ray_tracing_khr = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
        inline constexpr auto ray_tracing_nv  = VK_PIPELINE_BIND_POINT_RAY_TRACING_NV;
    }; // namespace pipeline_bind_point

    namespace cmd_buffer_level
    {
        using enum_t                    = VkCommandBufferLevel;
        inline constexpr auto primary   = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        inline constexpr auto secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    }; // namespace cmd_buffer_level

    namespace access_flag
    {
        using enum_t                                         = VkAccessFlagBits;
        inline constexpr auto indirect_command_read          = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
        inline constexpr auto index_read                     = VK_ACCESS_INDEX_READ_BIT;
        inline constexpr auto vertex_attribute_read          = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
        inline constexpr auto uniform_read                   = VK_ACCESS_UNIFORM_READ_BIT;
        inline constexpr auto input_attachment_read          = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        inline constexpr auto shader_read                    = VK_ACCESS_SHADER_READ_BIT;
        inline constexpr auto shader_write                   = VK_ACCESS_SHADER_WRITE_BIT;
        inline constexpr auto color_attachment_read          = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        inline constexpr auto color_attachment_write         = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        inline constexpr auto depth_stencil_attachment_read  = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        inline constexpr auto depth_stencil_attachment_write = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        inline constexpr auto transfer_read                  = VK_ACCESS_TRANSFER_READ_BIT;
        inline constexpr auto transfer_write                 = VK_ACCESS_TRANSFER_WRITE_BIT;
        inline constexpr auto host_read                      = VK_ACCESS_HOST_READ_BIT;
        inline constexpr auto host_write                     = VK_ACCESS_HOST_WRITE_BIT;
        inline constexpr auto memory_read                    = VK_ACCESS_MEMORY_READ_BIT;
        inline constexpr auto memory_write                   = VK_ACCESS_MEMORY_WRITE_BIT;
        inline constexpr auto none                           = VK_ACCESS_NONE;
        inline constexpr auto transform_feedback_write_ext   = VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT;
        inline constexpr auto transform_feedback_counter_read_ext =
            VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT;
        inline constexpr auto transform_feedback_counter_write_ext =
            VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT;
        inline constexpr auto conditional_rendering_read_ext = VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT;
        inline constexpr auto color_attachment_read_noncoherent_ext =
            VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT;
        inline constexpr auto acceleration_structure_read_khr = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;
        inline constexpr auto acceleration_structure_write_khr =
            VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
        inline constexpr auto fragment_density_map_read_ext = VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT;
        inline constexpr auto fragment_shading_rate_attachment_read_khr =
            VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR;
        inline constexpr auto command_preprocess_read_nv      = VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV;
        inline constexpr auto command_preprocess_write_nv     = VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV;
        inline constexpr auto shading_rate_image_read_nv      = VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV;
        inline constexpr auto acceleration_structure_read_nv  = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV;
        inline constexpr auto acceleration_structure_write_nv = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV;
        inline constexpr auto none_khr                        = VK_ACCESS_NONE_KHR;
    } // namespace access_flag

    namespace image_aspect_flag
    {
        using enum_t                             = VkImageAspectFlagBits;
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
    } // namespace image_aspect_flag

    namespace pipeline_stage_flag
    {
        using enum_t                                      = VkPipelineStageFlagBits;
        inline constexpr auto top_of_pipe                 = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        inline constexpr auto draw_indirect               = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
        inline constexpr auto vertex_input                = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
        inline constexpr auto vertex_shader               = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
        inline constexpr auto tessellation_control_shader = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
        inline constexpr auto tessellation_evaluation_shader =
            VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
        inline constexpr auto geometry_shader           = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
        inline constexpr auto fragment_shader           = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        inline constexpr auto early_fragment_tests      = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        inline constexpr auto late_fragment_tests       = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        inline constexpr auto color_attachment_output   = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        inline constexpr auto compute_shader            = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        inline constexpr auto transfer                  = VK_PIPELINE_STAGE_TRANSFER_BIT;
        inline constexpr auto bottom_of_pipe            = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        inline constexpr auto host                      = VK_PIPELINE_STAGE_HOST_BIT;
        inline constexpr auto all_graphics              = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
        inline constexpr auto all_commands              = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        inline constexpr auto none                      = VK_PIPELINE_STAGE_NONE;
        inline constexpr auto transform_feedback_ext    = VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT;
        inline constexpr auto conditional_rendering_ext = VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT;
        inline constexpr auto acceleration_structure_build_khr =
            VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
        inline constexpr auto ray_tracing_shader_khr = VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR;
        inline constexpr auto fragment_density_process_ext =
            VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT;
        inline constexpr auto fragment_shading_rate_attachment_khr =
            VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
        inline constexpr auto command_preprocess_nv = VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV;
        inline constexpr auto task_shader_ext       = VK_PIPELINE_STAGE_TASK_SHADER_BIT_EXT;
        inline constexpr auto mesh_shader_ext       = VK_PIPELINE_STAGE_MESH_SHADER_BIT_EXT;
        inline constexpr auto shading_rate_image_nv = VK_PIPELINE_STAGE_SHADING_RATE_IMAGE_BIT_NV;
        inline constexpr auto ray_tracing_shader_nv = VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV;
        inline constexpr auto acceleration_structure_build_nv =
            VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV;
        inline constexpr auto task_shader_nv = VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV;
        inline constexpr auto mesh_shader_nv = VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV;
        inline constexpr auto none_khr       = VK_PIPELINE_STAGE_NONE_KHR;
    } // namespace pipeline_stage_flag

    namespace vkres
    {
        using enum_t                                             = VkResult;
        inline constexpr auto success                            = VK_SUCCESS;
        inline constexpr auto not_ready                          = VK_NOT_READY;
        inline constexpr auto timeout                            = VK_TIMEOUT;
        inline constexpr auto event_set                          = VK_EVENT_SET;
        inline constexpr auto event_reset                        = VK_EVENT_RESET;
        inline constexpr auto incomplete                         = VK_INCOMPLETE;
        inline constexpr auto err_out_of_host_memory             = VK_ERROR_OUT_OF_HOST_MEMORY;
        inline constexpr auto err_out_of_device_memory           = VK_ERROR_OUT_OF_DEVICE_MEMORY;
        inline constexpr auto err_initialization_failed          = VK_ERROR_INITIALIZATION_FAILED;
        inline constexpr auto err_device_lost                    = VK_ERROR_DEVICE_LOST;
        inline constexpr auto err_memory_map_failed              = VK_ERROR_MEMORY_MAP_FAILED;
        inline constexpr auto err_layer_not_present              = VK_ERROR_LAYER_NOT_PRESENT;
        inline constexpr auto err_extension_not_present          = VK_ERROR_EXTENSION_NOT_PRESENT;
        inline constexpr auto err_feature_not_present            = VK_ERROR_FEATURE_NOT_PRESENT;
        inline constexpr auto err_incompatible_driver            = VK_ERROR_INCOMPATIBLE_DRIVER;
        inline constexpr auto err_too_many_objects               = VK_ERROR_TOO_MANY_OBJECTS;
        inline constexpr auto err_format_not_supported           = VK_ERROR_FORMAT_NOT_SUPPORTED;
        inline constexpr auto err_fragmented_pool                = VK_ERROR_FRAGMENTED_POOL;
        inline constexpr auto err_unknown                        = VK_ERROR_UNKNOWN;
        inline constexpr auto err_out_of_pool_memory             = VK_ERROR_OUT_OF_POOL_MEMORY;
        inline constexpr auto err_invalid_external_handle        = VK_ERROR_INVALID_EXTERNAL_HANDLE;
        inline constexpr auto err_fragmentation                  = VK_ERROR_FRAGMENTATION;
        inline constexpr auto err_invalid_opaque_capture_address = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS;
        inline constexpr auto pipeline_compile_required          = VK_PIPELINE_COMPILE_REQUIRED;
        inline constexpr auto err_surface_lost_khr               = VK_ERROR_SURFACE_LOST_KHR;
        inline constexpr auto err_native_window_in_use_khr       = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR;
        inline constexpr auto suboptimal_khr                     = VK_SUBOPTIMAL_KHR;
        inline constexpr auto err_out_of_date_khr                = VK_ERROR_OUT_OF_DATE_KHR;
        inline constexpr auto err_incompatible_display_khr       = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR;
        inline constexpr auto err_validation_failed_ext          = VK_ERROR_VALIDATION_FAILED_EXT;
        inline constexpr auto err_invalid_shader_nv              = VK_ERROR_INVALID_SHADER_NV;
        inline constexpr auto err_image_usage_not_supported_khr  = VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR;
        inline constexpr auto err_video_picture_layout_not_supported_khr =
            VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR;
        inline constexpr auto err_video_profile_operation_not_supported_khr =
            VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR;
        inline constexpr auto err_video_profile_format_not_supported_khr =
            VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR;
        inline constexpr auto err_video_profile_codec_not_supported_khr =
            VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR;
        inline constexpr auto err_video_std_version_not_supported_khr =
            VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR;
        inline constexpr auto err_invalid_drm_format_modifier_plane_layout_ext =
            VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT;
        inline constexpr auto err_not_permitted_khr = VK_ERROR_NOT_PERMITTED_KHR;
        inline constexpr auto err_full_screen_exclusive_mode_lost_ext =
            VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT;
        inline constexpr auto thread_idle_khr                 = VK_THREAD_IDLE_KHR;
        inline constexpr auto thread_done_khr                 = VK_THREAD_DONE_KHR;
        inline constexpr auto operation_deferred_khr          = VK_OPERATION_DEFERRED_KHR;
        inline constexpr auto operation_not_deferred_khr      = VK_OPERATION_NOT_DEFERRED_KHR;
        inline constexpr auto err_compression_exhausted_ext   = VK_ERROR_COMPRESSION_EXHAUSTED_EXT;
        inline constexpr auto err_out_of_pool_memory_khr      = VK_ERROR_OUT_OF_POOL_MEMORY_KHR;
        inline constexpr auto err_invalid_external_handle_khr = VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR;
        inline constexpr auto err_fragmentation_ext           = VK_ERROR_FRAGMENTATION_EXT;
        inline constexpr auto err_not_permitted_ext           = VK_ERROR_NOT_PERMITTED_EXT;
        inline constexpr auto err_invalid_device_address_ext  = VK_ERROR_INVALID_DEVICE_ADDRESS_EXT;
        inline constexpr auto err_invalid_opaque_capture_address_khr =
            VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR;
        inline constexpr auto pipeline_compile_required_ext     = VK_PIPELINE_COMPILE_REQUIRED_EXT;
        inline constexpr auto err_pipeline_compile_required_ext = VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT;
    } // namespace vkres

    namespace formats
    {
        using enum_t                                     = VkFormat;
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
        inline constexpr auto r10x6g10x6b10x6a10x6_unorm_4pack16 =
            VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16;
        inline constexpr auto g10x6b10x6g10x6r10x6_422_unorm_4pack16 =
            VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
        inline constexpr auto b10x6g10x6r10x6g10x6_422_unorm_4pack16 =
            VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_420_unorm_3pack16 =
            VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
        inline constexpr auto g10x6_b10x6r10x6_2plane_420_unorm_3pack16 =
            VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_422_unorm_3pack16 =
            VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
        inline constexpr auto g10x6_b10x6r10x6_2plane_422_unorm_3pack16 =
            VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_444_unorm_3pack16 =
            VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
        inline constexpr auto r12x4_unorm_pack16       = VK_FORMAT_R12X4_UNORM_PACK16;
        inline constexpr auto r12x4g12x4_unorm_2pack16 = VK_FORMAT_R12X4G12X4_UNORM_2PACK16;
        inline constexpr auto r12x4g12x4b12x4a12x4_unorm_4pack16 =
            VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16;
        inline constexpr auto g12x4b12x4g12x4r12x4_422_unorm_4pack16 =
            VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
        inline constexpr auto b12x4g12x4r12x4g12x4_422_unorm_4pack16 =
            VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_420_unorm_3pack16 =
            VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4r12x4_2plane_420_unorm_3pack16 =
            VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_422_unorm_3pack16 =
            VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4r12x4_2plane_422_unorm_3pack16 =
            VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_444_unorm_3pack16 =
            VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;
        inline constexpr auto g16b16g16r16_422_unorm       = VK_FORMAT_G16B16G16R16_422_UNORM;
        inline constexpr auto b16g16r16g16_422_unorm       = VK_FORMAT_B16G16R16G16_422_UNORM;
        inline constexpr auto g16_b16_r16_3plane_420_unorm = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM;
        inline constexpr auto g16_b16r16_2plane_420_unorm  = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM;
        inline constexpr auto g16_b16_r16_3plane_422_unorm = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM;
        inline constexpr auto g16_b16r16_2plane_422_unorm  = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM;
        inline constexpr auto g16_b16_r16_3plane_444_unorm = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM;
        inline constexpr auto g8_b8r8_2plane_444_unorm     = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM;
        inline constexpr auto g10x6_b10x6r10x6_2plane_444_unorm_3pack16 =
            VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16;
        inline constexpr auto g12x4_b12x4r12x4_2plane_444_unorm_3pack16 =
            VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16;
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
        inline constexpr auto r10x6g10x6b10x6a10x6_unorm_4pack16_khr =
            VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR;
        inline constexpr auto g10x6b10x6g10x6r10x6_422_unorm_4pack16_khr =
            VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR;
        inline constexpr auto b10x6g10x6r10x6g10x6_422_unorm_4pack16_khr =
            VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_420_unorm_3pack16_khr =
            VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6r10x6_2plane_420_unorm_3pack16_khr =
            VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_422_unorm_3pack16_khr =
            VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6r10x6_2plane_422_unorm_3pack16_khr =
            VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR;
        inline constexpr auto g10x6_b10x6_r10x6_3plane_444_unorm_3pack16_khr =
            VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR;
        inline constexpr auto r12x4_unorm_pack16_khr       = VK_FORMAT_R12X4_UNORM_PACK16_KHR;
        inline constexpr auto r12x4g12x4_unorm_2pack16_khr = VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR;
        inline constexpr auto r12x4g12x4b12x4a12x4_unorm_4pack16_khr =
            VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR;
        inline constexpr auto g12x4b12x4g12x4r12x4_422_unorm_4pack16_khr =
            VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR;
        inline constexpr auto b12x4g12x4r12x4g12x4_422_unorm_4pack16_khr =
            VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_420_unorm_3pack16_khr =
            VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR;
        inline constexpr auto g12x4_b12x4r12x4_2plane_420_unorm_3pack16_khr =
            VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_422_unorm_3pack16_khr =
            VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR;
        inline constexpr auto g12x4_b12x4r12x4_2plane_422_unorm_3pack16_khr =
            VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR;
        inline constexpr auto g12x4_b12x4_r12x4_3plane_444_unorm_3pack16_khr =
            VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR;
        inline constexpr auto g16b16g16r16_422_unorm_khr       = VK_FORMAT_G16B16G16R16_422_UNORM_KHR;
        inline constexpr auto b16g16r16g16_422_unorm_khr       = VK_FORMAT_B16G16R16G16_422_UNORM_KHR;
        inline constexpr auto g16_b16_r16_3plane_420_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR;
        inline constexpr auto g16_b16r16_2plane_420_unorm_khr  = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR;
        inline constexpr auto g16_b16_r16_3plane_422_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR;
        inline constexpr auto g16_b16r16_2plane_422_unorm_khr  = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR;
        inline constexpr auto g16_b16_r16_3plane_444_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR;
        inline constexpr auto g8_b8r8_2plane_444_unorm_ext     = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT;
        inline constexpr auto g10x6_b10x6r10x6_2plane_444_unorm_3pack16_ext =
            VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT;
        inline constexpr auto g12x4_b12x4r12x4_2plane_444_unorm_3pack16_ext =
            VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT;
        inline constexpr auto g16_b16r16_2plane_444_unorm_ext = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT;
        inline constexpr auto a4r4g4b4_unorm_pack16_ext       = VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT;
        inline constexpr auto a4b4g4r4_unorm_pack16_ext       = VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT;
    } // namespace formats

    namespace present_modes
    {
        using enum_t                                        = VkPresentModeKHR;
        inline constexpr auto immediate_khr                 = VK_PRESENT_MODE_IMMEDIATE_KHR;
        inline constexpr auto mailbox_khr                   = VK_PRESENT_MODE_MAILBOX_KHR;
        inline constexpr auto fifo_khr                      = VK_PRESENT_MODE_FIFO_KHR;
        inline constexpr auto fifo_relaxed_khr              = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        inline constexpr auto shared_demand_refresh_khr     = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
        inline constexpr auto shared_continuous_refresh_khr = VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;
    } // namespace present_modes

    namespace color_spaces
    {
        using enum_t                                      = VkColorSpaceKHR;
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

    namespace image_usage_flag
    {
        using enum_t                                       = VkImageUsageFlagBits;
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
        inline constexpr auto fragment_shading_rate_attachment_khr =
            VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_dst_khr = VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_src_khr = VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
        inline constexpr auto video_encode_dpb_khr = VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR;
#endif
    } // namespace image_usage_flag

    namespace sharing_modes
    {
        using enum_t                     = VkSharingMode;
        inline constexpr auto exclusive  = VK_SHARING_MODE_EXCLUSIVE;
        inline constexpr auto concurrent = VK_SHARING_MODE_CONCURRENT;
    } // namespace sharing_modes

    namespace surface_transform_flag
    {
        using enum_t                                = VkSurfaceTransformFlagBitsKHR;
        inline constexpr auto identity_khr          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        inline constexpr auto rotate_90_khr         = VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
        inline constexpr auto rotate_180_khr        = VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
        inline constexpr auto rotate_270_khr        = VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
        inline constexpr auto horizontal_mirror_khr = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;
        inline constexpr auto inherit_khr           = VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;
        inline constexpr auto horizontal_mirror_rotate_90_khr =
            VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;
        inline constexpr auto horizontal_mirror_rotate_180_khr =
            VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;
        inline constexpr auto horizontal_mirror_rotate_270_khr =
            VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;
    } // namespace surface_transform_flag

    namespace composite_alpha_flag
    {
        using enum_t                              = VkCompositeAlphaFlagBitsKHR;
        inline constexpr auto opaque_khr          = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        inline constexpr auto pre_multiplied_khr  = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
        inline constexpr auto post_multiplied_khr = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
        inline constexpr auto inherit_khr         = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    } // namespace composite_alpha_flag

    namespace image_view_types
    {
        using enum_t                   = VkImageViewType;
        constexpr auto type_1d         = VK_IMAGE_VIEW_TYPE_1D;
        constexpr auto type_2d         = VK_IMAGE_VIEW_TYPE_2D;
        constexpr auto type_3d         = VK_IMAGE_VIEW_TYPE_3D;
        constexpr auto type_cube       = VK_IMAGE_VIEW_TYPE_CUBE;
        constexpr auto type_1d_array   = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        constexpr auto type_2d_array   = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        constexpr auto type_cube_array = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
    } // namespace image_view_types

    namespace component_swizzles
    {
        using enum_t            = VkComponentSwizzle;
        constexpr auto identity = VK_COMPONENT_SWIZZLE_IDENTITY;
        constexpr auto zero     = VK_COMPONENT_SWIZZLE_ZERO;
        constexpr auto one      = VK_COMPONENT_SWIZZLE_ONE;
        constexpr auto r        = VK_COMPONENT_SWIZZLE_R;
        constexpr auto g        = VK_COMPONENT_SWIZZLE_G;
        constexpr auto b        = VK_COMPONENT_SWIZZLE_B;
        constexpr auto a        = VK_COMPONENT_SWIZZLE_A;
    } // namespace component_swizzles

} // namespace orb::vk
