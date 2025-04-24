#include "orb/vk/enums.hpp"

#include <orb/vk/core.hpp>

namespace orb::vk
{
    // gpu_type
    static_assert(vkenum(gpu_type::other) == VK_PHYSICAL_DEVICE_TYPE_OTHER);
    static_assert(vkenum(gpu_type::integrated) == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
    static_assert(vkenum(gpu_type::discrete) == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
    static_assert(vkenum(gpu_type::virtual_gpu) == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU);
    static_assert(vkenum(gpu_type::cpu) == VK_PHYSICAL_DEVICE_TYPE_CPU);

    // queue_family
    static_assert(vkenum(queue_family::graphics) == VK_QUEUE_GRAPHICS_BIT);
    static_assert(vkenum(queue_family::compute) == VK_QUEUE_COMPUTE_BIT);
    static_assert(vkenum(queue_family::transfer) == VK_QUEUE_TRANSFER_BIT);
    static_assert(vkenum(queue_family::sparse) == VK_QUEUE_SPARSE_BINDING_BIT);
    static_assert(vkenum(queue_family::prot) == VK_QUEUE_PROTECTED_BIT);
    static_assert(vkenum(queue_family::video_decode) == VK_QUEUE_VIDEO_DECODE_BIT_KHR);

    // instance_create
    static_assert(vkenum(instance_create::portability) == VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);

    // descriptor_type
    static_assert(vkenum(descriptor_type::sampler) == VK_DESCRIPTOR_TYPE_SAMPLER);
    static_assert(vkenum(descriptor_type::combined_image_sampler) == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    static_assert(vkenum(descriptor_type::sampled_image) == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
    static_assert(vkenum(descriptor_type::storage_image) == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
    static_assert(vkenum(descriptor_type::uniform_texel_buffer) == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER);
    static_assert(vkenum(descriptor_type::storage_texel_buffer) == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER);
    static_assert(vkenum(descriptor_type::uniform_buffer) == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    static_assert(vkenum(descriptor_type::storage_buffer) == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
    static_assert(vkenum(descriptor_type::uniform_buffer_dynamic) == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC);
    static_assert(vkenum(descriptor_type::storage_buffer_dynamic) == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC);
    static_assert(vkenum(descriptor_type::input_attachment) == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT);
    static_assert(vkenum(descriptor_type::inline_uniform_block) == VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK);
    static_assert(vkenum(descriptor_type::acceleration_structure_khr) == VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR);
    static_assert(vkenum(descriptor_type::acceleration_structure_nv) == VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV);
    static_assert(vkenum(descriptor_type::sample_weight_image_qcom) == VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM);
    static_assert(vkenum(descriptor_type::block_match_image_qcom) == VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM);
    static_assert(vkenum(descriptor_type::mutable_ext) == VK_DESCRIPTOR_TYPE_MUTABLE_EXT);
    static_assert(vkenum(descriptor_type::inline_uniform_block_ext) == VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT);
    static_assert(vkenum(descriptor_type::mutable_valve) == VK_DESCRIPTOR_TYPE_MUTABLE_VALVE);

    // attachment_load_op
    static_assert(vkenum(attachment_load_op::load) == VK_ATTACHMENT_LOAD_OP_LOAD);
    static_assert(vkenum(attachment_load_op::clear) == VK_ATTACHMENT_LOAD_OP_CLEAR);
    static_assert(vkenum(attachment_load_op::dont_care) == VK_ATTACHMENT_LOAD_OP_DONT_CARE);
    static_assert(vkenum(attachment_load_op::none) == VK_ATTACHMENT_LOAD_OP_NONE_EXT);

    // attachment_store_op
    static_assert(vkenum(attachment_store_op::store) == VK_ATTACHMENT_STORE_OP_STORE);
    static_assert(vkenum(attachment_store_op::dont_care) == VK_ATTACHMENT_STORE_OP_DONT_CARE);
    static_assert(vkenum(attachment_store_op::none) == VK_ATTACHMENT_STORE_OP_NONE);

    // pipeline_bind_point
    static_assert(vkenum(pipeline_bind_point::graphics) == VK_PIPELINE_BIND_POINT_GRAPHICS);
    static_assert(vkenum(pipeline_bind_point::compute) == VK_PIPELINE_BIND_POINT_COMPUTE);
    static_assert(vkenum(pipeline_bind_point::ray_tracing_khr) == VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR);
    static_assert(vkenum(pipeline_bind_point::ray_tracing_nv) == VK_PIPELINE_BIND_POINT_RAY_TRACING_NV);

    // cmd_buffer_level
    static_assert(vkenum(cmd_buffer_level::primary) == VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    static_assert(vkenum(cmd_buffer_level::secondary) == VK_COMMAND_BUFFER_LEVEL_SECONDARY);

    // access_flag
    static_assert(vkenum(access_flag::none) == VK_ACCESS_NONE);
    static_assert(vkenum(access_flag::none_khr) == VK_ACCESS_NONE_KHR);
    static_assert(vkenum(access_flag::indirect_command_read) == VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    static_assert(vkenum(access_flag::index_read) == VK_ACCESS_INDEX_READ_BIT);
    static_assert(vkenum(access_flag::vertex_attribute_read) == VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    static_assert(vkenum(access_flag::uniform_read) == VK_ACCESS_UNIFORM_READ_BIT);
    static_assert(vkenum(access_flag::input_attachment_read) == VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    static_assert(vkenum(access_flag::shader_read) == VK_ACCESS_SHADER_READ_BIT);
    static_assert(vkenum(access_flag::shader_write) == VK_ACCESS_SHADER_WRITE_BIT);
    static_assert(vkenum(access_flag::color_attachment_read) == VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    static_assert(vkenum(access_flag::color_attachment_write) == VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    static_assert(vkenum(access_flag::depth_stencil_attachment_read) == VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    static_assert(vkenum(access_flag::depth_stencil_attachment_write) == VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    static_assert(vkenum(access_flag::transfer_read) == VK_ACCESS_TRANSFER_READ_BIT);
    static_assert(vkenum(access_flag::transfer_write) == VK_ACCESS_TRANSFER_WRITE_BIT);
    static_assert(vkenum(access_flag::host_read) == VK_ACCESS_HOST_READ_BIT);
    static_assert(vkenum(access_flag::host_write) == VK_ACCESS_HOST_WRITE_BIT);
    static_assert(vkenum(access_flag::memory_read) == VK_ACCESS_MEMORY_READ_BIT);
    static_assert(vkenum(access_flag::memory_write) == VK_ACCESS_MEMORY_WRITE_BIT);
    static_assert(vkenum(access_flag::transform_feedback_write_ext) == VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT);
    static_assert(vkenum(access_flag::transform_feedback_counter_read_ext) == VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);
    static_assert(vkenum(access_flag::transform_feedback_counter_write_ext) == VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT);
    static_assert(vkenum(access_flag::conditional_rendering_read_ext) == VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT);
    static_assert(vkenum(access_flag::color_attachment_read_noncoherent_ext) == VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT);
    static_assert(vkenum(access_flag::acceleration_structure_read_khr) == VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
    static_assert(vkenum(access_flag::acceleration_structure_write_khr) == VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
    static_assert(vkenum(access_flag::fragment_density_map_read_ext) == VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
    static_assert(vkenum(access_flag::fragment_shading_rate_attachment_read_khr) == VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR);
    static_assert(vkenum(access_flag::command_preprocess_read_nv) == VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV);
    static_assert(vkenum(access_flag::command_preprocess_write_nv) == VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV);
    static_assert(vkenum(access_flag::shading_rate_image_read_nv) == VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
    static_assert(vkenum(access_flag::acceleration_structure_read_nv) == VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV);
    static_assert(vkenum(access_flag::acceleration_structure_write_nv) == VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV);

    // image_aspect_flags
    static_assert(vkenum(image_aspect_flag::none) == VK_IMAGE_ASPECT_NONE);
    static_assert(vkenum(image_aspect_flag::none) == VK_IMAGE_ASPECT_NONE_KHR);
    static_assert(vkenum(image_aspect_flag::color) == VK_IMAGE_ASPECT_COLOR_BIT);
    static_assert(vkenum(image_aspect_flag::depth) == VK_IMAGE_ASPECT_DEPTH_BIT);
    static_assert(vkenum(image_aspect_flag::stencil) == VK_IMAGE_ASPECT_STENCIL_BIT);
    static_assert(vkenum(image_aspect_flag::metadata) == VK_IMAGE_ASPECT_METADATA_BIT);
    static_assert(vkenum(image_aspect_flag::plane_0) == VK_IMAGE_ASPECT_PLANE_0_BIT);
    static_assert(vkenum(image_aspect_flag::plane_0) == VK_IMAGE_ASPECT_PLANE_0_BIT_KHR);
    static_assert(vkenum(image_aspect_flag::plane_1) == VK_IMAGE_ASPECT_PLANE_1_BIT);
    static_assert(vkenum(image_aspect_flag::plane_1) == VK_IMAGE_ASPECT_PLANE_1_BIT_KHR);
    static_assert(vkenum(image_aspect_flag::plane_2) == VK_IMAGE_ASPECT_PLANE_2_BIT);
    static_assert(vkenum(image_aspect_flag::plane_2) == VK_IMAGE_ASPECT_PLANE_2_BIT_KHR);
    static_assert(vkenum(image_aspect_flag::memory_plane_0) == VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT);
    static_assert(vkenum(image_aspect_flag::memory_plane_1) == VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT);
    static_assert(vkenum(image_aspect_flag::memory_plane_2) == VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT);

    // pipeline_stage_flag
    static_assert(vkenum(pipeline_stage_flag::none) == VK_PIPELINE_STAGE_NONE);
    static_assert(vkenum(pipeline_stage_flag::none) == VK_PIPELINE_STAGE_NONE_KHR);
    static_assert(vkenum(pipeline_stage_flag::top_of_pipe) == VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    static_assert(vkenum(pipeline_stage_flag::draw_indirect) == VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT);
    static_assert(vkenum(pipeline_stage_flag::vertex_input) == VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
    static_assert(vkenum(pipeline_stage_flag::vertex_shader) == VK_PIPELINE_STAGE_VERTEX_SHADER_BIT);
    static_assert(vkenum(pipeline_stage_flag::tessellation_control_shader) == VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT);
    static_assert(vkenum(pipeline_stage_flag::tessellation_evaluation_shader) == VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT);
    static_assert(vkenum(pipeline_stage_flag::geometry_shader) == VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT);
    static_assert(vkenum(pipeline_stage_flag::fragment_shader) == VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
    static_assert(vkenum(pipeline_stage_flag::early_fragment_tests) == VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
    static_assert(vkenum(pipeline_stage_flag::late_fragment_tests) == VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT);
    static_assert(vkenum(pipeline_stage_flag::color_attachment_output) == VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    static_assert(vkenum(pipeline_stage_flag::compute_shader) == VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
    static_assert(vkenum(pipeline_stage_flag::transfer) == VK_PIPELINE_STAGE_TRANSFER_BIT);
    static_assert(vkenum(pipeline_stage_flag::bottom_of_pipe) == VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    static_assert(vkenum(pipeline_stage_flag::host) == VK_PIPELINE_STAGE_HOST_BIT);
    static_assert(vkenum(pipeline_stage_flag::all_graphics) == VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
    static_assert(vkenum(pipeline_stage_flag::all_commands) == VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
    static_assert(vkenum(pipeline_stage_flag::none) == VK_PIPELINE_STAGE_NONE);
    static_assert(vkenum(pipeline_stage_flag::transform_feedback_ext) == VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT);
    static_assert(vkenum(pipeline_stage_flag::conditional_rendering_ext) == VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT);
    static_assert(vkenum(pipeline_stage_flag::acceleration_structure_build_khr) == VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
    static_assert(vkenum(pipeline_stage_flag::ray_tracing_shader_khr) == VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR);
    static_assert(vkenum(pipeline_stage_flag::fragment_density_process_ext) == VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT);
    static_assert(vkenum(pipeline_stage_flag::fragment_shading_rate_attachment_khr) == VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR);
    static_assert(vkenum(pipeline_stage_flag::command_preprocess_nv) == VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV);
    static_assert(vkenum(pipeline_stage_flag::task_shader_ext) == VK_PIPELINE_STAGE_TASK_SHADER_BIT_EXT);
    static_assert(vkenum(pipeline_stage_flag::mesh_shader_ext) == VK_PIPELINE_STAGE_MESH_SHADER_BIT_EXT);
    static_assert(vkenum(pipeline_stage_flag::shading_rate_image_nv) == VK_PIPELINE_STAGE_SHADING_RATE_IMAGE_BIT_NV);
    static_assert(vkenum(pipeline_stage_flag::ray_tracing_shader_nv) == VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV);
    static_assert(vkenum(pipeline_stage_flag::acceleration_structure_build_nv) == VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV);
    static_assert(vkenum(pipeline_stage_flag::task_shader_nv) == VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV);
    static_assert(vkenum(pipeline_stage_flag::mesh_shader_nv) == VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV);

    // format
    static_assert(vkenum(format::undefined) == VK_FORMAT_UNDEFINED);
    static_assert(vkenum(format::r4g4_unorm_pack8) == VK_FORMAT_R4G4_UNORM_PACK8);
    static_assert(vkenum(format::r4g4b4a4_unorm_pack16) == VK_FORMAT_R4G4B4A4_UNORM_PACK16);
    static_assert(vkenum(format::b4g4r4a4_unorm_pack16) == VK_FORMAT_B4G4R4A4_UNORM_PACK16);
    static_assert(vkenum(format::r5g6b5_unorm_pack16) == VK_FORMAT_R5G6B5_UNORM_PACK16);
    static_assert(vkenum(format::b5g6r5_unorm_pack16) == VK_FORMAT_B5G6R5_UNORM_PACK16);
    static_assert(vkenum(format::r5g5b5a1_unorm_pack16) == VK_FORMAT_R5G5B5A1_UNORM_PACK16);
    static_assert(vkenum(format::b5g5r5a1_unorm_pack16) == VK_FORMAT_B5G5R5A1_UNORM_PACK16);
    static_assert(vkenum(format::a1r5g5b5_unorm_pack16) == VK_FORMAT_A1R5G5B5_UNORM_PACK16);
    static_assert(vkenum(format::r8_unorm) == VK_FORMAT_R8_UNORM);
    static_assert(vkenum(format::r8_snorm) == VK_FORMAT_R8_SNORM);
    static_assert(vkenum(format::r8_uscaled) == VK_FORMAT_R8_USCALED);
    static_assert(vkenum(format::r8_sscaled) == VK_FORMAT_R8_SSCALED);
    static_assert(vkenum(format::r8_uint) == VK_FORMAT_R8_UINT);
    static_assert(vkenum(format::r8_sint) == VK_FORMAT_R8_SINT);
    static_assert(vkenum(format::r8_srgb) == VK_FORMAT_R8_SRGB);
    static_assert(vkenum(format::r8g8_unorm) == VK_FORMAT_R8G8_UNORM);
    static_assert(vkenum(format::r8g8_snorm) == VK_FORMAT_R8G8_SNORM);
    static_assert(vkenum(format::r8g8_uscaled) == VK_FORMAT_R8G8_USCALED);
    static_assert(vkenum(format::r8g8_sscaled) == VK_FORMAT_R8G8_SSCALED);
    static_assert(vkenum(format::r8g8_uint) == VK_FORMAT_R8G8_UINT);
    static_assert(vkenum(format::r8g8_sint) == VK_FORMAT_R8G8_SINT);
    static_assert(vkenum(format::r8g8_srgb) == VK_FORMAT_R8G8_SRGB);
    static_assert(vkenum(format::r8g8b8_unorm) == VK_FORMAT_R8G8B8_UNORM);
    static_assert(vkenum(format::r8g8b8_snorm) == VK_FORMAT_R8G8B8_SNORM);
    static_assert(vkenum(format::r8g8b8_uscaled) == VK_FORMAT_R8G8B8_USCALED);
    static_assert(vkenum(format::r8g8b8_sscaled) == VK_FORMAT_R8G8B8_SSCALED);
    static_assert(vkenum(format::r8g8b8_uint) == VK_FORMAT_R8G8B8_UINT);
    static_assert(vkenum(format::r8g8b8_sint) == VK_FORMAT_R8G8B8_SINT);
    static_assert(vkenum(format::r8g8b8_srgb) == VK_FORMAT_R8G8B8_SRGB);
    static_assert(vkenum(format::b8g8r8_unorm) == VK_FORMAT_B8G8R8_UNORM);
    static_assert(vkenum(format::b8g8r8_snorm) == VK_FORMAT_B8G8R8_SNORM);
    static_assert(vkenum(format::b8g8r8_uscaled) == VK_FORMAT_B8G8R8_USCALED);
    static_assert(vkenum(format::b8g8r8_sscaled) == VK_FORMAT_B8G8R8_SSCALED);
    static_assert(vkenum(format::b8g8r8_uint) == VK_FORMAT_B8G8R8_UINT);
    static_assert(vkenum(format::b8g8r8_sint) == VK_FORMAT_B8G8R8_SINT);
    static_assert(vkenum(format::b8g8r8_srgb) == VK_FORMAT_B8G8R8_SRGB);
    static_assert(vkenum(format::r8g8b8a8_unorm) == VK_FORMAT_R8G8B8A8_UNORM);
    static_assert(vkenum(format::r8g8b8a8_snorm) == VK_FORMAT_R8G8B8A8_SNORM);
    static_assert(vkenum(format::r8g8b8a8_uscaled) == VK_FORMAT_R8G8B8A8_USCALED);
    static_assert(vkenum(format::r8g8b8a8_sscaled) == VK_FORMAT_R8G8B8A8_SSCALED);
    static_assert(vkenum(format::r8g8b8a8_uint) == VK_FORMAT_R8G8B8A8_UINT);
    static_assert(vkenum(format::r8g8b8a8_sint) == VK_FORMAT_R8G8B8A8_SINT);
    static_assert(vkenum(format::r8g8b8a8_srgb) == VK_FORMAT_R8G8B8A8_SRGB);
    static_assert(vkenum(format::b8g8r8a8_unorm) == VK_FORMAT_B8G8R8A8_UNORM);
    static_assert(vkenum(format::b8g8r8a8_snorm) == VK_FORMAT_B8G8R8A8_SNORM);
    static_assert(vkenum(format::b8g8r8a8_uscaled) == VK_FORMAT_B8G8R8A8_USCALED);
    static_assert(vkenum(format::b8g8r8a8_sscaled) == VK_FORMAT_B8G8R8A8_SSCALED);
    static_assert(vkenum(format::b8g8r8a8_uint) == VK_FORMAT_B8G8R8A8_UINT);
    static_assert(vkenum(format::b8g8r8a8_sint) == VK_FORMAT_B8G8R8A8_SINT);
    static_assert(vkenum(format::b8g8r8a8_srgb) == VK_FORMAT_B8G8R8A8_SRGB);
    static_assert(vkenum(format::a8b8g8r8_unorm_pack32) == VK_FORMAT_A8B8G8R8_UNORM_PACK32);
    static_assert(vkenum(format::a8b8g8r8_snorm_pack32) == VK_FORMAT_A8B8G8R8_SNORM_PACK32);
    static_assert(vkenum(format::a8b8g8r8_uscaled_pack32) == VK_FORMAT_A8B8G8R8_USCALED_PACK32);
    static_assert(vkenum(format::a8b8g8r8_sscaled_pack32) == VK_FORMAT_A8B8G8R8_SSCALED_PACK32);
    static_assert(vkenum(format::a8b8g8r8_uint_pack32) == VK_FORMAT_A8B8G8R8_UINT_PACK32);
    static_assert(vkenum(format::a8b8g8r8_sint_pack32) == VK_FORMAT_A8B8G8R8_SINT_PACK32);
    static_assert(vkenum(format::a8b8g8r8_srgb_pack32) == VK_FORMAT_A8B8G8R8_SRGB_PACK32);
    static_assert(vkenum(format::a2r10g10b10_unorm_pack32) == VK_FORMAT_A2R10G10B10_UNORM_PACK32);
    static_assert(vkenum(format::a2r10g10b10_snorm_pack32) == VK_FORMAT_A2R10G10B10_SNORM_PACK32);
    static_assert(vkenum(format::a2r10g10b10_uscaled_pack32) == VK_FORMAT_A2R10G10B10_USCALED_PACK32);
    static_assert(vkenum(format::a2r10g10b10_sscaled_pack32) == VK_FORMAT_A2R10G10B10_SSCALED_PACK32);
    static_assert(vkenum(format::a2r10g10b10_uint_pack32) == VK_FORMAT_A2R10G10B10_UINT_PACK32);
    static_assert(vkenum(format::a2r10g10b10_sint_pack32) == VK_FORMAT_A2R10G10B10_SINT_PACK32);
    static_assert(vkenum(format::a2b10g10r10_unorm_pack32) == VK_FORMAT_A2B10G10R10_UNORM_PACK32);
    static_assert(vkenum(format::a2b10g10r10_snorm_pack32) == VK_FORMAT_A2B10G10R10_SNORM_PACK32);
    static_assert(vkenum(format::a2b10g10r10_uscaled_pack32) == VK_FORMAT_A2B10G10R10_USCALED_PACK32);
    static_assert(vkenum(format::a2b10g10r10_sscaled_pack32) == VK_FORMAT_A2B10G10R10_SSCALED_PACK32);
    static_assert(vkenum(format::a2b10g10r10_uint_pack32) == VK_FORMAT_A2B10G10R10_UINT_PACK32);
    static_assert(vkenum(format::a2b10g10r10_sint_pack32) == VK_FORMAT_A2B10G10R10_SINT_PACK32);
    static_assert(vkenum(format::r16_unorm) == VK_FORMAT_R16_UNORM);
    static_assert(vkenum(format::r16_snorm) == VK_FORMAT_R16_SNORM);
    static_assert(vkenum(format::r16_uscaled) == VK_FORMAT_R16_USCALED);
    static_assert(vkenum(format::r16_sscaled) == VK_FORMAT_R16_SSCALED);
    static_assert(vkenum(format::r16_uint) == VK_FORMAT_R16_UINT);
    static_assert(vkenum(format::r16_sint) == VK_FORMAT_R16_SINT);
    static_assert(vkenum(format::r16_sfloat) == VK_FORMAT_R16_SFLOAT);
    static_assert(vkenum(format::r16g16_unorm) == VK_FORMAT_R16G16_UNORM);
    static_assert(vkenum(format::r16g16_snorm) == VK_FORMAT_R16G16_SNORM);
    static_assert(vkenum(format::r16g16_uscaled) == VK_FORMAT_R16G16_USCALED);
    static_assert(vkenum(format::r16g16_sscaled) == VK_FORMAT_R16G16_SSCALED);
    static_assert(vkenum(format::r16g16_uint) == VK_FORMAT_R16G16_UINT);
    static_assert(vkenum(format::r16g16_sint) == VK_FORMAT_R16G16_SINT);
    static_assert(vkenum(format::r16g16_sfloat) == VK_FORMAT_R16G16_SFLOAT);
    static_assert(vkenum(format::r16g16b16_unorm) == VK_FORMAT_R16G16B16_UNORM);
    static_assert(vkenum(format::r16g16b16_snorm) == VK_FORMAT_R16G16B16_SNORM);
    static_assert(vkenum(format::r16g16b16_uscaled) == VK_FORMAT_R16G16B16_USCALED);
    static_assert(vkenum(format::r16g16b16_sscaled) == VK_FORMAT_R16G16B16_SSCALED);
    static_assert(vkenum(format::r16g16b16_uint) == VK_FORMAT_R16G16B16_UINT);
    static_assert(vkenum(format::r16g16b16_sint) == VK_FORMAT_R16G16B16_SINT);
    static_assert(vkenum(format::r16g16b16_sfloat) == VK_FORMAT_R16G16B16_SFLOAT);
    static_assert(vkenum(format::r16g16b16a16_unorm) == VK_FORMAT_R16G16B16A16_UNORM);
    static_assert(vkenum(format::r16g16b16a16_snorm) == VK_FORMAT_R16G16B16A16_SNORM);
    static_assert(vkenum(format::r16g16b16a16_uscaled) == VK_FORMAT_R16G16B16A16_USCALED);
    static_assert(vkenum(format::r16g16b16a16_sscaled) == VK_FORMAT_R16G16B16A16_SSCALED);
    static_assert(vkenum(format::r16g16b16a16_uint) == VK_FORMAT_R16G16B16A16_UINT);
    static_assert(vkenum(format::r16g16b16a16_sint) == VK_FORMAT_R16G16B16A16_SINT);
    static_assert(vkenum(format::r16g16b16a16_sfloat) == VK_FORMAT_R16G16B16A16_SFLOAT);
    static_assert(vkenum(format::r32_uint) == VK_FORMAT_R32_UINT);
    static_assert(vkenum(format::r32_sint) == VK_FORMAT_R32_SINT);
    static_assert(vkenum(format::r32_sfloat) == VK_FORMAT_R32_SFLOAT);
    static_assert(vkenum(format::r32g32_uint) == VK_FORMAT_R32G32_UINT);
    static_assert(vkenum(format::r32g32_sint) == VK_FORMAT_R32G32_SINT);
    static_assert(vkenum(format::r32g32_sfloat) == VK_FORMAT_R32G32_SFLOAT);
    static_assert(vkenum(format::r32g32b32_uint) == VK_FORMAT_R32G32B32_UINT);
    static_assert(vkenum(format::r32g32b32_sint) == VK_FORMAT_R32G32B32_SINT);
    static_assert(vkenum(format::r32g32b32_sfloat) == VK_FORMAT_R32G32B32_SFLOAT);
    static_assert(vkenum(format::r32g32b32a32_uint) == VK_FORMAT_R32G32B32A32_UINT);
    static_assert(vkenum(format::r32g32b32a32_sint) == VK_FORMAT_R32G32B32A32_SINT);
    static_assert(vkenum(format::r32g32b32a32_sfloat) == VK_FORMAT_R32G32B32A32_SFLOAT);
    static_assert(vkenum(format::r64_uint) == VK_FORMAT_R64_UINT);
    static_assert(vkenum(format::r64_sint) == VK_FORMAT_R64_SINT);
    static_assert(vkenum(format::r64_sfloat) == VK_FORMAT_R64_SFLOAT);
    static_assert(vkenum(format::r64g64_uint) == VK_FORMAT_R64G64_UINT);
    static_assert(vkenum(format::r64g64_sint) == VK_FORMAT_R64G64_SINT);
    static_assert(vkenum(format::r64g64_sfloat) == VK_FORMAT_R64G64_SFLOAT);
    static_assert(vkenum(format::r64g64b64_uint) == VK_FORMAT_R64G64B64_UINT);
    static_assert(vkenum(format::r64g64b64_sint) == VK_FORMAT_R64G64B64_SINT);
    static_assert(vkenum(format::r64g64b64_sfloat) == VK_FORMAT_R64G64B64_SFLOAT);
    static_assert(vkenum(format::r64g64b64a64_uint) == VK_FORMAT_R64G64B64A64_UINT);
    static_assert(vkenum(format::r64g64b64a64_sint) == VK_FORMAT_R64G64B64A64_SINT);
    static_assert(vkenum(format::r64g64b64a64_sfloat) == VK_FORMAT_R64G64B64A64_SFLOAT);
    static_assert(vkenum(format::b10g11r11_ufloat_pack32) == VK_FORMAT_B10G11R11_UFLOAT_PACK32);
    static_assert(vkenum(format::e5b9g9r9_ufloat_pack32) == VK_FORMAT_E5B9G9R9_UFLOAT_PACK32);
    static_assert(vkenum(format::d16_unorm) == VK_FORMAT_D16_UNORM);
    static_assert(vkenum(format::x8_d24_unorm_pack32) == VK_FORMAT_X8_D24_UNORM_PACK32);
    static_assert(vkenum(format::d32_sfloat) == VK_FORMAT_D32_SFLOAT);
    static_assert(vkenum(format::s8_uint) == VK_FORMAT_S8_UINT);
    static_assert(vkenum(format::d16_unorm_s8_uint) == VK_FORMAT_D16_UNORM_S8_UINT);
    static_assert(vkenum(format::d24_unorm_s8_uint) == VK_FORMAT_D24_UNORM_S8_UINT);
    static_assert(vkenum(format::d32_sfloat_s8_uint) == VK_FORMAT_D32_SFLOAT_S8_UINT);
    static_assert(vkenum(format::bc1_rgb_unorm_block) == VK_FORMAT_BC1_RGB_UNORM_BLOCK);
    static_assert(vkenum(format::bc1_rgb_srgb_block) == VK_FORMAT_BC1_RGB_SRGB_BLOCK);
    static_assert(vkenum(format::bc1_rgba_unorm_block) == VK_FORMAT_BC1_RGBA_UNORM_BLOCK);
    static_assert(vkenum(format::bc1_rgba_srgb_block) == VK_FORMAT_BC1_RGBA_SRGB_BLOCK);
    static_assert(vkenum(format::bc2_unorm_block) == VK_FORMAT_BC2_UNORM_BLOCK);
    static_assert(vkenum(format::bc2_srgb_block) == VK_FORMAT_BC2_SRGB_BLOCK);
    static_assert(vkenum(format::bc3_unorm_block) == VK_FORMAT_BC3_UNORM_BLOCK);
    static_assert(vkenum(format::bc3_srgb_block) == VK_FORMAT_BC3_SRGB_BLOCK);
    static_assert(vkenum(format::bc4_unorm_block) == VK_FORMAT_BC4_UNORM_BLOCK);
    static_assert(vkenum(format::bc4_snorm_block) == VK_FORMAT_BC4_SNORM_BLOCK);
    static_assert(vkenum(format::bc5_unorm_block) == VK_FORMAT_BC5_UNORM_BLOCK);
    static_assert(vkenum(format::bc5_snorm_block) == VK_FORMAT_BC5_SNORM_BLOCK);
    static_assert(vkenum(format::bc6h_ufloat_block) == VK_FORMAT_BC6H_UFLOAT_BLOCK);
    static_assert(vkenum(format::bc6h_sfloat_block) == VK_FORMAT_BC6H_SFLOAT_BLOCK);
    static_assert(vkenum(format::bc7_unorm_block) == VK_FORMAT_BC7_UNORM_BLOCK);
    static_assert(vkenum(format::bc7_srgb_block) == VK_FORMAT_BC7_SRGB_BLOCK);
    static_assert(vkenum(format::etc2_r8g8b8_unorm_block) == VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK);
    static_assert(vkenum(format::etc2_r8g8b8_srgb_block) == VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK);
    static_assert(vkenum(format::etc2_r8g8b8a1_unorm_block) == VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK);
    static_assert(vkenum(format::etc2_r8g8b8a1_srgb_block) == VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK);
    static_assert(vkenum(format::etc2_r8g8b8a8_unorm_block) == VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK);
    static_assert(vkenum(format::etc2_r8g8b8a8_srgb_block) == VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK);
    static_assert(vkenum(format::eac_r11_unorm_block) == VK_FORMAT_EAC_R11_UNORM_BLOCK);
    static_assert(vkenum(format::eac_r11_snorm_block) == VK_FORMAT_EAC_R11_SNORM_BLOCK);
    static_assert(vkenum(format::eac_r11g11_unorm_block) == VK_FORMAT_EAC_R11G11_UNORM_BLOCK);
    static_assert(vkenum(format::eac_r11g11_snorm_block) == VK_FORMAT_EAC_R11G11_SNORM_BLOCK);
    static_assert(vkenum(format::astc_4x4_unorm_block) == VK_FORMAT_ASTC_4x4_UNORM_BLOCK);
    static_assert(vkenum(format::astc_4x4_srgb_block) == VK_FORMAT_ASTC_4x4_SRGB_BLOCK);
    static_assert(vkenum(format::astc_5x4_unorm_block) == VK_FORMAT_ASTC_5x4_UNORM_BLOCK);
    static_assert(vkenum(format::astc_5x4_srgb_block) == VK_FORMAT_ASTC_5x4_SRGB_BLOCK);
    static_assert(vkenum(format::astc_5x5_unorm_block) == VK_FORMAT_ASTC_5x5_UNORM_BLOCK);
    static_assert(vkenum(format::astc_5x5_srgb_block) == VK_FORMAT_ASTC_5x5_SRGB_BLOCK);
    static_assert(vkenum(format::astc_6x5_unorm_block) == VK_FORMAT_ASTC_6x5_UNORM_BLOCK);
    static_assert(vkenum(format::astc_6x5_srgb_block) == VK_FORMAT_ASTC_6x5_SRGB_BLOCK);
    static_assert(vkenum(format::astc_6x6_unorm_block) == VK_FORMAT_ASTC_6x6_UNORM_BLOCK);
    static_assert(vkenum(format::astc_6x6_srgb_block) == VK_FORMAT_ASTC_6x6_SRGB_BLOCK);
    static_assert(vkenum(format::astc_8x5_unorm_block) == VK_FORMAT_ASTC_8x5_UNORM_BLOCK);
    static_assert(vkenum(format::astc_8x5_srgb_block) == VK_FORMAT_ASTC_8x5_SRGB_BLOCK);
    static_assert(vkenum(format::astc_8x6_unorm_block) == VK_FORMAT_ASTC_8x6_UNORM_BLOCK);
    static_assert(vkenum(format::astc_8x6_srgb_block) == VK_FORMAT_ASTC_8x6_SRGB_BLOCK);
    static_assert(vkenum(format::astc_8x8_unorm_block) == VK_FORMAT_ASTC_8x8_UNORM_BLOCK);
    static_assert(vkenum(format::astc_8x8_srgb_block) == VK_FORMAT_ASTC_8x8_SRGB_BLOCK);
    static_assert(vkenum(format::astc_10x5_unorm_block) == VK_FORMAT_ASTC_10x5_UNORM_BLOCK);
    static_assert(vkenum(format::astc_10x5_srgb_block) == VK_FORMAT_ASTC_10x5_SRGB_BLOCK);
    static_assert(vkenum(format::astc_10x6_unorm_block) == VK_FORMAT_ASTC_10x6_UNORM_BLOCK);
    static_assert(vkenum(format::astc_10x6_srgb_block) == VK_FORMAT_ASTC_10x6_SRGB_BLOCK);
    static_assert(vkenum(format::astc_10x8_unorm_block) == VK_FORMAT_ASTC_10x8_UNORM_BLOCK);
    static_assert(vkenum(format::astc_10x8_srgb_block) == VK_FORMAT_ASTC_10x8_SRGB_BLOCK);
    static_assert(vkenum(format::astc_10x10_unorm_block) == VK_FORMAT_ASTC_10x10_UNORM_BLOCK);
    static_assert(vkenum(format::astc_10x10_srgb_block) == VK_FORMAT_ASTC_10x10_SRGB_BLOCK);
    static_assert(vkenum(format::astc_12x10_unorm_block) == VK_FORMAT_ASTC_12x10_UNORM_BLOCK);
    static_assert(vkenum(format::astc_12x10_srgb_block) == VK_FORMAT_ASTC_12x10_SRGB_BLOCK);
    static_assert(vkenum(format::astc_12x12_unorm_block) == VK_FORMAT_ASTC_12x12_UNORM_BLOCK);
    static_assert(vkenum(format::astc_12x12_srgb_block) == VK_FORMAT_ASTC_12x12_SRGB_BLOCK);
    static_assert(vkenum(format::g8b8g8r8_422_unorm) == VK_FORMAT_G8B8G8R8_422_UNORM);
    static_assert(vkenum(format::b8g8r8g8_422_unorm) == VK_FORMAT_B8G8R8G8_422_UNORM);
    static_assert(vkenum(format::g8_b8_r8_3plane_420_unorm) == VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM);
    static_assert(vkenum(format::g8_b8r8_2plane_420_unorm) == VK_FORMAT_G8_B8R8_2PLANE_420_UNORM);
    static_assert(vkenum(format::g8_b8_r8_3plane_422_unorm) == VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM);
    static_assert(vkenum(format::g8_b8r8_2plane_422_unorm) == VK_FORMAT_G8_B8R8_2PLANE_422_UNORM);
    static_assert(vkenum(format::g8_b8_r8_3plane_444_unorm) == VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM);
    static_assert(vkenum(format::r10x6_unorm_pack16) == VK_FORMAT_R10X6_UNORM_PACK16);
    static_assert(vkenum(format::r10x6g10x6_unorm_2pack16) == VK_FORMAT_R10X6G10X6_UNORM_2PACK16);
    static_assert(vkenum(format::r10x6g10x6b10x6a10x6_unorm_4pack16) == VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16);
    static_assert(vkenum(format::g10x6b10x6g10x6r10x6_422_unorm_4pack16) == VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16);
    static_assert(vkenum(format::r12x4_unorm_pack16) == VK_FORMAT_R12X4_UNORM_PACK16);
    static_assert(vkenum(format::r12x4g12x4_unorm_2pack16) == VK_FORMAT_R12X4G12X4_UNORM_2PACK16);
    static_assert(vkenum(format::b10x6g10x6r10x6g10x6_422_unorm_4pack16) == VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16);
    static_assert(vkenum(format::g10x6_b10x6_r10x6_3plane_420_unorm_3pack16) == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16);
    static_assert(vkenum(format::g10x6_b10x6r10x6_2plane_420_unorm_3pack16) == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16);
    static_assert(vkenum(format::g10x6_b10x6_r10x6_3plane_422_unorm_3pack16) == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16);
    static_assert(vkenum(format::g10x6_b10x6r10x6_2plane_422_unorm_3pack16) == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16);
    static_assert(vkenum(format::g10x6_b10x6_r10x6_3plane_444_unorm_3pack16) == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16);
    static_assert(vkenum(format::r12x4g12x4b12x4a12x4_unorm_4pack16) == VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16);
    static_assert(vkenum(format::g12x4b12x4g12x4r12x4_422_unorm_4pack16) == VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16);
    static_assert(vkenum(format::b12x4g12x4r12x4g12x4_422_unorm_4pack16) == VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16);
    static_assert(vkenum(format::g12x4_b12x4_r12x4_3plane_420_unorm_3pack16) == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16);
    static_assert(vkenum(format::g12x4_b12x4r12x4_2plane_420_unorm_3pack16) == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16);
    static_assert(vkenum(format::g12x4_b12x4_r12x4_3plane_422_unorm_3pack16) == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16);
    static_assert(vkenum(format::g12x4_b12x4r12x4_2plane_422_unorm_3pack16) == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16);
    static_assert(vkenum(format::g12x4_b12x4_r12x4_3plane_444_unorm_3pack16) == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16);
    static_assert(vkenum(format::g16b16g16r16_422_unorm) == VK_FORMAT_G16B16G16R16_422_UNORM);
    static_assert(vkenum(format::b16g16r16g16_422_unorm) == VK_FORMAT_B16G16R16G16_422_UNORM);
    static_assert(vkenum(format::g16_b16_r16_3plane_420_unorm) == VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM);
    static_assert(vkenum(format::g16_b16r16_2plane_420_unorm) == VK_FORMAT_G16_B16R16_2PLANE_420_UNORM);
    static_assert(vkenum(format::g16_b16_r16_3plane_422_unorm) == VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM);
    static_assert(vkenum(format::g16_b16r16_2plane_422_unorm) == VK_FORMAT_G16_B16R16_2PLANE_422_UNORM);
    static_assert(vkenum(format::g16_b16_r16_3plane_444_unorm) == VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM);
    static_assert(vkenum(format::g8_b8r8_2plane_444_unorm) == VK_FORMAT_G8_B8R8_2PLANE_444_UNORM);
    static_assert(vkenum(format::g10x6_b10x6r10x6_2plane_444_unorm_3pack16) == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16);
    static_assert(vkenum(format::g12x4_b12x4r12x4_2plane_444_unorm_3pack16) == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16);
    static_assert(vkenum(format::g16_b16r16_2plane_444_unorm) == VK_FORMAT_G16_B16R16_2PLANE_444_UNORM);
    static_assert(vkenum(format::a4r4g4b4_unorm_pack16) == VK_FORMAT_A4R4G4B4_UNORM_PACK16);
    static_assert(vkenum(format::a4b4g4r4_unorm_pack16) == VK_FORMAT_A4B4G4R4_UNORM_PACK16);
    static_assert(vkenum(format::astc_4x4_sfloat_block) == VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_5x4_sfloat_block) == VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_5x5_sfloat_block) == VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_6x5_sfloat_block) == VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_6x6_sfloat_block) == VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_8x5_sfloat_block) == VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_8x6_sfloat_block) == VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_8x8_sfloat_block) == VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_10x5_sfloat_block) == VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_10x6_sfloat_block) == VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_10x8_sfloat_block) == VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_10x10_sfloat_block) == VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_12x10_sfloat_block) == VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK);
    static_assert(vkenum(format::astc_12x12_sfloat_block) == VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK);
    static_assert(vkenum(format::pvrtc1_2bpp_unorm_block_img) == VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG);
    static_assert(vkenum(format::pvrtc1_4bpp_unorm_block_img) == VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG);
    static_assert(vkenum(format::pvrtc2_2bpp_unorm_block_img) == VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG);
    static_assert(vkenum(format::pvrtc2_4bpp_unorm_block_img) == VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG);
    static_assert(vkenum(format::pvrtc1_2bpp_srgb_block_img) == VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG);
    static_assert(vkenum(format::pvrtc1_4bpp_srgb_block_img) == VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG);
    static_assert(vkenum(format::pvrtc2_2bpp_srgb_block_img) == VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG);
    static_assert(vkenum(format::pvrtc2_4bpp_srgb_block_img) == VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG);
    static_assert(vkenum(format::r16g16_s10_5_nv) == VK_FORMAT_R16G16_S10_5_NV);
    static_assert(vkenum(format::astc_4x4_sfloat_block_ext) == VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_5x4_sfloat_block_ext) == VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_5x5_sfloat_block_ext) == VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_6x5_sfloat_block_ext) == VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_6x6_sfloat_block_ext) == VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_8x5_sfloat_block_ext) == VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_8x6_sfloat_block_ext) == VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_8x8_sfloat_block_ext) == VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_10x5_sfloat_block_ext) == VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_10x6_sfloat_block_ext) == VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_10x8_sfloat_block_ext) == VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_10x10_sfloat_block_ext) == VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_12x10_sfloat_block_ext) == VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::astc_12x12_sfloat_block_ext) == VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT);
    static_assert(vkenum(format::g8b8g8r8_422_unorm_khr) == VK_FORMAT_G8B8G8R8_422_UNORM_KHR);
    static_assert(vkenum(format::b8g8r8g8_422_unorm_khr) == VK_FORMAT_B8G8R8G8_422_UNORM_KHR);
    static_assert(vkenum(format::g8_b8_r8_3plane_420_unorm_khr) == VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR);
    static_assert(vkenum(format::g8_b8r8_2plane_420_unorm_khr) == VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR);
    static_assert(vkenum(format::g8_b8_r8_3plane_422_unorm_khr) == VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR);
    static_assert(vkenum(format::g8_b8r8_2plane_422_unorm_khr) == VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR);
    static_assert(vkenum(format::g8_b8_r8_3plane_444_unorm_khr) == VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR);
    static_assert(vkenum(format::r10x6_unorm_pack16_khr) == VK_FORMAT_R10X6_UNORM_PACK16_KHR);
    static_assert(vkenum(format::r10x6g10x6_unorm_2pack16_khr) == VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR);
    static_assert(vkenum(format::r10x6g10x6b10x6a10x6_unorm_4pack16_khr) == VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR);
    static_assert(vkenum(format::g10x6b10x6g10x6r10x6_422_unorm_4pack16_khr) == VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR);
    static_assert(vkenum(format::b10x6g10x6r10x6g10x6_422_unorm_4pack16_khr) == VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR);
    static_assert(vkenum(format::g10x6_b10x6_r10x6_3plane_420_unorm_3pack16_khr) == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g10x6_b10x6r10x6_2plane_420_unorm_3pack16_khr) == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g10x6_b10x6_r10x6_3plane_422_unorm_3pack16_khr) == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g10x6_b10x6r10x6_2plane_422_unorm_3pack16_khr) == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g10x6_b10x6_r10x6_3plane_444_unorm_3pack16_khr) == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::r12x4_unorm_pack16_khr) == VK_FORMAT_R12X4_UNORM_PACK16_KHR);
    static_assert(vkenum(format::r12x4g12x4_unorm_2pack16_khr) == VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR);
    static_assert(vkenum(format::r12x4g12x4b12x4a12x4_unorm_4pack16_khr) == VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR);
    static_assert(vkenum(format::g12x4b12x4g12x4r12x4_422_unorm_4pack16_khr) == VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR);
    static_assert(vkenum(format::b12x4g12x4r12x4g12x4_422_unorm_4pack16_khr) == VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR);
    static_assert(vkenum(format::g12x4_b12x4_r12x4_3plane_420_unorm_3pack16_khr) == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g12x4_b12x4r12x4_2plane_420_unorm_3pack16_khr) == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g12x4_b12x4_r12x4_3plane_422_unorm_3pack16_khr) == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g12x4_b12x4r12x4_2plane_422_unorm_3pack16_khr) == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g12x4_b12x4_r12x4_3plane_444_unorm_3pack16_khr) == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR);
    static_assert(vkenum(format::g10x6_b10x6r10x6_2plane_444_unorm_3pack16_ext) == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT);
    static_assert(vkenum(format::g12x4_b12x4r12x4_2plane_444_unorm_3pack16_ext) == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT);
    static_assert(vkenum(format::g16b16g16r16_422_unorm_khr) == VK_FORMAT_G16B16G16R16_422_UNORM_KHR);
    static_assert(vkenum(format::b16g16r16g16_422_unorm_khr) == VK_FORMAT_B16G16R16G16_422_UNORM_KHR);
    static_assert(vkenum(format::g16_b16_r16_3plane_420_unorm_khr) == VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR);
    static_assert(vkenum(format::g16_b16r16_2plane_420_unorm_khr) == VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR);
    static_assert(vkenum(format::g16_b16_r16_3plane_422_unorm_khr) == VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR);
    static_assert(vkenum(format::g16_b16r16_2plane_422_unorm_khr) == VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR);
    static_assert(vkenum(format::g16_b16_r16_3plane_444_unorm_khr) == VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR);
    static_assert(vkenum(format::g8_b8r8_2plane_444_unorm_ext) == VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT);
    static_assert(vkenum(format::g16_b16r16_2plane_444_unorm_ext) == VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT);
    static_assert(vkenum(format::a4r4g4b4_unorm_pack16_ext) == VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT);
    static_assert(vkenum(format::a4b4g4r4_unorm_pack16_ext) == VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT);

    // present_mode
    static_assert(vkenum(present_mode::immediate_khr) == VK_PRESENT_MODE_IMMEDIATE_KHR);
    static_assert(vkenum(present_mode::mailbox_khr) == VK_PRESENT_MODE_MAILBOX_KHR);
    static_assert(vkenum(present_mode::fifo_khr) == VK_PRESENT_MODE_FIFO_KHR);
    static_assert(vkenum(present_mode::fifo_relaxed_khr) == VK_PRESENT_MODE_FIFO_RELAXED_KHR);
    static_assert(vkenum(present_mode::shared_demand_refresh_khr) == VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR);
    static_assert(vkenum(present_mode::shared_continuous_refresh_khr) == VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR);

    // color_space
    static_assert(vkenum(color_space::srgb_nonlinear_khr) == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
    static_assert(vkenum(color_space::display_p3_nonlinear_ext) == VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT);
    static_assert(vkenum(color_space::extended_srgb_linear_ext) == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT);
    static_assert(vkenum(color_space::display_p3_linear_ext) == VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT);
    static_assert(vkenum(color_space::dci_p3_nonlinear_ext) == VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT);
    static_assert(vkenum(color_space::bt709_linear_ext) == VK_COLOR_SPACE_BT709_LINEAR_EXT);
    static_assert(vkenum(color_space::bt709_nonlinear_ext) == VK_COLOR_SPACE_BT709_NONLINEAR_EXT);
    static_assert(vkenum(color_space::bt2020_linear_ext) == VK_COLOR_SPACE_BT2020_LINEAR_EXT);
    static_assert(vkenum(color_space::hdr10_st2084_ext) == VK_COLOR_SPACE_HDR10_ST2084_EXT);
    static_assert(vkenum(color_space::dolbyvision_ext) == VK_COLOR_SPACE_DOLBYVISION_EXT);
    static_assert(vkenum(color_space::hdr10_hlg_ext) == VK_COLOR_SPACE_HDR10_HLG_EXT);
    static_assert(vkenum(color_space::adobergb_linear_ext) == VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT);
    static_assert(vkenum(color_space::adobergb_nonlinear_ext) == VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT);
    static_assert(vkenum(color_space::pass_through_ext) == VK_COLOR_SPACE_PASS_THROUGH_EXT);
    static_assert(vkenum(color_space::extended_srgb_nonlinear_ext) == VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT);
    static_assert(vkenum(color_space::display_native_amd) == VK_COLOR_SPACE_DISPLAY_NATIVE_AMD);
    static_assert(vkenum(color_space::dci_p3_linear_ext) == VK_COLOR_SPACE_DCI_P3_LINEAR_EXT);

    // image_usage_flag
    static_assert(vkenum(image_usage_flag::transfer_src) == VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
    static_assert(vkenum(image_usage_flag::transfer_dst) == VK_IMAGE_USAGE_TRANSFER_DST_BIT);
    static_assert(vkenum(image_usage_flag::sampled) == VK_IMAGE_USAGE_SAMPLED_BIT);
    static_assert(vkenum(image_usage_flag::storage) == VK_IMAGE_USAGE_STORAGE_BIT);
    static_assert(vkenum(image_usage_flag::color_attachment) == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    static_assert(vkenum(image_usage_flag::depth_stencil_attachment) == VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    static_assert(vkenum(image_usage_flag::transient_attachment) == VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT);
    static_assert(vkenum(image_usage_flag::input_attachment) == VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
    static_assert(vkenum(image_usage_flag::video_decode_dst_khr) == VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR);
    static_assert(vkenum(image_usage_flag::video_decode_src_khr) == VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR);
    static_assert(vkenum(image_usage_flag::video_decode_dpb_khr) == VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR);
    static_assert(vkenum(image_usage_flag::fragment_density_map_ext) == VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT);
    static_assert(vkenum(image_usage_flag::invocation_mask_huawei) == VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI);
    static_assert(vkenum(image_usage_flag::sample_weight_qcom) == VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM);
    static_assert(vkenum(image_usage_flag::sample_block_match_qcom) == VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM);
    static_assert(vkenum(image_usage_flag::shading_rate_image_nv) == VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV);
    static_assert(vkenum(image_usage_flag::attachment_feedback_loop_ext) == VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT);

    static_assert(vkenum(image_usage_flag::fragment_shading_rate_attachment_khr) == VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR);
#ifdef VK_ENABLE_BETA_EXTENSIONS
    static_assert(vkenum(image_usage_flag::video_encode_dst_khr) == VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR);
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    static_assert(vkenum(image_usage_flag::video_encode_src_khr) == VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR);
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    static_assert(vkenum(image_usage_flag::video_encode_dpb_khr) == VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR);
#endif

    // sharing_mode
    static_assert(vkenum(sharing_mode::exclusive) == VK_SHARING_MODE_EXCLUSIVE);
    static_assert(vkenum(sharing_mode::concurrent) == VK_SHARING_MODE_CONCURRENT);

    // surface_transform_flag
    static_assert(vkenum(surface_transform_flag::identity_khr) == VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
    static_assert(vkenum(surface_transform_flag::rotate_90_khr) == VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR);
    static_assert(vkenum(surface_transform_flag::rotate_180_khr) == VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR);
    static_assert(vkenum(surface_transform_flag::rotate_270_khr) == VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR);
    static_assert(vkenum(surface_transform_flag::horizontal_mirror_khr) == VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR);
    static_assert(vkenum(surface_transform_flag::inherit_khr) == VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR);
    static_assert(vkenum(surface_transform_flag::horizontal_mirror_rotate_90_khr) == VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR);
    static_assert(vkenum(surface_transform_flag::horizontal_mirror_rotate_180_khr) == VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR);
    static_assert(vkenum(surface_transform_flag::horizontal_mirror_rotate_270_khr) == VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR);

    // composite_alpha_flag
    static_assert(vkenum(composite_alpha_flag::opaque_khr) == VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
    static_assert(vkenum(composite_alpha_flag::pre_multiplied_khr) == VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR);
    static_assert(vkenum(composite_alpha_flag::post_multiplied_khr) == VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR);
    static_assert(vkenum(composite_alpha_flag::inherit_khr) == VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR);

    // image_tiling
    static_assert(vkenum(image_tiling::optimal) == VK_IMAGE_TILING_OPTIMAL);
    static_assert(vkenum(image_tiling::linear) == VK_IMAGE_TILING_LINEAR);
    static_assert(vkenum(image_tiling::drm_format_modifier_ext) == VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT);

    // image_type
    static_assert(vkenum(image_type::_1d) == VK_IMAGE_TYPE_1D);
    static_assert(vkenum(image_type::_2d) == VK_IMAGE_TYPE_2D);
    static_assert(vkenum(image_type::_3d) == VK_IMAGE_TYPE_3D);

    // image_view_type
    static_assert(vkenum(image_view_type::_1d) == VK_IMAGE_VIEW_TYPE_1D);
    static_assert(vkenum(image_view_type::_2d) == VK_IMAGE_VIEW_TYPE_2D);
    static_assert(vkenum(image_view_type::_3d) == VK_IMAGE_VIEW_TYPE_3D);
    static_assert(vkenum(image_view_type::cube) == VK_IMAGE_VIEW_TYPE_CUBE);
    static_assert(vkenum(image_view_type::_1d_array) == VK_IMAGE_VIEW_TYPE_1D_ARRAY);
    static_assert(vkenum(image_view_type::_2d_array) == VK_IMAGE_VIEW_TYPE_2D_ARRAY);
    static_assert(vkenum(image_view_type::cube_array) == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY);

    // component_swizzle
    static_assert(vkenum(component_swizzle::identity) == VK_COMPONENT_SWIZZLE_IDENTITY);
    static_assert(vkenum(component_swizzle::zero) == VK_COMPONENT_SWIZZLE_ZERO);
    static_assert(vkenum(component_swizzle::one) == VK_COMPONENT_SWIZZLE_ONE);
    static_assert(vkenum(component_swizzle::r) == VK_COMPONENT_SWIZZLE_R);
    static_assert(vkenum(component_swizzle::g) == VK_COMPONENT_SWIZZLE_G);
    static_assert(vkenum(component_swizzle::b) == VK_COMPONENT_SWIZZLE_B);
    static_assert(vkenum(component_swizzle::a) == VK_COMPONENT_SWIZZLE_A);

    // sample_count_flag
    static_assert(vkenum(sample_count_flag::_1) == VK_SAMPLE_COUNT_1_BIT);
    static_assert(vkenum(sample_count_flag::_2) == VK_SAMPLE_COUNT_2_BIT);
    static_assert(vkenum(sample_count_flag::_4) == VK_SAMPLE_COUNT_4_BIT);
    static_assert(vkenum(sample_count_flag::_8) == VK_SAMPLE_COUNT_8_BIT);
    static_assert(vkenum(sample_count_flag::_16) == VK_SAMPLE_COUNT_16_BIT);
    static_assert(vkenum(sample_count_flag::_32) == VK_SAMPLE_COUNT_32_BIT);
    static_assert(vkenum(sample_count_flag::_64) == VK_SAMPLE_COUNT_64_BIT);

    // descriptor_pool_create_flag
    static_assert(vkenum(descriptor_pool_create_flag::free_descriptor_set) == VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
    static_assert(vkenum(descriptor_pool_create_flag::update_after_bind) == VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT);
    static_assert(vkenum(descriptor_pool_create_flag::update_after_bind_ext) == VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT);
    static_assert(vkenum(descriptor_pool_create_flag::host_only_ext) == VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT);
    static_assert(vkenum(descriptor_pool_create_flag::host_only_valve) == VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_VALVE);

    // image_layout
    static_assert(vkenum(image_layout::undefined) == VK_IMAGE_LAYOUT_UNDEFINED);
    static_assert(vkenum(image_layout::general) == VK_IMAGE_LAYOUT_GENERAL);
    static_assert(vkenum(image_layout::color_attachment_optimal) == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    static_assert(vkenum(image_layout::depth_stencil_attachment_optimal) == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    static_assert(vkenum(image_layout::depth_stencil_read_only_optimal) == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
    static_assert(vkenum(image_layout::shader_read_only_optimal) == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    static_assert(vkenum(image_layout::transfer_src_optimal) == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    static_assert(vkenum(image_layout::transfer_dst_optimal) == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    static_assert(vkenum(image_layout::preinitialized) == VK_IMAGE_LAYOUT_PREINITIALIZED);
    static_assert(vkenum(image_layout::depth_read_only_stencil_attachment_optimal) == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL);
    static_assert(vkenum(image_layout::depth_attachment_stencil_read_only_optimal) == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL);
    static_assert(vkenum(image_layout::depth_attachment_optimal) == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
    static_assert(vkenum(image_layout::depth_read_only_optimal) == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL);
    static_assert(vkenum(image_layout::stencil_attachment_optimal) == VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL);
    static_assert(vkenum(image_layout::stencil_read_only_optimal) == VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL);
    static_assert(vkenum(image_layout::read_only_optimal) == VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL);
    static_assert(vkenum(image_layout::attachment_optimal) == VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL);
    static_assert(vkenum(image_layout::present_src_khr) == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    static_assert(vkenum(image_layout::video_decode_dst_khr) == VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR);
    static_assert(vkenum(image_layout::video_decode_src_khr) == VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR);
    static_assert(vkenum(image_layout::video_decode_dpb_khr) == VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR);
    static_assert(vkenum(image_layout::shared_present_khr) == VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR);
    static_assert(vkenum(image_layout::fragment_density_map_optimal_ext) == VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT);
    static_assert(vkenum(image_layout::fragment_shading_rate_attachment_optimal_khr) == VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR);
    static_assert(vkenum(image_layout::attachment_feedback_loop_optimal_ext) == VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT);
    static_assert(vkenum(image_layout::depth_read_only_stencil_attachment_optimal_khr) == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR);
    static_assert(vkenum(image_layout::depth_attachment_stencil_read_only_optimal_khr) == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR);
    static_assert(vkenum(image_layout::shading_rate_optimal_nv) == VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV);
    static_assert(vkenum(image_layout::depth_attachment_optimal_khr) == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR);
    static_assert(vkenum(image_layout::depth_read_only_optimal_khr) == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR);
    static_assert(vkenum(image_layout::stencil_attachment_optimal_khr) == VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR);
    static_assert(vkenum(image_layout::stencil_read_only_optimal_khr) == VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR);
    static_assert(vkenum(image_layout::read_only_optimal_khr) == VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR);
    static_assert(vkenum(image_layout::attachment_optimal_khr) == VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR);
#ifdef VK_ENABLE_BETA_EXTENSIONS
    static_assert(vkenum(image_layout::video_encode_dst_khr) == VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR);
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    static_assert(vkenum(image_layout::video_encode_src_khr) == VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR);
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
    static_assert(vkenum(image_layout::video_encode_dpb_khr) == VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR);
#endif

    // command_pool_create_flag
    static_assert(vkenum(command_pool_create_flag::transient) == VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
    static_assert(vkenum(command_pool_create_flag::reset_command_buffer) == VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    static_assert(vkenum(command_pool_create_flag::prot) == VK_COMMAND_POOL_CREATE_PROTECTED_BIT);

    // memory_property_flags
    static_assert(vkenum(memory_property_flag::device_local) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    static_assert(vkenum(memory_property_flag::host_visible) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    static_assert(vkenum(memory_property_flag::host_coherent) == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    static_assert(vkenum(memory_property_flag::host_cached) == VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
    static_assert(vkenum(memory_property_flag::lazily_allocated) == VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT);
    static_assert(vkenum(memory_property_flag::prot) == VK_MEMORY_PROPERTY_PROTECTED_BIT);
    static_assert(vkenum(memory_property_flag::device_coherent_amd) == VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD);
    static_assert(vkenum(memory_property_flag::device_uncached_amd) == VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD);
    static_assert(vkenum(memory_property_flag::rdma_capable_nv) == VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV);

    // memory_usage
    static_assert(vkenum(memory_usage::unknown) == VMA_MEMORY_USAGE_UNKNOWN);
    static_assert(vkenum(memory_usage::gpu_only) == VMA_MEMORY_USAGE_GPU_ONLY);
    static_assert(vkenum(memory_usage::cpu_only) == VMA_MEMORY_USAGE_CPU_ONLY);
    static_assert(vkenum(memory_usage::cpu_to_gpu) == VMA_MEMORY_USAGE_CPU_TO_GPU);
    static_assert(vkenum(memory_usage::gpu_to_cpu) == VMA_MEMORY_USAGE_GPU_TO_CPU);
    static_assert(vkenum(memory_usage::cpu_copy) == VMA_MEMORY_USAGE_CPU_COPY);
    static_assert(vkenum(memory_usage::gpu_lazily_allocated) == VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED);
    static_assert(vkenum(memory_usage::usage_auto) == VMA_MEMORY_USAGE_AUTO);
    static_assert(vkenum(memory_usage::auto_prefer_device) == VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);
    static_assert(vkenum(memory_usage::auto_prefer_host) == VMA_MEMORY_USAGE_AUTO_PREFER_HOST);

    // memory_flag
    static_assert(vkenum(memory_flag::dedicated_memory) == VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);
    static_assert(vkenum(memory_flag::never_allocate) == VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT);
    static_assert(vkenum(memory_flag::mapped) == VMA_ALLOCATION_CREATE_MAPPED_BIT);
    static_assert(vkenum(memory_flag::user_data_copy_string) == VMA_ALLOCATION_CREATE_USER_DATA_COPY_STRING_BIT);
    static_assert(vkenum(memory_flag::upper_address) == VMA_ALLOCATION_CREATE_UPPER_ADDRESS_BIT);
    static_assert(vkenum(memory_flag::dont_bind) == VMA_ALLOCATION_CREATE_DONT_BIND_BIT);
    static_assert(vkenum(memory_flag::within_budget) == VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT);
    static_assert(vkenum(memory_flag::can_alias) == VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT);
    static_assert(vkenum(memory_flag::host_access_sequential_write) == VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
    static_assert(vkenum(memory_flag::host_access_random) == VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT);
    static_assert(vkenum(memory_flag::host_access_allow_transfer_instead) == VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT);
    static_assert(vkenum(memory_flag::strategy_min_memory) == VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT);
    static_assert(vkenum(memory_flag::strategy_min_time) == VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT);
    static_assert(vkenum(memory_flag::strategy_min_offset) == VMA_ALLOCATION_CREATE_STRATEGY_MIN_OFFSET_BIT);
    static_assert(vkenum(memory_flag::strategy_best_fit) == VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT);
    static_assert(vkenum(memory_flag::strategy_first_fit) == VMA_ALLOCATION_CREATE_STRATEGY_FIRST_FIT_BIT);
    static_assert(vkenum(memory_flag::strategy_mask) == VMA_ALLOCATION_CREATE_STRATEGY_MASK);

    // filter
    static_assert(vkenum(filter::nearest) == VK_FILTER_NEAREST);
    static_assert(vkenum(filter::linear) == VK_FILTER_LINEAR);
    static_assert(vkenum(filter::cubic_ext) == VK_FILTER_CUBIC_EXT);
    static_assert(vkenum(filter::cubic_img) == VK_FILTER_CUBIC_IMG);

    // debug_utils_message_severity_flag
    static_assert(vkenum(debug_utils_message_severity_flag::verbose) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT);
    static_assert(vkenum(debug_utils_message_severity_flag::info) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT);
    static_assert(vkenum(debug_utils_message_severity_flag::warning) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT);
    static_assert(vkenum(debug_utils_message_severity_flag::error) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

    // debug_utils_message_type_flag
    static_assert(vkenum(debug_utils_message_type_flag::general) == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT);
    static_assert(vkenum(debug_utils_message_type_flag::validation) == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT);
    static_assert(vkenum(debug_utils_message_type_flag::performance) == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);
    static_assert(vkenum(debug_utils_message_type_flag::device_address_binding) == VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT);

    // command_buffer_usage_flag
    static_assert(vkenum(command_buffer_usage_flag::one_time_submit) == VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    static_assert(vkenum(command_buffer_usage_flag::render_pass_continue) == VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
    static_assert(vkenum(command_buffer_usage_flag::simultaneous_use) == VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

    // subpass_external
    static_assert(subpass_external == VK_SUBPASS_EXTERNAL);

    // subpass_description_flag
    static_assert(vkenum(subpass_description_flag::per_view_attributes) == VK_SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX);
    static_assert(vkenum(subpass_description_flag::per_view_position_x_only) == VK_SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX);
    static_assert(vkenum(subpass_description_flag::fragment_region) == VK_SUBPASS_DESCRIPTION_FRAGMENT_REGION_BIT_QCOM);
    static_assert(vkenum(subpass_description_flag::shader_resolve) == VK_SUBPASS_DESCRIPTION_SHADER_RESOLVE_BIT_QCOM);
    static_assert(vkenum(subpass_description_flag::rasterization_order_attachment_color_access) == VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS_BIT_EXT);
    static_assert(vkenum(subpass_description_flag::rasterization_order_attachment_depth_access) == VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_EXT);
    static_assert(vkenum(subpass_description_flag::rasterization_order_attachment_stencil_access) == VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_EXT);
    static_assert(vkenum(subpass_description_flag::enable_legacy_dithering) == VK_SUBPASS_DESCRIPTION_ENABLE_LEGACY_DITHERING_BIT_EXT);

    // shader_stage_flag
    static_assert(vkenum(shader_stage_flag::vertex) == VK_SHADER_STAGE_VERTEX_BIT);
    static_assert(vkenum(shader_stage_flag::tesselation_control) == VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT);
    static_assert(vkenum(shader_stage_flag::tesselation_evaluation) == VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);
    static_assert(vkenum(shader_stage_flag::geometry) == VK_SHADER_STAGE_GEOMETRY_BIT);
    static_assert(vkenum(shader_stage_flag::fragment) == VK_SHADER_STAGE_FRAGMENT_BIT);
    static_assert(vkenum(shader_stage_flag::compute) == VK_SHADER_STAGE_COMPUTE_BIT);
    static_assert(vkenum(shader_stage_flag::all_graphics) == VK_SHADER_STAGE_ALL_GRAPHICS);
    static_assert(vkenum(shader_stage_flag::all) == VK_SHADER_STAGE_ALL);
    static_assert(vkenum(shader_stage_flag::raygen) == VK_SHADER_STAGE_RAYGEN_BIT_KHR);
    static_assert(vkenum(shader_stage_flag::any_hit) == VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
    static_assert(vkenum(shader_stage_flag::closest_hit) == VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR);
    static_assert(vkenum(shader_stage_flag::miss) == VK_SHADER_STAGE_MISS_BIT_KHR);
    static_assert(vkenum(shader_stage_flag::intersection) == VK_SHADER_STAGE_INTERSECTION_BIT_KHR);
    static_assert(vkenum(shader_stage_flag::callable) == VK_SHADER_STAGE_CALLABLE_BIT_KHR);
    static_assert(vkenum(shader_stage_flag::task) == VK_SHADER_STAGE_TASK_BIT_EXT);
    static_assert(vkenum(shader_stage_flag::mesh) == VK_SHADER_STAGE_MESH_BIT_EXT);
    static_assert(vkenum(shader_stage_flag::subpass_shading_huawei) == VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI);
    static_assert(vkenum(shader_stage_flag::cluster_culling_huawei) == VK_SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI);
    static_assert(vkenum(shader_stage_flag::raygen_nv) == VK_SHADER_STAGE_RAYGEN_BIT_NV);
    static_assert(vkenum(shader_stage_flag::any_hit_nv) == VK_SHADER_STAGE_ANY_HIT_BIT_NV);
    static_assert(vkenum(shader_stage_flag::closest_hit_nv) == VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV);
    static_assert(vkenum(shader_stage_flag::miss_nv) == VK_SHADER_STAGE_MISS_BIT_NV);
    static_assert(vkenum(shader_stage_flag::intersection_nv) == VK_SHADER_STAGE_INTERSECTION_BIT_NV);
    static_assert(vkenum(shader_stage_flag::callable_nv) == VK_SHADER_STAGE_CALLABLE_BIT_NV);
    static_assert(vkenum(shader_stage_flag::task_nv) == VK_SHADER_STAGE_TASK_BIT_NV);
    static_assert(vkenum(shader_stage_flag::mesh_nv) == VK_SHADER_STAGE_MESH_BIT_NV);

    // dynamic_state
    static_assert(vkenum(dynamic_state::viewport) == VK_DYNAMIC_STATE_VIEWPORT);
    static_assert(vkenum(dynamic_state::scissor) == VK_DYNAMIC_STATE_SCISSOR);
    static_assert(vkenum(dynamic_state::line_width) == VK_DYNAMIC_STATE_LINE_WIDTH);
    static_assert(vkenum(dynamic_state::depth_bias) == VK_DYNAMIC_STATE_DEPTH_BIAS);
    static_assert(vkenum(dynamic_state::blend_constants) == VK_DYNAMIC_STATE_BLEND_CONSTANTS);
    static_assert(vkenum(dynamic_state::depth_bounds) == VK_DYNAMIC_STATE_DEPTH_BOUNDS);
    static_assert(vkenum(dynamic_state::stencil_compare_mask) == VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK);
    static_assert(vkenum(dynamic_state::stencil_write_mask) == VK_DYNAMIC_STATE_STENCIL_WRITE_MASK);
    static_assert(vkenum(dynamic_state::stencil_reference) == VK_DYNAMIC_STATE_STENCIL_REFERENCE);
    static_assert(vkenum(dynamic_state::cull_mode) == VK_DYNAMIC_STATE_CULL_MODE);
    static_assert(vkenum(dynamic_state::front_face) == VK_DYNAMIC_STATE_FRONT_FACE);
    static_assert(vkenum(dynamic_state::primitive_topology) == VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY);
    static_assert(vkenum(dynamic_state::viewport_with_count) == VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT);
    static_assert(vkenum(dynamic_state::scissor_with_count) == VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT);
    static_assert(vkenum(dynamic_state::vertex_input_binding_stride) == VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE);
    static_assert(vkenum(dynamic_state::depth_test_enable) == VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE);
    static_assert(vkenum(dynamic_state::depth_write_enable) == VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE);
    static_assert(vkenum(dynamic_state::depth_compare_op) == VK_DYNAMIC_STATE_DEPTH_COMPARE_OP);
    static_assert(vkenum(dynamic_state::depth_bounds_test_enable) == VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE);
    static_assert(vkenum(dynamic_state::stencil_test_enable) == VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE);
    static_assert(vkenum(dynamic_state::stencil_op) == VK_DYNAMIC_STATE_STENCIL_OP);
    static_assert(vkenum(dynamic_state::rasterizer_discard_enable) == VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE);
    static_assert(vkenum(dynamic_state::depth_bias_enable) == VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE);
    static_assert(vkenum(dynamic_state::primitive_restart_enable) == VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE);
    static_assert(vkenum(dynamic_state::viewport_w_scaling_nv) == VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV);
    static_assert(vkenum(dynamic_state::discard_rectangle_ext) == VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT);
    static_assert(vkenum(dynamic_state::sample_locations_ext) == VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT);
    static_assert(vkenum(dynamic_state::ray_tracing_pipeline_stack_size_khr) == VK_DYNAMIC_STATE_RAY_TRACING_PIPELINE_STACK_SIZE_KHR);
    static_assert(vkenum(dynamic_state::viewport_shading_rate_palette_nv) == VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV);
    static_assert(vkenum(dynamic_state::viewport_coarse_sample_order_nv) == VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV);
    static_assert(vkenum(dynamic_state::exclusive_scissor_nv) == VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV);
    static_assert(vkenum(dynamic_state::fragment_shading_rate_khr) == VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR);
    static_assert(vkenum(dynamic_state::line_stipple_ext) == VK_DYNAMIC_STATE_LINE_STIPPLE_EXT);
    static_assert(vkenum(dynamic_state::vertex_input_ext) == VK_DYNAMIC_STATE_VERTEX_INPUT_EXT);
    static_assert(vkenum(dynamic_state::patch_control_points_ext) == VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT);
    static_assert(vkenum(dynamic_state::logic_op_ext) == VK_DYNAMIC_STATE_LOGIC_OP_EXT);
    static_assert(vkenum(dynamic_state::color_write_enable_ext) == VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::tessellation_domain_origin_ext) == VK_DYNAMIC_STATE_TESSELLATION_DOMAIN_ORIGIN_EXT);
    static_assert(vkenum(dynamic_state::depth_clamp_enable_ext) == VK_DYNAMIC_STATE_DEPTH_CLAMP_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::polygon_mode_ext) == VK_DYNAMIC_STATE_POLYGON_MODE_EXT);
    static_assert(vkenum(dynamic_state::rasterization_samples_ext) == VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT);
    static_assert(vkenum(dynamic_state::sample_mask_ext) == VK_DYNAMIC_STATE_SAMPLE_MASK_EXT);
    static_assert(vkenum(dynamic_state::alpha_to_coverage_enable_ext) == VK_DYNAMIC_STATE_ALPHA_TO_COVERAGE_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::alpha_to_one_enable_ext) == VK_DYNAMIC_STATE_ALPHA_TO_ONE_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::logic_op_enable_ext) == VK_DYNAMIC_STATE_LOGIC_OP_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::color_blend_enable_ext) == VK_DYNAMIC_STATE_COLOR_BLEND_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::color_blend_equation_ext) == VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT);
    static_assert(vkenum(dynamic_state::color_write_mask_ext) == VK_DYNAMIC_STATE_COLOR_WRITE_MASK_EXT);
    static_assert(vkenum(dynamic_state::rasterization_stream_ext) == VK_DYNAMIC_STATE_RASTERIZATION_STREAM_EXT);
    static_assert(vkenum(dynamic_state::conservative_rasterization_mode_ext) == VK_DYNAMIC_STATE_CONSERVATIVE_RASTERIZATION_MODE_EXT);
    static_assert(vkenum(dynamic_state::extra_primitive_overestimation_size_ext) == VK_DYNAMIC_STATE_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT);
    static_assert(vkenum(dynamic_state::depth_clip_enable_ext) == VK_DYNAMIC_STATE_DEPTH_CLIP_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::sample_locations_enable_ext) == VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::color_blend_advanced_ext) == VK_DYNAMIC_STATE_COLOR_BLEND_ADVANCED_EXT);
    static_assert(vkenum(dynamic_state::provoking_vertex_mode_ext) == VK_DYNAMIC_STATE_PROVOKING_VERTEX_MODE_EXT);
    static_assert(vkenum(dynamic_state::line_rasterization_mode_ext) == VK_DYNAMIC_STATE_LINE_RASTERIZATION_MODE_EXT);
    static_assert(vkenum(dynamic_state::line_stipple_enable_ext) == VK_DYNAMIC_STATE_LINE_STIPPLE_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::depth_clip_negative_one_to_one_ext) == VK_DYNAMIC_STATE_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT);
    static_assert(vkenum(dynamic_state::viewport_w_scaling_enable_nv) == VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_ENABLE_NV);
    static_assert(vkenum(dynamic_state::viewport_swizzle_nv) == VK_DYNAMIC_STATE_VIEWPORT_SWIZZLE_NV);
    static_assert(vkenum(dynamic_state::coverage_to_color_enable_nv) == VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_ENABLE_NV);
    static_assert(vkenum(dynamic_state::coverage_to_color_location_nv) == VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_LOCATION_NV);
    static_assert(vkenum(dynamic_state::coverage_modulation_mode_nv) == VK_DYNAMIC_STATE_COVERAGE_MODULATION_MODE_NV);
    static_assert(vkenum(dynamic_state::coverage_modulation_table_enable_nv) == VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_ENABLE_NV);
    static_assert(vkenum(dynamic_state::coverage_modulation_table_nv) == VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_NV);
    static_assert(vkenum(dynamic_state::shading_rate_image_enable_nv) == VK_DYNAMIC_STATE_SHADING_RATE_IMAGE_ENABLE_NV);
    static_assert(vkenum(dynamic_state::representative_fragment_test_enable_nv) == VK_DYNAMIC_STATE_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV);
    static_assert(vkenum(dynamic_state::coverage_reduction_mode_nv) == VK_DYNAMIC_STATE_COVERAGE_REDUCTION_MODE_NV);
    static_assert(vkenum(dynamic_state::cull_mode_ext) == VK_DYNAMIC_STATE_CULL_MODE_EXT);
    static_assert(vkenum(dynamic_state::front_face_ext) == VK_DYNAMIC_STATE_FRONT_FACE_EXT);
    static_assert(vkenum(dynamic_state::primitive_topology_ext) == VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY_EXT);
    static_assert(vkenum(dynamic_state::viewport_with_count_ext) == VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT_EXT);
    static_assert(vkenum(dynamic_state::scissor_with_count_ext) == VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT_EXT);
    static_assert(vkenum(dynamic_state::vertex_input_binding_stride_ext) == VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE_EXT);
    static_assert(vkenum(dynamic_state::depth_test_enable_ext) == VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::depth_write_enable_ext) == VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::depth_compare_op_ext) == VK_DYNAMIC_STATE_DEPTH_COMPARE_OP_EXT);
    static_assert(vkenum(dynamic_state::depth_bounds_test_enable_ext) == VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::stencil_test_enable_ext) == VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::stencil_op_ext) == VK_DYNAMIC_STATE_STENCIL_OP_EXT);
    static_assert(vkenum(dynamic_state::rasterizer_discard_enable_ext) == VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::depth_bias_enable_ext) == VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE_EXT);
    static_assert(vkenum(dynamic_state::primitive_restart_enable_ext) == VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE_EXT);

    // primitive_topology
    static_assert(vkenum(primitive_topology::point_list) == VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
    static_assert(vkenum(primitive_topology::line_list) == VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
    static_assert(vkenum(primitive_topology::line_strip) == VK_PRIMITIVE_TOPOLOGY_LINE_STRIP);
    static_assert(vkenum(primitive_topology::triangle_list) == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    static_assert(vkenum(primitive_topology::triangle_strip) == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
    static_assert(vkenum(primitive_topology::triangle_fan) == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN);
    static_assert(vkenum(primitive_topology::line_list_with_adjacency) == VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY);
    static_assert(vkenum(primitive_topology::line_strip_with_adjacency) == VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY);
    static_assert(vkenum(primitive_topology::triangle_list_with_adjacency) == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY);
    static_assert(vkenum(primitive_topology::triangle_strip_with_adjacency) == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY);
    static_assert(vkenum(primitive_topology::patch_list) == VK_PRIMITIVE_TOPOLOGY_PATCH_LIST);

    // polygon_mode
    static_assert(vkenum(polygon_mode::fill) == VK_POLYGON_MODE_FILL);
    static_assert(vkenum(polygon_mode::line) == VK_POLYGON_MODE_LINE);
    static_assert(vkenum(polygon_mode::point) == VK_POLYGON_MODE_POINT);
    static_assert(vkenum(polygon_mode::fill_rectangle_nv) == VK_POLYGON_MODE_FILL_RECTANGLE_NV);

    // cull_mode
    static_assert(vkenum(cull_mode::none) == VK_CULL_MODE_NONE);
    static_assert(vkenum(cull_mode::front) == VK_CULL_MODE_FRONT_BIT);
    static_assert(vkenum(cull_mode::back) == VK_CULL_MODE_BACK_BIT);
    static_assert(vkenum(cull_mode::front_and_back) == VK_CULL_MODE_FRONT_AND_BACK);

    // front_face
    static_assert(vkenum(front_face::counter_clockwise) == VK_FRONT_FACE_COUNTER_CLOCKWISE);
    static_assert(vkenum(front_face::clockwise) == VK_FRONT_FACE_CLOCKWISE);

    // color_component
    static_assert(vkenum(color_component::r) == VK_COLOR_COMPONENT_R_BIT);
    static_assert(vkenum(color_component::g) == VK_COLOR_COMPONENT_G_BIT);
    static_assert(vkenum(color_component::b) == VK_COLOR_COMPONENT_B_BIT);
    static_assert(vkenum(color_component::a) == VK_COLOR_COMPONENT_A_BIT);

    // blend_factor
    static_assert(vkenum(blend_factor::zero) == VK_BLEND_FACTOR_ZERO);
    static_assert(vkenum(blend_factor::one) == VK_BLEND_FACTOR_ONE);
    static_assert(vkenum(blend_factor::src_color) == VK_BLEND_FACTOR_SRC_COLOR);
    static_assert(vkenum(blend_factor::one_minus_src_color) == VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR);
    static_assert(vkenum(blend_factor::dst_color) == VK_BLEND_FACTOR_DST_COLOR);
    static_assert(vkenum(blend_factor::one_minus_dst_color) == VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR);
    static_assert(vkenum(blend_factor::src_alpha) == VK_BLEND_FACTOR_SRC_ALPHA);
    static_assert(vkenum(blend_factor::one_minus_src_alpha) == VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
    static_assert(vkenum(blend_factor::dst_alpha) == VK_BLEND_FACTOR_DST_ALPHA);
    static_assert(vkenum(blend_factor::one_minus_dst_alpha) == VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA);
    static_assert(vkenum(blend_factor::constant_color) == VK_BLEND_FACTOR_CONSTANT_COLOR);
    static_assert(vkenum(blend_factor::one_minus_constant_color) == VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR);
    static_assert(vkenum(blend_factor::constant_alpha) == VK_BLEND_FACTOR_CONSTANT_ALPHA);
    static_assert(vkenum(blend_factor::one_minus_constant_alpha) == VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA);
    static_assert(vkenum(blend_factor::src_alpha_saturate) == VK_BLEND_FACTOR_SRC_ALPHA_SATURATE);
    static_assert(vkenum(blend_factor::src1_color) == VK_BLEND_FACTOR_SRC1_COLOR);
    static_assert(vkenum(blend_factor::one_minus_src1_color) == VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR);
    static_assert(vkenum(blend_factor::src1_alpha) == VK_BLEND_FACTOR_SRC1_ALPHA);
    static_assert(vkenum(blend_factor::one_minus_src1_alpha) == VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA);

    // blend_op
    static_assert(vkenum(blend_op::add) == VK_BLEND_OP_ADD);
    static_assert(vkenum(blend_op::subtract) == VK_BLEND_OP_SUBTRACT);
    static_assert(vkenum(blend_op::reverse_subtract) == VK_BLEND_OP_REVERSE_SUBTRACT);
    static_assert(vkenum(blend_op::min) == VK_BLEND_OP_MIN);
    static_assert(vkenum(blend_op::max) == VK_BLEND_OP_MAX);
    static_assert(vkenum(blend_op::zero_ext) == VK_BLEND_OP_ZERO_EXT);
    static_assert(vkenum(blend_op::src_ext) == VK_BLEND_OP_SRC_EXT);
    static_assert(vkenum(blend_op::dst_ext) == VK_BLEND_OP_DST_EXT);
    static_assert(vkenum(blend_op::src_over_ext) == VK_BLEND_OP_SRC_OVER_EXT);
    static_assert(vkenum(blend_op::dst_over_ext) == VK_BLEND_OP_DST_OVER_EXT);
    static_assert(vkenum(blend_op::src_in_ext) == VK_BLEND_OP_SRC_IN_EXT);
    static_assert(vkenum(blend_op::dst_in_ext) == VK_BLEND_OP_DST_IN_EXT);
    static_assert(vkenum(blend_op::src_out_ext) == VK_BLEND_OP_SRC_OUT_EXT);
    static_assert(vkenum(blend_op::dst_out_ext) == VK_BLEND_OP_DST_OUT_EXT);
    static_assert(vkenum(blend_op::src_atop_ext) == VK_BLEND_OP_SRC_ATOP_EXT);
    static_assert(vkenum(blend_op::dst_atop_ext) == VK_BLEND_OP_DST_ATOP_EXT);
    static_assert(vkenum(blend_op::xor_ext) == VK_BLEND_OP_XOR_EXT);
    static_assert(vkenum(blend_op::multiply_ext) == VK_BLEND_OP_MULTIPLY_EXT);
    static_assert(vkenum(blend_op::screen_ext) == VK_BLEND_OP_SCREEN_EXT);
    static_assert(vkenum(blend_op::overlay_ext) == VK_BLEND_OP_OVERLAY_EXT);
    static_assert(vkenum(blend_op::darken_ext) == VK_BLEND_OP_DARKEN_EXT);
    static_assert(vkenum(blend_op::lighten_ext) == VK_BLEND_OP_LIGHTEN_EXT);
    static_assert(vkenum(blend_op::colordodge_ext) == VK_BLEND_OP_COLORDODGE_EXT);
    static_assert(vkenum(blend_op::colorburn_ext) == VK_BLEND_OP_COLORBURN_EXT);
    static_assert(vkenum(blend_op::hardlight_ext) == VK_BLEND_OP_HARDLIGHT_EXT);
    static_assert(vkenum(blend_op::softlight_ext) == VK_BLEND_OP_SOFTLIGHT_EXT);
    static_assert(vkenum(blend_op::difference_ext) == VK_BLEND_OP_DIFFERENCE_EXT);
    static_assert(vkenum(blend_op::exclusion_ext) == VK_BLEND_OP_EXCLUSION_EXT);
    static_assert(vkenum(blend_op::invert_ext) == VK_BLEND_OP_INVERT_EXT);
    static_assert(vkenum(blend_op::invert_rgb_ext) == VK_BLEND_OP_INVERT_RGB_EXT);
    static_assert(vkenum(blend_op::lineardodge_ext) == VK_BLEND_OP_LINEARDODGE_EXT);
    static_assert(vkenum(blend_op::linearburn_ext) == VK_BLEND_OP_LINEARBURN_EXT);
    static_assert(vkenum(blend_op::vividlight_ext) == VK_BLEND_OP_VIVIDLIGHT_EXT);
    static_assert(vkenum(blend_op::linearlight_ext) == VK_BLEND_OP_LINEARLIGHT_EXT);
    static_assert(vkenum(blend_op::pinlight_ext) == VK_BLEND_OP_PINLIGHT_EXT);
    static_assert(vkenum(blend_op::hardmix_ext) == VK_BLEND_OP_HARDMIX_EXT);
    static_assert(vkenum(blend_op::hsl_hue_ext) == VK_BLEND_OP_HSL_HUE_EXT);
    static_assert(vkenum(blend_op::hsl_saturation_ext) == VK_BLEND_OP_HSL_SATURATION_EXT);
    static_assert(vkenum(blend_op::hsl_color_ext) == VK_BLEND_OP_HSL_COLOR_EXT);
    static_assert(vkenum(blend_op::hsl_luminosity_ext) == VK_BLEND_OP_HSL_LUMINOSITY_EXT);
    static_assert(vkenum(blend_op::plus_ext) == VK_BLEND_OP_PLUS_EXT);
    static_assert(vkenum(blend_op::plus_clamped_ext) == VK_BLEND_OP_PLUS_CLAMPED_EXT);
    static_assert(vkenum(blend_op::plus_clamped_alpha_ext) == VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT);
    static_assert(vkenum(blend_op::plus_darker_ext) == VK_BLEND_OP_PLUS_DARKER_EXT);
    static_assert(vkenum(blend_op::minus_ext) == VK_BLEND_OP_MINUS_EXT);
    static_assert(vkenum(blend_op::minus_clamped_ext) == VK_BLEND_OP_MINUS_CLAMPED_EXT);
    static_assert(vkenum(blend_op::contrast_ext) == VK_BLEND_OP_CONTRAST_EXT);
    static_assert(vkenum(blend_op::invert_ovg_ext) == VK_BLEND_OP_INVERT_OVG_EXT);
    static_assert(vkenum(blend_op::red_ext) == VK_BLEND_OP_RED_EXT);
    static_assert(vkenum(blend_op::green_ext) == VK_BLEND_OP_GREEN_EXT);
    static_assert(vkenum(blend_op::blue_ext) == VK_BLEND_OP_BLUE_EXT);

    // shader_kind
    static_assert(vkenum(shader_kind::vertex) == shaderc_shader_kind::shaderc_vertex_shader);
    static_assert(vkenum(shader_kind::fragment) == shaderc_shader_kind::shaderc_fragment_shader);
    static_assert(vkenum(shader_kind::compute) == shaderc_shader_kind::shaderc_compute_shader);
    static_assert(vkenum(shader_kind::geometry) == shaderc_shader_kind::shaderc_geometry_shader);
    static_assert(vkenum(shader_kind::tess_control) == shaderc_shader_kind::shaderc_tess_control_shader);
    static_assert(vkenum(shader_kind::tess_evaluation) == shaderc_shader_kind::shaderc_tess_evaluation_shader);
    static_assert(vkenum(shader_kind::raygen) == shaderc_shader_kind::shaderc_raygen_shader);
    static_assert(vkenum(shader_kind::anyhit) == shaderc_shader_kind::shaderc_anyhit_shader);
    static_assert(vkenum(shader_kind::closesthit) == shaderc_shader_kind::shaderc_closesthit_shader);
    static_assert(vkenum(shader_kind::miss) == shaderc_shader_kind::shaderc_miss_shader);
    static_assert(vkenum(shader_kind::intersection) == shaderc_shader_kind::shaderc_intersection_shader);
    static_assert(vkenum(shader_kind::callable) == shaderc_shader_kind::shaderc_callable_shader);
    static_assert(vkenum(shader_kind::task) == shaderc_shader_kind::shaderc_task_shader);
    static_assert(vkenum(shader_kind::mesh) == shaderc_shader_kind::shaderc_mesh_shader);
    static_assert(vkenum(shader_kind::glsl_vertex) == shaderc_shader_kind::shaderc_glsl_vertex_shader);
    static_assert(vkenum(shader_kind::glsl_fragment) == shaderc_shader_kind::shaderc_glsl_fragment_shader);
    static_assert(vkenum(shader_kind::glsl_compute) == shaderc_shader_kind::shaderc_glsl_compute_shader);
    static_assert(vkenum(shader_kind::glsl_geometry) == shaderc_shader_kind::shaderc_glsl_geometry_shader);
    static_assert(vkenum(shader_kind::glsl_tess_control) == shaderc_shader_kind::shaderc_glsl_tess_control_shader);
    static_assert(vkenum(shader_kind::glsl_tess_evaluation) == shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader);
    static_assert(vkenum(shader_kind::glsl_raygen) == shaderc_shader_kind::shaderc_glsl_raygen_shader);
    static_assert(vkenum(shader_kind::glsl_anyhit) == shaderc_shader_kind::shaderc_glsl_anyhit_shader);
    static_assert(vkenum(shader_kind::glsl_closesthit) == shaderc_shader_kind::shaderc_glsl_closesthit_shader);
    static_assert(vkenum(shader_kind::glsl_miss) == shaderc_shader_kind::shaderc_glsl_miss_shader);
    static_assert(vkenum(shader_kind::glsl_intersection) == shaderc_shader_kind::shaderc_glsl_intersection_shader);
    static_assert(vkenum(shader_kind::glsl_callable) == shaderc_shader_kind::shaderc_glsl_callable_shader);
    static_assert(vkenum(shader_kind::glsl_task) == shaderc_shader_kind::shaderc_glsl_task_shader);
    static_assert(vkenum(shader_kind::glsl_mesh) == shaderc_shader_kind::shaderc_glsl_mesh_shader);
    static_assert(vkenum(shader_kind::glsl_infer) == shaderc_shader_kind::shaderc_glsl_infer_from_source);

    // vertex_input_rate
    static_assert(vkenum(vertex_input_rate::vertex) == VK_VERTEX_INPUT_RATE_VERTEX);
    static_assert(vkenum(vertex_input_rate::instance) == VK_VERTEX_INPUT_RATE_INSTANCE);

    // vertex_format
    static_assert(vkenum(vertex_format::float_t) == VK_FORMAT_R32_SFLOAT);
    static_assert(vkenum(vertex_format::vec2_t) == VK_FORMAT_R32G32_SFLOAT);
    static_assert(vkenum(vertex_format::vec3_t) == VK_FORMAT_R32G32B32_SFLOAT);
    static_assert(vkenum(vertex_format::vec4_t) == VK_FORMAT_R32G32B32A32_SFLOAT);
    static_assert(vkenum(vertex_format::int_t) == VK_FORMAT_R32_SINT);
    static_assert(vkenum(vertex_format::ivec3_t) == VK_FORMAT_R32G32B32_SINT);
    static_assert(vkenum(vertex_format::ivec4_t) == VK_FORMAT_R32G32B32A32_SINT);
    static_assert(vkenum(vertex_format::uint_t) == VK_FORMAT_R32_UINT);
    static_assert(vkenum(vertex_format::uvec2_t) == VK_FORMAT_R32G32_UINT);
    static_assert(vkenum(vertex_format::uvec3_t) == VK_FORMAT_R32G32B32_UINT);
    static_assert(vkenum(vertex_format::uvec4_t) == VK_FORMAT_R32G32B32A32_UINT);
    static_assert(vkenum(vertex_format::short_t) == VK_FORMAT_R16_SINT);
    static_assert(vkenum(vertex_format::svec2_t) == VK_FORMAT_R16G16_SINT);
    static_assert(vkenum(vertex_format::svec3_t) == VK_FORMAT_R16G16B16_SINT);
    static_assert(vkenum(vertex_format::svec4_t) == VK_FORMAT_R16G16B16A16_SINT);
    static_assert(vkenum(vertex_format::ushort_t) == VK_FORMAT_R16_UINT);
    static_assert(vkenum(vertex_format::usvec2_t) == VK_FORMAT_R16G16_UINT);
    static_assert(vkenum(vertex_format::usvec3_t) == VK_FORMAT_R16G16B16_UINT);
    static_assert(vkenum(vertex_format::usvec4_t) == VK_FORMAT_R16G16B16A16_UINT);
    static_assert(vkenum(vertex_format::byte_t) == VK_FORMAT_R8_SINT);
    static_assert(vkenum(vertex_format::bvec2_t) == VK_FORMAT_R8G8_SINT);
    static_assert(vkenum(vertex_format::bvec3_t) == VK_FORMAT_R8G8B8_SINT);
    static_assert(vkenum(vertex_format::bvec4_t) == VK_FORMAT_R8G8B8A8_SINT);
    static_assert(vkenum(vertex_format::ubyte_t) == VK_FORMAT_R8_UINT);
    static_assert(vkenum(vertex_format::ubvec2_t) == VK_FORMAT_R8G8_UINT);
    static_assert(vkenum(vertex_format::ubvec3_t) == VK_FORMAT_R8G8B8_UINT);
    static_assert(vkenum(vertex_format::ubvec4_t) == VK_FORMAT_R8G8B8A8_UINT);
    static_assert(vkenum(vertex_format::double_t) == VK_FORMAT_R64_SFLOAT);
    static_assert(vkenum(vertex_format::dvec2_t) == VK_FORMAT_R64G64_SFLOAT);
    static_assert(vkenum(vertex_format::dvec3_t) == VK_FORMAT_R64G64B64_SFLOAT);
    static_assert(vkenum(vertex_format::dvec4_t) == VK_FORMAT_R64G64B64A64_SFLOAT);

    // buffer_usage_flag
    static_assert(vkenum(buffer_usage_flag::transfer_source) == VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    static_assert(vkenum(buffer_usage_flag::transfer_destination) == VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    static_assert(vkenum(buffer_usage_flag::uniform_texel_buffer) == VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
    static_assert(vkenum(buffer_usage_flag::storage_texel_buffer) == VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
    static_assert(vkenum(buffer_usage_flag::uniform_buffer) == VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    static_assert(vkenum(buffer_usage_flag::storage_buffer) == VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    static_assert(vkenum(buffer_usage_flag::index_buffer) == VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    static_assert(vkenum(buffer_usage_flag::vertex_buffer) == VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    static_assert(vkenum(buffer_usage_flag::indirect_buffer) == VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);

} // namespace orb::vk
