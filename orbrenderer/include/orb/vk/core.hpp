#pragma once

#include "orb/vk/enums.hpp"
#include "orb/vk/vma.hpp"

#include <orb/assert.hpp>

#include <array>
#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan_core.h>

namespace orb::vk
{
    namespace khr_extensions
    {
        inline constexpr const char* device_properties_2     = "VK_KHR_get_physical_device_properties2";
        inline constexpr const char* portability_enumeration = "VK_KHR_portability_enumeration";
        inline constexpr const char* surface                 = "VK_KHR_surface";
        inline constexpr const char* win32_surface           = "VK_KHR_win32_surface";
        inline constexpr const char* swapchain               = "VK_KHR_swapchain";
        inline constexpr const char* buffer_device_address   = "VK_KHR_buffer_device_address";
    } // namespace khr_extensions

    namespace extensions
    {
        inline constexpr const char* debug_report = "VK_EXT_debug_report";
        inline constexpr const char* debug_utils  = "VK_EXT_debug_utils";
    } // namespace extensions

    namespace validation_layers
    {
        inline constexpr const char* validation = "VK_LAYER_KHRONOS_validation";
    } // namespace validation_layers

    template <typename T>
    concept is_vk_native_flag = std::is_same_v<T, VkAccessFlags>
                             || std::is_same_v<T, VkQueueFlags>
                             || std::is_same_v<T, VkInstanceCreateFlags>
                             || std::is_same_v<T, VkImageAspectFlags>
                             || std::is_same_v<T, VkPipelineStageFlags>
                             || std::is_same_v<T, VkImageUsageFlags>
                             || std::is_same_v<T, VkSurfaceTransformFlagsKHR>
                             || std::is_same_v<T, VkCompositeAlphaFlagsKHR>
                             || std::is_same_v<T, VkSampleCountFlags>
                             || std::is_same_v<T, VkDescriptorPoolCreateFlags>
                             || std::is_same_v<T, VkCommandPoolCreateFlags>
                             || std::is_same_v<T, VkMemoryPropertyFlags>
                             || std::is_same_v<T, VmaAllocationCreateFlags>
                             || std::is_same_v<T, VkDebugUtilsMessageSeverityFlagsEXT>
                             || std::is_same_v<T, VkDebugUtilsMessageTypeFlagsEXT>
                             || std::is_same_v<T, VkCommandBufferUsageFlags>
                             || std::is_same_v<T, VkSubpassDescriptionFlags>
                             || std::is_same_v<T, VkShaderStageFlags>
                             || std::is_same_v<T, VkColorComponentFlags>
                             || std::is_same_v<T, VkBufferUsageFlags>;

    template <typename T>
    concept is_vk_native_enum = std::is_same_v<T, VkPhysicalDeviceType>
                             || std::is_same_v<T, VkDescriptorType>
                             || std::is_same_v<T, VkAttachmentLoadOp>
                             || std::is_same_v<T, VkAttachmentStoreOp>
                             || std::is_same_v<T, VkPipelineBindPoint>
                             || std::is_same_v<T, VkCommandBufferLevel>
                             || std::is_same_v<T, VkFormat>
                             || std::is_same_v<T, VkPresentModeKHR>
                             || std::is_same_v<T, VkColorSpaceKHR>
                             || std::is_same_v<T, VkSharingMode>
                             || std::is_same_v<T, VkImageTiling>
                             || std::is_same_v<T, VkImageType>
                             || std::is_same_v<T, VkImageViewType>
                             || std::is_same_v<T, VkComponentSwizzle>
                             || std::is_same_v<T, VkImageLayout>
                             || std::is_same_v<T, VmaMemoryUsage>
                             || std::is_same_v<T, VkFilter>
                             || std::is_same_v<T, VkDynamicState>
                             || std::is_same_v<T, VkPrimitiveTopology>
                             || std::is_same_v<T, VkPolygonMode>
                             || std::is_same_v<T, VkCullModeFlags>
                             || std::is_same_v<T, VkFrontFace>
                             || std::is_same_v<T, VkBlendFactor>
                             || std::is_same_v<T, VkBlendOp>
                             || std::is_same_v<T, shaderc_shader_kind>
                             || std::is_same_v<T, VkVertexInputRate>;

    template <typename T>
    concept is_vk_native_type = is_vk_native_flag<T> || is_vk_native_enum<T>;

    template <is_vktype T>
    struct orb_to_vk;

#define ORB_TO_VK(OrbT, VkT) \
    template <>              \
    struct orb_to_vk<OrbT>   \
    {                        \
        using type = VkT;    \
    }

    ORB_TO_VK(access_flag, VkAccessFlagBits);
    ORB_TO_VK(queue_family, VkQueueFlagBits);
    ORB_TO_VK(instance_create, VkInstanceCreateFlagBits);
    ORB_TO_VK(image_aspect_flag, VkImageAspectFlagBits);
    ORB_TO_VK(pipeline_stage_flag, VkPipelineStageFlagBits);
    ORB_TO_VK(image_usage_flag, VkImageUsageFlagBits);
    ORB_TO_VK(surface_transform_flag, VkSurfaceTransformFlagBitsKHR);
    ORB_TO_VK(composite_alpha_flag, VkCompositeAlphaFlagBitsKHR);
    ORB_TO_VK(sample_count_flag, VkSampleCountFlagBits);
    ORB_TO_VK(descriptor_pool_create_flag, VkDescriptorPoolCreateFlagBits);
    ORB_TO_VK(command_pool_create_flag, VkCommandPoolCreateFlagBits);
    ORB_TO_VK(memory_property_flag, VkMemoryPropertyFlagBits);
    ORB_TO_VK(memory_flag, VmaAllocationCreateFlagBits);
    ORB_TO_VK(debug_utils_message_severity_flag, VkDebugUtilsMessageSeverityFlagBitsEXT);
    ORB_TO_VK(debug_utils_message_type_flag, VkDebugUtilsMessageTypeFlagBitsEXT);
    ORB_TO_VK(command_buffer_usage_flag, VkCommandBufferUsageFlagBits);
    ORB_TO_VK(subpass_description_flag, VkSubpassDescriptionFlagBits);
    ORB_TO_VK(shader_stage_flag, VkShaderStageFlagBits);
    ORB_TO_VK(color_component, VkColorComponentFlagBits);
    ORB_TO_VK(buffer_usage_flag, VkBufferUsageFlagBits);
    ORB_TO_VK(gpu_type, VkPhysicalDeviceType);
    ORB_TO_VK(descriptor_type, VkDescriptorType);
    ORB_TO_VK(attachment_load_op, VkAttachmentLoadOp);
    ORB_TO_VK(attachment_store_op, VkAttachmentStoreOp);
    ORB_TO_VK(pipeline_bind_point, VkPipelineBindPoint);
    ORB_TO_VK(cmd_buffer_level, VkCommandBufferLevel);
    ORB_TO_VK(format, VkFormat);
    ORB_TO_VK(present_mode, VkPresentModeKHR);
    ORB_TO_VK(color_space, VkColorSpaceKHR);
    ORB_TO_VK(sharing_mode, VkSharingMode);
    ORB_TO_VK(image_tiling, VkImageTiling);
    ORB_TO_VK(image_type, VkImageType);
    ORB_TO_VK(image_view_type, VkImageViewType);
    ORB_TO_VK(component_swizzle, VkComponentSwizzle);
    ORB_TO_VK(image_layout, VkImageLayout);
    ORB_TO_VK(memory_usage, VmaMemoryUsage);
    ORB_TO_VK(filter, VkFilter);
    ORB_TO_VK(dynamic_state, VkDynamicState);
    ORB_TO_VK(primitive_topology, VkPrimitiveTopology);
    ORB_TO_VK(polygon_mode, VkPolygonMode);
    ORB_TO_VK(cull_mode, VkCullModeFlagBits);
    ORB_TO_VK(front_face, VkFrontFace);
    ORB_TO_VK(blend_factor, VkBlendFactor);
    ORB_TO_VK(blend_op, VkBlendOp);
    ORB_TO_VK(shader_kind, shaderc_shader_kind);
    ORB_TO_VK(vertex_input_rate, VkVertexInputRate);
    ORB_TO_VK(vertex_format, VkFormat);

    template <is_vktype T>
    inline constexpr auto vkenum(T e)
    {
        return static_cast<orb_to_vk<T>::type>(std::to_underlying(e));
    }

    template <is_vkflag T>
    inline constexpr auto vkflag(T e)
    {
        return static_cast<VkFlags>(std::to_underlying(e));
    }

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
                    .imageType = vkenum(image_type::_2d),
                };
            }

            [[nodiscard]] inline auto image_view() -> VkImageViewCreateInfo
            {
                return {
                    .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                    .viewType   = vkenum(image_view_type::_2d),
                    .components = {
                                   .r = vkenum(component_swizzle::r),
                                   .g = vkenum(component_swizzle::g),
                                   .b = vkenum(component_swizzle::b),
                                   .a = vkenum(component_swizzle::a) },
                    .subresourceRange = { vkenum(image_aspect_flag::color), 0, 1, 0, 1 },
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
