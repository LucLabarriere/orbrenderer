#pragma once

#include "orb/eternal.hpp"

#include <orb/utility.hpp>

#include <array>
#include <map>
#include <string_view>
#include <vulkan/vulkan_core.h>

#define NAME_ENTRY(e) { e, #e }

namespace orb::vk
{

    enum class gpu_type : ui32
    {
        other = 0,
        integrated,
        discrete,
        virtual_gpu,
        cpu,
    };

    inline constexpr auto gpu_type_names = create_name_map<gpu_type>({
        NAME_ENTRY(gpu_type::other),
        NAME_ENTRY(gpu_type::integrated),
        NAME_ENTRY(gpu_type::discrete),
        NAME_ENTRY(gpu_type::virtual_gpu),
        NAME_ENTRY(gpu_type::cpu),
    });

    static_assert(gpu_type_names.unique());

    enum class queue_family : ui32
    {
        graphics     = 1 << 0,
        compute      = 1 << 1,
        transfer     = 1 << 2,
        sparse       = 1 << 3,
        prot         = 1 << 4,
        video_decode = 1 << 5,
    };

    inline constexpr auto queue_family_names = create_name_map<queue_family>({
        NAME_ENTRY(queue_family::graphics),
        NAME_ENTRY(queue_family::compute),
        NAME_ENTRY(queue_family::transfer),
        NAME_ENTRY(queue_family::sparse),
        NAME_ENTRY(queue_family::prot),
        NAME_ENTRY(queue_family::video_decode),
    });

    enum class instance_create : ui32
    {
        portability = 1 << 0,
    };

    inline constexpr auto instance_create_names = create_name_map<instance_create>({
        NAME_ENTRY(instance_create::portability),
    });

    enum class descriptor_type : ui32
    {
        sampler,
        combined_image_sampler,
        sampled_image,
        storage_image,
        uniform_texel_buffer,
        storage_texel_buffer,
        uniform_buffer,
        storage_buffer,
        uniform_buffer_dynamic,
        storage_buffer_dynamic,
        input_attachment,
        inline_uniform_block       = 1000138000,
        acceleration_structure_khr = 1000150000,
        acceleration_structure_nv  = 1000165000,
        sample_weight_image_qcom   = 1000440000,
        block_match_image_qcom     = 1000440001,
        mutable_ext                = 1000351000,
        inline_uniform_block_ext   = 1000138000,
        mutable_valve              = 1000351000,
    };

    inline constexpr auto descriptor_type_names = create_name_map<descriptor_type>({
        NAME_ENTRY(descriptor_type::sampled_image),
        NAME_ENTRY(descriptor_type::storage_image),
        NAME_ENTRY(descriptor_type::uniform_texel_buffer),
        NAME_ENTRY(descriptor_type::storage_texel_buffer),
        NAME_ENTRY(descriptor_type::uniform_buffer),
        NAME_ENTRY(descriptor_type::storage_buffer),
        NAME_ENTRY(descriptor_type::uniform_buffer_dynamic),
        NAME_ENTRY(descriptor_type::storage_buffer_dynamic),
        NAME_ENTRY(descriptor_type::input_attachment),
        NAME_ENTRY(descriptor_type::inline_uniform_block),
        NAME_ENTRY(descriptor_type::acceleration_structure_khr),
        NAME_ENTRY(descriptor_type::acceleration_structure_nv),
        NAME_ENTRY(descriptor_type::sample_weight_image_qcom),
        NAME_ENTRY(descriptor_type::block_match_image_qcom),
        NAME_ENTRY(descriptor_type::mutable_ext),
        NAME_ENTRY(descriptor_type::inline_uniform_block_ext),
        NAME_ENTRY(descriptor_type::mutable_valve),
    });

    enum class attachment_load_op : ui32
    {
        load = 0,
        clear,
        dont_care,
        none = 1000400000,
    };

    inline constexpr auto attachment_load_op_names = create_name_map<attachment_load_op>({
        NAME_ENTRY(attachment_load_op::load),
        NAME_ENTRY(attachment_load_op::clear),
        NAME_ENTRY(attachment_load_op::dont_care),
        NAME_ENTRY(attachment_load_op::none),
    });

    enum class attachment_store_op : ui32
    {
        store = 0,
        dont_care,
        none = 1000301000,
    };

    inline constexpr auto attachment_store_op_names = create_name_map<attachment_store_op>({
        NAME_ENTRY(attachment_store_op::store),
        NAME_ENTRY(attachment_store_op::dont_care),
        NAME_ENTRY(attachment_store_op::none),
    });

    enum class pipeline_bind_point : ui32
    {
        graphics = 0,
        compute,
        ray_tracing_khr = 1000165000,
        ray_tracing_nv  = 1000165000,
    };

    inline constexpr auto pipeline_bind_point_names = create_name_map<pipeline_bind_point>({
        NAME_ENTRY(pipeline_bind_point::graphics),
        NAME_ENTRY(pipeline_bind_point::compute),
        NAME_ENTRY(pipeline_bind_point::ray_tracing_khr),
        NAME_ENTRY(pipeline_bind_point::ray_tracing_nv),
    });

    enum class cmd_buffer_level : ui32
    {
        primary   = 0,
        secondary = 1,
    };

    inline constexpr auto cmd_buffer_level_names = create_name_map<cmd_buffer_level>({
        NAME_ENTRY(cmd_buffer_level::primary),
        NAME_ENTRY(cmd_buffer_level::secondary),
    });

    enum class access_flag : ui32
    {
        none                                      = 0,
        none_khr                                  = 0,
        indirect_command_read                     = 1 << 0,
        index_read                                = 1 << 1,
        vertex_attribute_read                     = 1 << 2,
        uniform_read                              = 1 << 3,
        input_attachment_read                     = 1 << 4,
        shader_read                               = 1 << 5,
        shader_write                              = 1 << 6,
        color_attachment_read                     = 1 << 7,
        color_attachment_write                    = 1 << 8,
        depth_stencil_attachment_read             = 1 << 9,
        depth_stencil_attachment_write            = 1 << 10,
        transfer_read                             = 1 << 11,
        transfer_write                            = 1 << 12,
        host_read                                 = 1 << 13,
        host_write                                = 1 << 14,
        memory_read                               = 1 << 15,
        memory_write                              = 1 << 16,
        command_preprocess_read_nv                = 1 << 17,
        command_preprocess_write_nv               = 1 << 18,
        color_attachment_read_noncoherent_ext     = 1 << 19,
        conditional_rendering_read_ext            = 1 << 20,
        acceleration_structure_read_khr           = 1 << 21,
        acceleration_structure_read_nv            = 1 << 21,
        acceleration_structure_write_khr          = 1 << 22,
        acceleration_structure_write_nv           = 1 << 22,
        fragment_shading_rate_attachment_read_khr = 1 << 23,
        shading_rate_image_read_nv                = 1 << 23,
        fragment_density_map_read_ext             = 1 << 24,
        transform_feedback_write_ext              = 1 << 25,
        transform_feedback_counter_read_ext       = 1 << 26,
        transform_feedback_counter_write_ext      = 1 << 27,
    };

    inline constexpr auto access_flag_names = create_name_map<access_flag>({
        NAME_ENTRY(access_flag::none),
        NAME_ENTRY(access_flag::indirect_command_read),
        NAME_ENTRY(access_flag::index_read),
        NAME_ENTRY(access_flag::vertex_attribute_read),
        NAME_ENTRY(access_flag::uniform_read),
        NAME_ENTRY(access_flag::input_attachment_read),
        NAME_ENTRY(access_flag::shader_read),
        NAME_ENTRY(access_flag::shader_write),
        NAME_ENTRY(access_flag::color_attachment_read),
        NAME_ENTRY(access_flag::color_attachment_write),
        NAME_ENTRY(access_flag::depth_stencil_attachment_read),
        NAME_ENTRY(access_flag::depth_stencil_attachment_write),
        NAME_ENTRY(access_flag::transfer_read),
        NAME_ENTRY(access_flag::transfer_write),
        NAME_ENTRY(access_flag::host_read),
        NAME_ENTRY(access_flag::host_write),
        NAME_ENTRY(access_flag::memory_read),
        NAME_ENTRY(access_flag::memory_write),
        NAME_ENTRY(access_flag::command_preprocess_read_nv),
        NAME_ENTRY(access_flag::command_preprocess_write_nv),
        NAME_ENTRY(access_flag::color_attachment_read_noncoherent_ext),
        NAME_ENTRY(access_flag::conditional_rendering_read_ext),
        NAME_ENTRY(access_flag::acceleration_structure_read_khr),
        NAME_ENTRY(access_flag::acceleration_structure_read_nv),
        NAME_ENTRY(access_flag::acceleration_structure_write_khr),
        NAME_ENTRY(access_flag::acceleration_structure_write_nv),
        NAME_ENTRY(access_flag::fragment_shading_rate_attachment_read_khr),
        NAME_ENTRY(access_flag::shading_rate_image_read_nv),
        NAME_ENTRY(access_flag::fragment_density_map_read_ext),
        NAME_ENTRY(access_flag::transform_feedback_write_ext),
        NAME_ENTRY(access_flag::transform_feedback_counter_read_ext),
        NAME_ENTRY(access_flag::transform_feedback_counter_write_ext),
    });

    enum class image_aspect_flag : ui32
    {
        none           = 0,
        color          = 1 << 0,
        depth          = 1 << 1,
        stencil        = 1 << 2,
        metadata       = 1 << 3,
        plane_0        = 1 << 4,
        plane_1        = 1 << 5,
        plane_2        = 1 << 6,
        memory_plane_0 = 1 << 7,
        memory_plane_1 = 1 << 8,
        memory_plane_2 = 1 << 9,
        memory_plane_3 = 1 << 10,
    };

    inline constexpr auto image_aspect_flag_names = create_name_map<image_aspect_flag>({
        NAME_ENTRY(image_aspect_flag::none),
        NAME_ENTRY(image_aspect_flag::color),
        NAME_ENTRY(image_aspect_flag::depth),
        NAME_ENTRY(image_aspect_flag::stencil),
        NAME_ENTRY(image_aspect_flag::metadata),
        NAME_ENTRY(image_aspect_flag::plane_0),
        NAME_ENTRY(image_aspect_flag::plane_1),
        NAME_ENTRY(image_aspect_flag::plane_2),
        NAME_ENTRY(image_aspect_flag::memory_plane_0),
        NAME_ENTRY(image_aspect_flag::memory_plane_1),
        NAME_ENTRY(image_aspect_flag::memory_plane_2),
        NAME_ENTRY(image_aspect_flag::memory_plane_3),
    });

    enum class pipeline_stage_flag : ui32
    {
        none                                 = 0,
        none_khr                             = 0,
        top_of_pipe                          = 1 << 0,
        draw_indirect                        = 1 << 1,
        vertex_input                         = 1 << 2,
        vertex_shader                        = 1 << 3,
        tessellation_control_shader          = 1 << 4,
        tessellation_evaluation_shader       = 1 << 5,
        geometry_shader                      = 1 << 6,
        fragment_shader                      = 1 << 7,
        early_fragment_tests                 = 1 << 8,
        late_fragment_tests                  = 1 << 9,
        color_attachment_output              = 1 << 10,
        compute_shader                       = 1 << 11,
        transfer                             = 1 << 12,
        bottom_of_pipe                       = 1 << 13,
        host                                 = 1 << 14,
        all_graphics                         = 1 << 15,
        all_commands                         = 1 << 16,
        command_preprocess_nv                = 1 << 17,
        conditional_rendering_ext            = 1 << 18,
        task_shader_ext                      = 1 << 19,
        task_shader_nv                       = 1 << 19,
        mesh_shader_ext                      = 1 << 20,
        mesh_shader_nv                       = 1 << 20,
        ray_tracing_shader_khr               = 1 << 21,
        ray_tracing_shader_nv                = 1 << 21,
        fragment_shading_rate_attachment_khr = 1 << 22,
        shading_rate_image_nv                = 1 << 22,
        fragment_density_process_ext         = 1 << 23,
        transform_feedback_ext               = 1 << 24,
        acceleration_structure_build_khr     = 1 << 25,
        acceleration_structure_build_nv      = 1 << 25,
    };

    inline constexpr auto pipeline_stage_flag_name = create_name_map<pipeline_stage_flag>({
        NAME_ENTRY(pipeline_stage_flag::none),
        NAME_ENTRY(pipeline_stage_flag::none_khr),
        NAME_ENTRY(pipeline_stage_flag::top_of_pipe),
        NAME_ENTRY(pipeline_stage_flag::draw_indirect),
        NAME_ENTRY(pipeline_stage_flag::vertex_input),
        NAME_ENTRY(pipeline_stage_flag::vertex_shader),
        NAME_ENTRY(pipeline_stage_flag::tessellation_control_shader),
        NAME_ENTRY(pipeline_stage_flag::tessellation_evaluation_shader),
        NAME_ENTRY(pipeline_stage_flag::geometry_shader),
        NAME_ENTRY(pipeline_stage_flag::fragment_shader),
        NAME_ENTRY(pipeline_stage_flag::early_fragment_tests),
        NAME_ENTRY(pipeline_stage_flag::late_fragment_tests),
        NAME_ENTRY(pipeline_stage_flag::color_attachment_output),
        NAME_ENTRY(pipeline_stage_flag::compute_shader),
        NAME_ENTRY(pipeline_stage_flag::transfer),
        NAME_ENTRY(pipeline_stage_flag::bottom_of_pipe),
        NAME_ENTRY(pipeline_stage_flag::host),
        NAME_ENTRY(pipeline_stage_flag::all_graphics),
        NAME_ENTRY(pipeline_stage_flag::all_commands),
        NAME_ENTRY(pipeline_stage_flag::command_preprocess_nv),
        NAME_ENTRY(pipeline_stage_flag::conditional_rendering_ext),
        NAME_ENTRY(pipeline_stage_flag::task_shader_ext),
        NAME_ENTRY(pipeline_stage_flag::task_shader_nv),
        NAME_ENTRY(pipeline_stage_flag::mesh_shader_ext),
        NAME_ENTRY(pipeline_stage_flag::mesh_shader_nv),
        NAME_ENTRY(pipeline_stage_flag::ray_tracing_shader_khr),
        NAME_ENTRY(pipeline_stage_flag::ray_tracing_shader_nv),
        NAME_ENTRY(pipeline_stage_flag::fragment_shading_rate_attachment_khr),
        NAME_ENTRY(pipeline_stage_flag::shading_rate_image_nv),
        NAME_ENTRY(pipeline_stage_flag::fragment_density_process_ext),
        NAME_ENTRY(pipeline_stage_flag::transform_feedback_ext),
        NAME_ENTRY(pipeline_stage_flag::acceleration_structure_build_khr),
        NAME_ENTRY(pipeline_stage_flag::acceleration_structure_build_nv),
    });

    enum class format : ui32
    {
        undefined,
        r4g4_unorm_pack8,
        r4g4b4a4_unorm_pack16,
        b4g4r4a4_unorm_pack16,
        r5g6b5_unorm_pack16,
        b5g6r5_unorm_pack16,
        r5g5b5a1_unorm_pack16,
        b5g5r5a1_unorm_pack16,
        a1r5g5b5_unorm_pack16,
        r8_unorm,
        r8_snorm,
        r8_uscaled,
        r8_sscaled,
        r8_uint,
        r8_sint,
        r8_srgb,
        r8g8_unorm,
        r8g8_snorm,
        r8g8_uscaled,
        r8g8_sscaled,
        r8g8_uint,
        r8g8_sint,
        r8g8_srgb,
        r8g8b8_unorm,
        r8g8b8_snorm,
        r8g8b8_uscaled,
        r8g8b8_sscaled,
        r8g8b8_uint,
        r8g8b8_sint,
        r8g8b8_srgb,
        b8g8r8_unorm,
        b8g8r8_snorm,
        b8g8r8_uscaled,
        b8g8r8_sscaled,
        b8g8r8_uint,
        b8g8r8_sint,
        b8g8r8_srgb,
        r8g8b8a8_unorm,
        r8g8b8a8_snorm,
        r8g8b8a8_uscaled,
        r8g8b8a8_sscaled,
        r8g8b8a8_uint,
        r8g8b8a8_sint,
        r8g8b8a8_srgb,
        b8g8r8a8_unorm,
        b8g8r8a8_snorm,
        b8g8r8a8_uscaled,
        b8g8r8a8_sscaled,
        b8g8r8a8_uint,
        b8g8r8a8_sint,
        b8g8r8a8_srgb,
        a8b8g8r8_unorm_pack32,
        a8b8g8r8_snorm_pack32,
        a8b8g8r8_uscaled_pack32,
        a8b8g8r8_sscaled_pack32,
        a8b8g8r8_uint_pack32,
        a8b8g8r8_sint_pack32,
        a8b8g8r8_srgb_pack32,
        a2r10g10b10_unorm_pack32,
        a2r10g10b10_snorm_pack32,
        a2r10g10b10_uscaled_pack32,
        a2r10g10b10_sscaled_pack32,
        a2r10g10b10_uint_pack32,
        a2r10g10b10_sint_pack32,
        a2b10g10r10_unorm_pack32,
        a2b10g10r10_snorm_pack32,
        a2b10g10r10_uscaled_pack32,
        a2b10g10r10_sscaled_pack32,
        a2b10g10r10_uint_pack32,
        a2b10g10r10_sint_pack32,
        r16_unorm,
        r16_snorm,
        r16_uscaled,
        r16_sscaled,
        r16_uint,
        r16_sint,
        r16_sfloat,
        r16g16_unorm,
        r16g16_snorm,
        r16g16_uscaled,
        r16g16_sscaled,
        r16g16_uint,
        r16g16_sint,
        r16g16_sfloat,
        r16g16b16_unorm,
        r16g16b16_snorm,
        r16g16b16_uscaled,
        r16g16b16_sscaled,
        r16g16b16_uint,
        r16g16b16_sint,
        r16g16b16_sfloat,
        r16g16b16a16_unorm,
        r16g16b16a16_snorm,
        r16g16b16a16_uscaled,
        r16g16b16a16_sscaled,
        r16g16b16a16_uint,
        r16g16b16a16_sint,
        r16g16b16a16_sfloat,
        r32_uint,
        r32_sint,
        r32_sfloat,
        r32g32_uint,
        r32g32_sint,
        r32g32_sfloat,
        r32g32b32_uint,
        r32g32b32_sint,
        r32g32b32_sfloat,
        r32g32b32a32_uint,
        r32g32b32a32_sint,
        r32g32b32a32_sfloat,
        r64_uint,
        r64_sint,
        r64_sfloat,
        r64g64_uint,
        r64g64_sint,
        r64g64_sfloat,
        r64g64b64_uint,
        r64g64b64_sint,
        r64g64b64_sfloat,
        r64g64b64a64_uint,
        r64g64b64a64_sint,
        r64g64b64a64_sfloat,
        b10g11r11_ufloat_pack32,
        e5b9g9r9_ufloat_pack32,
        d16_unorm,
        x8_d24_unorm_pack32,
        d32_sfloat,
        s8_uint,
        d16_unorm_s8_uint,
        d24_unorm_s8_uint,
        d32_sfloat_s8_uint,
        bc1_rgb_unorm_block,
        bc1_rgb_srgb_block,
        bc1_rgba_unorm_block,
        bc1_rgba_srgb_block,
        bc2_unorm_block,
        bc2_srgb_block,
        bc3_unorm_block,
        bc3_srgb_block,
        bc4_unorm_block,
        bc4_snorm_block,
        bc5_unorm_block,
        bc5_snorm_block,
        bc6h_ufloat_block,
        bc6h_sfloat_block,
        bc7_unorm_block,
        bc7_srgb_block,
        etc2_r8g8b8_unorm_block,
        etc2_r8g8b8_srgb_block,
        etc2_r8g8b8a1_unorm_block,
        etc2_r8g8b8a1_srgb_block,
        etc2_r8g8b8a8_unorm_block,
        etc2_r8g8b8a8_srgb_block,
        eac_r11_unorm_block,
        eac_r11_snorm_block,
        eac_r11g11_unorm_block,
        eac_r11g11_snorm_block,
        astc_4x4_unorm_block,
        astc_4x4_srgb_block,
        astc_5x4_unorm_block,
        astc_5x4_srgb_block,
        astc_5x5_unorm_block,
        astc_5x5_srgb_block,
        astc_6x5_unorm_block,
        astc_6x5_srgb_block,
        astc_6x6_unorm_block,
        astc_6x6_srgb_block,
        astc_8x5_unorm_block,
        astc_8x5_srgb_block,
        astc_8x6_unorm_block,
        astc_8x6_srgb_block,
        astc_8x8_unorm_block,
        astc_8x8_srgb_block,
        astc_10x5_unorm_block,
        astc_10x5_srgb_block,
        astc_10x6_unorm_block,
        astc_10x6_srgb_block,
        astc_10x8_unorm_block,
        astc_10x8_srgb_block,
        astc_10x10_unorm_block,
        astc_10x10_srgb_block,
        astc_12x10_unorm_block,
        astc_12x10_srgb_block,
        astc_12x12_unorm_block,
        astc_12x12_srgb_block,
        g8b8g8r8_422_unorm = 1000156000,
        b8g8r8g8_422_unorm,
        g8_b8_r8_3plane_420_unorm,
        g8_b8r8_2plane_420_unorm,
        g8_b8_r8_3plane_422_unorm,
        g8_b8r8_2plane_422_unorm,
        g8_b8_r8_3plane_444_unorm,
        r10x6_unorm_pack16,
        r10x6g10x6_unorm_2pack16,
        r10x6g10x6b10x6a10x6_unorm_4pack16,
        g10x6b10x6g10x6r10x6_422_unorm_4pack16,
        b10x6g10x6r10x6g10x6_422_unorm_4pack16,
        g10x6_b10x6_r10x6_3plane_420_unorm_3pack16,
        g10x6_b10x6r10x6_2plane_420_unorm_3pack16,
        g10x6_b10x6_r10x6_3plane_422_unorm_3pack16,
        g10x6_b10x6r10x6_2plane_422_unorm_3pack16,
        g10x6_b10x6_r10x6_3plane_444_unorm_3pack16,
        r12x4_unorm_pack16,
        r12x4g12x4_unorm_2pack16,

        r12x4g12x4b12x4a12x4_unorm_4pack16,
        g12x4b12x4g12x4r12x4_422_unorm_4pack16,
        b12x4g12x4r12x4g12x4_422_unorm_4pack16,
        g12x4_b12x4_r12x4_3plane_420_unorm_3pack16,
        g12x4_b12x4r12x4_2plane_420_unorm_3pack16,
        g12x4_b12x4_r12x4_3plane_422_unorm_3pack16,
        g12x4_b12x4r12x4_2plane_422_unorm_3pack16,
        g12x4_b12x4_r12x4_3plane_444_unorm_3pack16,

        g16b16g16r16_422_unorm,
        b16g16r16g16_422_unorm,
        g16_b16_r16_3plane_420_unorm,
        g16_b16r16_2plane_420_unorm,
        g16_b16_r16_3plane_422_unorm,
        g16_b16r16_2plane_422_unorm,
        g16_b16_r16_3plane_444_unorm,
        g8_b8r8_2plane_444_unorm = 1000330000,
        g10x6_b10x6r10x6_2plane_444_unorm_3pack16,
        g12x4_b12x4r12x4_2plane_444_unorm_3pack16,
        g16_b16r16_2plane_444_unorm,
        a4r4g4b4_unorm_pack16 = 1000340000,
        a4b4g4r4_unorm_pack16,
        astc_4x4_sfloat_block = 1000066000,
        astc_5x4_sfloat_block,
        astc_5x5_sfloat_block,
        astc_6x5_sfloat_block,
        astc_6x6_sfloat_block,
        astc_8x5_sfloat_block,
        astc_8x6_sfloat_block,
        astc_8x8_sfloat_block,
        astc_10x5_sfloat_block,
        astc_10x6_sfloat_block,
        astc_10x8_sfloat_block,
        astc_10x10_sfloat_block,
        astc_12x10_sfloat_block,
        astc_12x12_sfloat_block,
        pvrtc1_2bpp_unorm_block_img = 1000054000,
        pvrtc1_4bpp_unorm_block_img,
        pvrtc2_2bpp_unorm_block_img,
        pvrtc2_4bpp_unorm_block_img,
        pvrtc1_2bpp_srgb_block_img,
        pvrtc1_4bpp_srgb_block_img,
        pvrtc2_2bpp_srgb_block_img,
        pvrtc2_4bpp_srgb_block_img,
        r16g16_s10_5_nv           = 1000464000,
        astc_4x4_sfloat_block_ext = astc_4x4_sfloat_block,
        astc_5x4_sfloat_block_ext,
        astc_5x5_sfloat_block_ext,
        astc_6x5_sfloat_block_ext,
        astc_6x6_sfloat_block_ext,
        astc_8x5_sfloat_block_ext,
        astc_8x6_sfloat_block_ext,
        astc_8x8_sfloat_block_ext,
        astc_10x5_sfloat_block_ext,
        astc_10x6_sfloat_block_ext,
        astc_10x8_sfloat_block_ext,
        astc_10x10_sfloat_block_ext,
        astc_12x10_sfloat_block_ext,
        astc_12x12_sfloat_block_ext,
        g8b8g8r8_422_unorm_khr = g8b8g8r8_422_unorm,
        b8g8r8g8_422_unorm_khr,
        g8_b8_r8_3plane_420_unorm_khr,
        g8_b8r8_2plane_420_unorm_khr,
        g8_b8_r8_3plane_422_unorm_khr,
        g8_b8r8_2plane_422_unorm_khr,
        g8_b8_r8_3plane_444_unorm_khr,
        r10x6_unorm_pack16_khr,
        r10x6g10x6_unorm_2pack16_khr,

        r10x6g10x6b10x6a10x6_unorm_4pack16_khr,
        g10x6b10x6g10x6r10x6_422_unorm_4pack16_khr,
        b10x6g10x6r10x6g10x6_422_unorm_4pack16_khr,
        g10x6_b10x6_r10x6_3plane_420_unorm_3pack16_khr,
        g10x6_b10x6r10x6_2plane_420_unorm_3pack16_khr,
        g10x6_b10x6_r10x6_3plane_422_unorm_3pack16_khr,
        g10x6_b10x6r10x6_2plane_422_unorm_3pack16_khr,
        g10x6_b10x6_r10x6_3plane_444_unorm_3pack16_khr,
        r12x4_unorm_pack16_khr,
        r12x4g12x4_unorm_2pack16_khr,
        r12x4g12x4b12x4a12x4_unorm_4pack16_khr,
        g12x4b12x4g12x4r12x4_422_unorm_4pack16_khr,
        b12x4g12x4r12x4g12x4_422_unorm_4pack16_khr,
        g12x4_b12x4_r12x4_3plane_420_unorm_3pack16_khr,
        g12x4_b12x4r12x4_2plane_420_unorm_3pack16_khr,
        g12x4_b12x4_r12x4_3plane_422_unorm_3pack16_khr,
        g12x4_b12x4r12x4_2plane_422_unorm_3pack16_khr,
        g12x4_b12x4_r12x4_3plane_444_unorm_3pack16_khr,
        g10x6_b10x6r10x6_2plane_444_unorm_3pack16_ext = g10x6_b10x6r10x6_2plane_444_unorm_3pack16,
        g12x4_b12x4r12x4_2plane_444_unorm_3pack16_ext,

        g16b16g16r16_422_unorm_khr = g16b16g16r16_422_unorm,
        b16g16r16g16_422_unorm_khr,
        g16_b16_r16_3plane_420_unorm_khr,
        g16_b16r16_2plane_420_unorm_khr,
        g16_b16_r16_3plane_422_unorm_khr,
        g16_b16r16_2plane_422_unorm_khr,
        g16_b16_r16_3plane_444_unorm_khr,
        g8_b8r8_2plane_444_unorm_ext    = g8_b8r8_2plane_444_unorm,
        g16_b16r16_2plane_444_unorm_ext = g16_b16r16_2plane_444_unorm,
        a4r4g4b4_unorm_pack16_ext       = a4r4g4b4_unorm_pack16,
        a4b4g4r4_unorm_pack16_ext,
    };

    inline constexpr auto format_names = create_name_map<format>({
        NAME_ENTRY(format::undefined),
        NAME_ENTRY(format::r4g4_unorm_pack8),
        NAME_ENTRY(format::r4g4b4a4_unorm_pack16),
        NAME_ENTRY(format::b4g4r4a4_unorm_pack16),
        NAME_ENTRY(format::r5g6b5_unorm_pack16),
        NAME_ENTRY(format::b5g6r5_unorm_pack16),
        NAME_ENTRY(format::r5g5b5a1_unorm_pack16),
        NAME_ENTRY(format::b5g5r5a1_unorm_pack16),
        NAME_ENTRY(format::a1r5g5b5_unorm_pack16),
        NAME_ENTRY(format::r8_unorm),
        NAME_ENTRY(format::r8_snorm),
        NAME_ENTRY(format::r8_uscaled),
        NAME_ENTRY(format::r8_sscaled),
        NAME_ENTRY(format::r8_uint),
        NAME_ENTRY(format::r8_sint),
        NAME_ENTRY(format::r8_srgb),
        NAME_ENTRY(format::r8g8_unorm),
        NAME_ENTRY(format::r8g8_snorm),
        NAME_ENTRY(format::r8g8_uscaled),
        NAME_ENTRY(format::r8g8_sscaled),
        NAME_ENTRY(format::r8g8_uint),
        NAME_ENTRY(format::r8g8_sint),
        NAME_ENTRY(format::r8g8_srgb),
        NAME_ENTRY(format::r8g8b8_unorm),
        NAME_ENTRY(format::r8g8b8_snorm),
        NAME_ENTRY(format::r8g8b8_uscaled),
        NAME_ENTRY(format::r8g8b8_sscaled),
        NAME_ENTRY(format::r8g8b8_uint),
        NAME_ENTRY(format::r8g8b8_sint),
        NAME_ENTRY(format::r8g8b8_srgb),
        NAME_ENTRY(format::b8g8r8_unorm),
        NAME_ENTRY(format::b8g8r8_snorm),
        NAME_ENTRY(format::b8g8r8_uscaled),
        NAME_ENTRY(format::b8g8r8_sscaled),
        NAME_ENTRY(format::b8g8r8_uint),
        NAME_ENTRY(format::b8g8r8_sint),
        NAME_ENTRY(format::b8g8r8_srgb),
        NAME_ENTRY(format::r8g8b8a8_unorm),
        NAME_ENTRY(format::r8g8b8a8_snorm),
        NAME_ENTRY(format::r8g8b8a8_uscaled),
        NAME_ENTRY(format::r8g8b8a8_sscaled),
        NAME_ENTRY(format::r8g8b8a8_uint),
        NAME_ENTRY(format::r8g8b8a8_sint),
        NAME_ENTRY(format::r8g8b8a8_srgb),
        NAME_ENTRY(format::b8g8r8a8_unorm),
        NAME_ENTRY(format::b8g8r8a8_snorm),
        NAME_ENTRY(format::b8g8r8a8_uscaled),
        NAME_ENTRY(format::b8g8r8a8_sscaled),
        NAME_ENTRY(format::b8g8r8a8_uint),
        NAME_ENTRY(format::b8g8r8a8_sint),
        NAME_ENTRY(format::b8g8r8a8_srgb),
        NAME_ENTRY(format::a8b8g8r8_unorm_pack32),
        NAME_ENTRY(format::a8b8g8r8_snorm_pack32),
        NAME_ENTRY(format::a8b8g8r8_uscaled_pack32),
        NAME_ENTRY(format::a8b8g8r8_sscaled_pack32),
        NAME_ENTRY(format::a8b8g8r8_uint_pack32),
        NAME_ENTRY(format::a8b8g8r8_sint_pack32),
        NAME_ENTRY(format::a8b8g8r8_srgb_pack32),
        NAME_ENTRY(format::a2r10g10b10_unorm_pack32),
        NAME_ENTRY(format::a2r10g10b10_snorm_pack32),
        NAME_ENTRY(format::a2r10g10b10_uscaled_pack32),
        NAME_ENTRY(format::a2r10g10b10_sscaled_pack32),
        NAME_ENTRY(format::a2r10g10b10_uint_pack32),
        NAME_ENTRY(format::a2r10g10b10_sint_pack32),
        NAME_ENTRY(format::a2b10g10r10_unorm_pack32),
        NAME_ENTRY(format::a2b10g10r10_snorm_pack32),
        NAME_ENTRY(format::a2b10g10r10_uscaled_pack32),
        NAME_ENTRY(format::a2b10g10r10_sscaled_pack32),
        NAME_ENTRY(format::a2b10g10r10_uint_pack32),
        NAME_ENTRY(format::a2b10g10r10_sint_pack32),
        NAME_ENTRY(format::r16_unorm),
        NAME_ENTRY(format::r16_snorm),
        NAME_ENTRY(format::r16_uscaled),
        NAME_ENTRY(format::r16_sscaled),
        NAME_ENTRY(format::r16_uint),
        NAME_ENTRY(format::r16_sint),
        NAME_ENTRY(format::r16_sfloat),
        NAME_ENTRY(format::r16g16_unorm),
        NAME_ENTRY(format::r16g16_snorm),
        NAME_ENTRY(format::r16g16_uscaled),
        NAME_ENTRY(format::r16g16_sscaled),
        NAME_ENTRY(format::r16g16_uint),
        NAME_ENTRY(format::r16g16_sint),
        NAME_ENTRY(format::r16g16_sfloat),
        NAME_ENTRY(format::r16g16b16_unorm),
        NAME_ENTRY(format::r16g16b16_snorm),
        NAME_ENTRY(format::r16g16b16_uscaled),
        NAME_ENTRY(format::r16g16b16_sscaled),
        NAME_ENTRY(format::r16g16b16_uint),
        NAME_ENTRY(format::r16g16b16_sint),
        NAME_ENTRY(format::r16g16b16_sfloat),
        NAME_ENTRY(format::r16g16b16a16_unorm),
        NAME_ENTRY(format::r16g16b16a16_snorm),
        NAME_ENTRY(format::r16g16b16a16_uscaled),
        NAME_ENTRY(format::r16g16b16a16_sscaled),
        NAME_ENTRY(format::r16g16b16a16_uint),
        NAME_ENTRY(format::r16g16b16a16_sint),
        NAME_ENTRY(format::r16g16b16a16_sfloat),
        NAME_ENTRY(format::r32_uint),
        NAME_ENTRY(format::r32_sint),
        NAME_ENTRY(format::r32_sfloat),
        NAME_ENTRY(format::r32g32_uint),
        NAME_ENTRY(format::r32g32_sint),
        NAME_ENTRY(format::r32g32_sfloat),
        NAME_ENTRY(format::r32g32b32_uint),
        NAME_ENTRY(format::r32g32b32_sint),
        NAME_ENTRY(format::r32g32b32_sfloat),
        NAME_ENTRY(format::r32g32b32a32_uint),
        NAME_ENTRY(format::r32g32b32a32_sint),
        NAME_ENTRY(format::r32g32b32a32_sfloat),
        NAME_ENTRY(format::r64_uint),
        NAME_ENTRY(format::r64_sint),
        NAME_ENTRY(format::r64_sfloat),
        NAME_ENTRY(format::r64g64_uint),
        NAME_ENTRY(format::r64g64_sint),
        NAME_ENTRY(format::r64g64_sfloat),
        NAME_ENTRY(format::r64g64b64_uint),
        NAME_ENTRY(format::r64g64b64_sint),
        NAME_ENTRY(format::r64g64b64_sfloat),
        NAME_ENTRY(format::r64g64b64a64_uint),
        NAME_ENTRY(format::r64g64b64a64_sint),
        NAME_ENTRY(format::r64g64b64a64_sfloat),
        NAME_ENTRY(format::b10g11r11_ufloat_pack32),
        NAME_ENTRY(format::e5b9g9r9_ufloat_pack32),
        NAME_ENTRY(format::d16_unorm),
        NAME_ENTRY(format::x8_d24_unorm_pack32),
        NAME_ENTRY(format::d32_sfloat),
        NAME_ENTRY(format::s8_uint),
        NAME_ENTRY(format::d16_unorm_s8_uint),
        NAME_ENTRY(format::d24_unorm_s8_uint),
        NAME_ENTRY(format::d32_sfloat_s8_uint),
        NAME_ENTRY(format::bc1_rgb_unorm_block),
        NAME_ENTRY(format::bc1_rgb_srgb_block),
        NAME_ENTRY(format::bc1_rgba_unorm_block),
        NAME_ENTRY(format::bc1_rgba_srgb_block),
        NAME_ENTRY(format::bc2_unorm_block),
        NAME_ENTRY(format::bc2_srgb_block),
        NAME_ENTRY(format::bc3_unorm_block),
        NAME_ENTRY(format::bc3_srgb_block),
        NAME_ENTRY(format::bc4_unorm_block),
        NAME_ENTRY(format::bc4_snorm_block),
        NAME_ENTRY(format::bc5_unorm_block),
        NAME_ENTRY(format::bc5_snorm_block),
        NAME_ENTRY(format::bc6h_ufloat_block),
        NAME_ENTRY(format::bc6h_sfloat_block),
        NAME_ENTRY(format::bc7_unorm_block),
        NAME_ENTRY(format::bc7_srgb_block),
        NAME_ENTRY(format::etc2_r8g8b8_unorm_block),
        NAME_ENTRY(format::etc2_r8g8b8_srgb_block),
        NAME_ENTRY(format::etc2_r8g8b8a1_unorm_block),
        NAME_ENTRY(format::etc2_r8g8b8a1_srgb_block),
        NAME_ENTRY(format::etc2_r8g8b8a8_unorm_block),
        NAME_ENTRY(format::etc2_r8g8b8a8_srgb_block),
        NAME_ENTRY(format::eac_r11_unorm_block),
        NAME_ENTRY(format::eac_r11_snorm_block),
        NAME_ENTRY(format::eac_r11g11_unorm_block),
        NAME_ENTRY(format::eac_r11g11_snorm_block),
        NAME_ENTRY(format::astc_4x4_unorm_block),
        NAME_ENTRY(format::astc_4x4_srgb_block),
        NAME_ENTRY(format::astc_5x4_unorm_block),
        NAME_ENTRY(format::astc_5x4_srgb_block),
        NAME_ENTRY(format::astc_5x5_unorm_block),
        NAME_ENTRY(format::astc_5x5_srgb_block),
        NAME_ENTRY(format::astc_6x5_unorm_block),
        NAME_ENTRY(format::astc_6x5_srgb_block),
        NAME_ENTRY(format::astc_6x6_unorm_block),
        NAME_ENTRY(format::astc_6x6_srgb_block),
        NAME_ENTRY(format::astc_8x5_unorm_block),
        NAME_ENTRY(format::astc_8x5_srgb_block),
        NAME_ENTRY(format::astc_8x6_unorm_block),
        NAME_ENTRY(format::astc_8x6_srgb_block),
        NAME_ENTRY(format::astc_8x8_unorm_block),
        NAME_ENTRY(format::astc_8x8_srgb_block),
        NAME_ENTRY(format::astc_10x5_unorm_block),
        NAME_ENTRY(format::astc_10x5_srgb_block),
        NAME_ENTRY(format::astc_10x6_unorm_block),
        NAME_ENTRY(format::astc_10x6_srgb_block),
        NAME_ENTRY(format::astc_10x8_unorm_block),
        NAME_ENTRY(format::astc_10x8_srgb_block),
        NAME_ENTRY(format::astc_10x10_unorm_block),
        NAME_ENTRY(format::astc_10x10_srgb_block),
        NAME_ENTRY(format::astc_12x10_unorm_block),
        NAME_ENTRY(format::astc_12x10_srgb_block),
        NAME_ENTRY(format::astc_12x12_unorm_block),
        NAME_ENTRY(format::astc_12x12_srgb_block),
        NAME_ENTRY(format::g8b8g8r8_422_unorm),
        NAME_ENTRY(format::b8g8r8g8_422_unorm),
        NAME_ENTRY(format::g8_b8_r8_3plane_420_unorm),
        NAME_ENTRY(format::g8_b8r8_2plane_420_unorm),
        NAME_ENTRY(format::g8_b8_r8_3plane_422_unorm),
        NAME_ENTRY(format::g8_b8r8_2plane_422_unorm),
        NAME_ENTRY(format::g8_b8_r8_3plane_444_unorm),
        NAME_ENTRY(format::r10x6_unorm_pack16),
        NAME_ENTRY(format::r10x6g10x6_unorm_2pack16),
        NAME_ENTRY(format::r10x6g10x6b10x6a10x6_unorm_4pack16),
        NAME_ENTRY(format::g10x6b10x6g10x6r10x6_422_unorm_4pack16),
        NAME_ENTRY(format::b10x6g10x6r10x6g10x6_422_unorm_4pack16),
        NAME_ENTRY(format::g10x6_b10x6_r10x6_3plane_420_unorm_3pack16),
        NAME_ENTRY(format::g10x6_b10x6r10x6_2plane_420_unorm_3pack16),
        NAME_ENTRY(format::g10x6_b10x6_r10x6_3plane_422_unorm_3pack16),
        NAME_ENTRY(format::g10x6_b10x6r10x6_2plane_422_unorm_3pack16),
        NAME_ENTRY(format::g10x6_b10x6_r10x6_3plane_444_unorm_3pack16),
        NAME_ENTRY(format::r12x4_unorm_pack16),
        NAME_ENTRY(format::r12x4g12x4_unorm_2pack16),

        NAME_ENTRY(format::r12x4g12x4b12x4a12x4_unorm_4pack16),
        NAME_ENTRY(format::g12x4b12x4g12x4r12x4_422_unorm_4pack16),
        NAME_ENTRY(format::b12x4g12x4r12x4g12x4_422_unorm_4pack16),
        NAME_ENTRY(format::g12x4_b12x4_r12x4_3plane_420_unorm_3pack16),
        NAME_ENTRY(format::g12x4_b12x4r12x4_2plane_420_unorm_3pack16),
        NAME_ENTRY(format::g12x4_b12x4_r12x4_3plane_422_unorm_3pack16),
        NAME_ENTRY(format::g12x4_b12x4r12x4_2plane_422_unorm_3pack16),
        NAME_ENTRY(format::g12x4_b12x4_r12x4_3plane_444_unorm_3pack16),

        NAME_ENTRY(format::g16b16g16r16_422_unorm),
        NAME_ENTRY(format::b16g16r16g16_422_unorm),
        NAME_ENTRY(format::g16_b16_r16_3plane_420_unorm),
        NAME_ENTRY(format::g16_b16r16_2plane_420_unorm),
        NAME_ENTRY(format::g16_b16_r16_3plane_422_unorm),
        NAME_ENTRY(format::g16_b16r16_2plane_422_unorm),
        NAME_ENTRY(format::g16_b16_r16_3plane_444_unorm),
        NAME_ENTRY(format::g8_b8r8_2plane_444_unorm),
        NAME_ENTRY(format::g10x6_b10x6r10x6_2plane_444_unorm_3pack16),
        NAME_ENTRY(format::g12x4_b12x4r12x4_2plane_444_unorm_3pack16),
        NAME_ENTRY(format::g16_b16r16_2plane_444_unorm),
        NAME_ENTRY(format::a4r4g4b4_unorm_pack16),
        NAME_ENTRY(format::a4b4g4r4_unorm_pack16),
        NAME_ENTRY(format::astc_4x4_sfloat_block),
        NAME_ENTRY(format::astc_5x4_sfloat_block),
        NAME_ENTRY(format::astc_5x5_sfloat_block),
        NAME_ENTRY(format::astc_6x5_sfloat_block),
        NAME_ENTRY(format::astc_6x6_sfloat_block),
        NAME_ENTRY(format::astc_8x5_sfloat_block),
        NAME_ENTRY(format::astc_8x6_sfloat_block),
        NAME_ENTRY(format::astc_8x8_sfloat_block),
        NAME_ENTRY(format::astc_10x5_sfloat_block),
        NAME_ENTRY(format::astc_10x6_sfloat_block),
        NAME_ENTRY(format::astc_10x8_sfloat_block),
        NAME_ENTRY(format::astc_10x10_sfloat_block),
        NAME_ENTRY(format::astc_12x10_sfloat_block),
        NAME_ENTRY(format::astc_12x12_sfloat_block),
        NAME_ENTRY(format::pvrtc1_2bpp_unorm_block_img),
        NAME_ENTRY(format::pvrtc1_4bpp_unorm_block_img),
        NAME_ENTRY(format::pvrtc2_2bpp_unorm_block_img),
        NAME_ENTRY(format::pvrtc2_4bpp_unorm_block_img),
        NAME_ENTRY(format::pvrtc1_2bpp_srgb_block_img),
        NAME_ENTRY(format::pvrtc1_4bpp_srgb_block_img),
        NAME_ENTRY(format::pvrtc2_2bpp_srgb_block_img),
        NAME_ENTRY(format::pvrtc2_4bpp_srgb_block_img),
        NAME_ENTRY(format::r16g16_s10_5_nv),
        NAME_ENTRY(format::astc_4x4_sfloat_block_ext),
        NAME_ENTRY(format::astc_5x4_sfloat_block_ext),
        NAME_ENTRY(format::astc_5x5_sfloat_block_ext),
        NAME_ENTRY(format::astc_6x5_sfloat_block_ext),
        NAME_ENTRY(format::astc_6x6_sfloat_block_ext),
        NAME_ENTRY(format::astc_8x5_sfloat_block_ext),
        NAME_ENTRY(format::astc_8x6_sfloat_block_ext),
        NAME_ENTRY(format::astc_8x8_sfloat_block_ext),
        NAME_ENTRY(format::astc_10x5_sfloat_block_ext),
        NAME_ENTRY(format::astc_10x6_sfloat_block_ext),
        NAME_ENTRY(format::astc_10x8_sfloat_block_ext),
        NAME_ENTRY(format::astc_10x10_sfloat_block_ext),
        NAME_ENTRY(format::astc_12x10_sfloat_block_ext),
        NAME_ENTRY(format::astc_12x12_sfloat_block_ext),
        NAME_ENTRY(format::g8b8g8r8_422_unorm_khr),
        NAME_ENTRY(format::b8g8r8g8_422_unorm_khr),
        NAME_ENTRY(format::g8_b8_r8_3plane_420_unorm_khr),
        NAME_ENTRY(format::g8_b8r8_2plane_420_unorm_khr),
        NAME_ENTRY(format::g8_b8_r8_3plane_422_unorm_khr),
        NAME_ENTRY(format::g8_b8r8_2plane_422_unorm_khr),
        NAME_ENTRY(format::g8_b8_r8_3plane_444_unorm_khr),
        NAME_ENTRY(format::r10x6_unorm_pack16_khr),
        NAME_ENTRY(format::r10x6g10x6_unorm_2pack16_khr),

        NAME_ENTRY(format::r10x6g10x6b10x6a10x6_unorm_4pack16_khr),
        NAME_ENTRY(format::g10x6b10x6g10x6r10x6_422_unorm_4pack16_khr),
        NAME_ENTRY(format::b10x6g10x6r10x6g10x6_422_unorm_4pack16_khr),
        NAME_ENTRY(format::g10x6_b10x6_r10x6_3plane_420_unorm_3pack16_khr),
        NAME_ENTRY(format::g10x6_b10x6r10x6_2plane_420_unorm_3pack16_khr),
        NAME_ENTRY(format::g10x6_b10x6_r10x6_3plane_422_unorm_3pack16_khr),
        NAME_ENTRY(format::g10x6_b10x6r10x6_2plane_422_unorm_3pack16_khr),
        NAME_ENTRY(format::g10x6_b10x6_r10x6_3plane_444_unorm_3pack16_khr),
        NAME_ENTRY(format::r12x4_unorm_pack16_khr),
        NAME_ENTRY(format::r12x4g12x4_unorm_2pack16_khr),
        NAME_ENTRY(format::r12x4g12x4b12x4a12x4_unorm_4pack16_khr),
        NAME_ENTRY(format::g12x4b12x4g12x4r12x4_422_unorm_4pack16_khr),
        NAME_ENTRY(format::b12x4g12x4r12x4g12x4_422_unorm_4pack16_khr),
        NAME_ENTRY(format::g12x4_b12x4_r12x4_3plane_420_unorm_3pack16_khr),
        NAME_ENTRY(format::g12x4_b12x4r12x4_2plane_420_unorm_3pack16_khr),
        NAME_ENTRY(format::g12x4_b12x4_r12x4_3plane_422_unorm_3pack16_khr),
        NAME_ENTRY(format::g12x4_b12x4r12x4_2plane_422_unorm_3pack16_khr),
        NAME_ENTRY(format::g12x4_b12x4_r12x4_3plane_444_unorm_3pack16_khr),
        NAME_ENTRY(format::g10x6_b10x6r10x6_2plane_444_unorm_3pack16_ext),
        NAME_ENTRY(format::g12x4_b12x4r12x4_2plane_444_unorm_3pack16_ext),

        NAME_ENTRY(format::g16b16g16r16_422_unorm_khr),
        NAME_ENTRY(format::b16g16r16g16_422_unorm_khr),
        NAME_ENTRY(format::g16_b16_r16_3plane_420_unorm_khr),
        NAME_ENTRY(format::g16_b16r16_2plane_420_unorm_khr),
        NAME_ENTRY(format::g16_b16_r16_3plane_422_unorm_khr),
        NAME_ENTRY(format::g16_b16r16_2plane_422_unorm_khr),
        NAME_ENTRY(format::g16_b16_r16_3plane_444_unorm_khr),
        NAME_ENTRY(format::g8_b8r8_2plane_444_unorm_ext),
        NAME_ENTRY(format::g16_b16r16_2plane_444_unorm_ext),
        NAME_ENTRY(format::a4r4g4b4_unorm_pack16_ext),
        NAME_ENTRY(format::a4b4g4r4_unorm_pack16_ext),

    });

    enum class present_mode : ui32
    {
        immediate_khr,
        mailbox_khr,
        fifo_khr,
        fifo_relaxed_khr,
        shared_demand_refresh_khr = 1000111000,
        shared_continuous_refresh_khr,
    };

    inline constexpr auto present_mode_names = create_name_map<present_mode>({
        NAME_ENTRY(present_mode::immediate_khr),
        NAME_ENTRY(present_mode::mailbox_khr),
        NAME_ENTRY(present_mode::fifo_khr),
        NAME_ENTRY(present_mode::fifo_relaxed_khr),
        NAME_ENTRY(present_mode::shared_demand_refresh_khr),
        NAME_ENTRY(present_mode::shared_continuous_refresh_khr),
    });

    enum class color_space : ui32
    {
        srgb_nonlinear_khr,
        display_p3_nonlinear_ext = 1000104001,
        extended_srgb_linear_ext,
        display_p3_linear_ext,
        dci_p3_nonlinear_ext,
        bt709_linear_ext,
        bt709_nonlinear_ext,
        bt2020_linear_ext,
        hdr10_st2084_ext,
        dolbyvision_ext,
        hdr10_hlg_ext,
        adobergb_linear_ext,
        adobergb_nonlinear_ext,
        pass_through_ext,
        extended_srgb_nonlinear_ext,
        display_native_amd = 1000213000,
        dci_p3_linear_ext  = display_p3_linear_ext,
    };

    inline constexpr auto color_space_names = create_name_map<color_space>({
        NAME_ENTRY(color_space::srgb_nonlinear_khr),
        NAME_ENTRY(color_space::display_p3_nonlinear_ext),
        NAME_ENTRY(color_space::extended_srgb_linear_ext),
        NAME_ENTRY(color_space::display_p3_linear_ext),
        NAME_ENTRY(color_space::dci_p3_nonlinear_ext),
        NAME_ENTRY(color_space::bt709_linear_ext),
        NAME_ENTRY(color_space::bt709_nonlinear_ext),
        NAME_ENTRY(color_space::bt2020_linear_ext),
        NAME_ENTRY(color_space::hdr10_st2084_ext),
        NAME_ENTRY(color_space::dolbyvision_ext),
        NAME_ENTRY(color_space::hdr10_hlg_ext),
        NAME_ENTRY(color_space::adobergb_linear_ext),
        NAME_ENTRY(color_space::adobergb_nonlinear_ext),
        NAME_ENTRY(color_space::pass_through_ext),
        NAME_ENTRY(color_space::extended_srgb_nonlinear_ext),
        NAME_ENTRY(color_space::display_native_amd),
        NAME_ENTRY(color_space::dci_p3_linear_ext),
    });

    enum class image_usage_flag : ui32
    {
        transfer_src                         = 1 << 0,
        transfer_dst                         = 1 << 1,
        sampled                              = 1 << 2,
        storage                              = 1 << 3,
        color_attachment                     = 1 << 4,
        depth_stencil_attachment             = 1 << 5,
        transient_attachment                 = 1 << 6,
        input_attachment                     = 1 << 7,
        fragment_shading_rate_attachment_khr = 1 << 8,
        shading_rate_image_nv                = fragment_shading_rate_attachment_khr,
        fragment_density_map_ext             = 1 << 9,
        video_decode_dst_khr                 = 1 << 10,
        video_decode_src_khr                 = 1 << 11,
        video_decode_dpb_khr                 = 1 << 12,
        video_encode_dst_khr                 = 1 << 13,
        video_encode_src_khr                 = 1 << 14,
        video_encode_dpb_khr                 = 1 << 15,
        invocation_mask_huawei               = 1 << 18,
        attachment_feedback_loop_ext         = 1 << 19,
        sample_weight_qcom                   = 1 << 20,
        sample_block_match_qcom              = 1 << 21,
    };

    inline constexpr auto image_usage_flag_names = create_name_map<image_usage_flag>({
        NAME_ENTRY(image_usage_flag::transfer_src),
        NAME_ENTRY(image_usage_flag::transfer_dst),
        NAME_ENTRY(image_usage_flag::sampled),
        NAME_ENTRY(image_usage_flag::storage),
        NAME_ENTRY(image_usage_flag::color_attachment),
        NAME_ENTRY(image_usage_flag::depth_stencil_attachment),
        NAME_ENTRY(image_usage_flag::transient_attachment),
        NAME_ENTRY(image_usage_flag::input_attachment),
        NAME_ENTRY(image_usage_flag::fragment_shading_rate_attachment_khr),
        NAME_ENTRY(image_usage_flag::shading_rate_image_nv),
        NAME_ENTRY(image_usage_flag::fragment_density_map_ext),
        NAME_ENTRY(image_usage_flag::video_decode_dst_khr),
        NAME_ENTRY(image_usage_flag::video_decode_src_khr),
        NAME_ENTRY(image_usage_flag::video_decode_dpb_khr),
        NAME_ENTRY(image_usage_flag::video_encode_dst_khr),
        NAME_ENTRY(image_usage_flag::video_encode_src_khr),
        NAME_ENTRY(image_usage_flag::video_encode_dpb_khr),
        NAME_ENTRY(image_usage_flag::invocation_mask_huawei),
        NAME_ENTRY(image_usage_flag::attachment_feedback_loop_ext),
        NAME_ENTRY(image_usage_flag::sample_weight_qcom),
        NAME_ENTRY(image_usage_flag::sample_block_match_qcom),
    });

    enum class sharing_mode : ui32
    {
        exclusive,
        concurrent,
    };

    inline constexpr auto sharing_mode_names = create_name_map<sharing_mode>({
        NAME_ENTRY(sharing_mode::exclusive),
        NAME_ENTRY(sharing_mode::concurrent),
    });

    enum class surface_transform_flag : ui32
    {
        identity_khr                     = 1 << 0,
        rotate_90_khr                    = 1 << 1,
        rotate_180_khr                   = 1 << 2,
        rotate_270_khr                   = 1 << 3,
        horizontal_mirror_khr            = 1 << 4,
        horizontal_mirror_rotate_90_khr  = 1 << 5,
        horizontal_mirror_rotate_180_khr = 1 << 6,
        horizontal_mirror_rotate_270_khr = 1 << 7,
        inherit_khr                      = 1 << 8,
    };

    inline constexpr auto surface_transform_flag_names = create_name_map<surface_transform_flag>({
        NAME_ENTRY(surface_transform_flag::identity_khr),
        NAME_ENTRY(surface_transform_flag::rotate_90_khr),
        NAME_ENTRY(surface_transform_flag::rotate_180_khr),
        NAME_ENTRY(surface_transform_flag::rotate_270_khr),
        NAME_ENTRY(surface_transform_flag::horizontal_mirror_khr),
        NAME_ENTRY(surface_transform_flag::horizontal_mirror_rotate_90_khr),
        NAME_ENTRY(surface_transform_flag::horizontal_mirror_rotate_180_khr),
        NAME_ENTRY(surface_transform_flag::horizontal_mirror_rotate_270_khr),
        NAME_ENTRY(surface_transform_flag::inherit_khr),
    });

    enum class composite_alpha_flag : ui32
    {
        opaque_khr          = 1 << 0,
        pre_multiplied_khr  = 1 << 1,
        post_multiplied_khr = 1 << 2,
        inherit_khr         = 1 << 3,
    };

    inline constexpr auto composite_alpha_flag_names = create_name_map<composite_alpha_flag>({
        NAME_ENTRY(composite_alpha_flag::opaque_khr),
        NAME_ENTRY(composite_alpha_flag::pre_multiplied_khr),
        NAME_ENTRY(composite_alpha_flag::post_multiplied_khr),
        NAME_ENTRY(composite_alpha_flag::inherit_khr),
    });

    enum class image_tiling : ui32
    {
        optimal,
        linear,
        drm_format_modifier_ext = 1000158000,
    };

    inline constexpr auto image_tiling_names = create_name_map<image_tiling>({
        NAME_ENTRY(image_tiling::optimal),
        NAME_ENTRY(image_tiling::linear),
        NAME_ENTRY(image_tiling::drm_format_modifier_ext),
    });

    enum class image_type : ui32
    {
        _1d,
        _2d,
        _3d,
    };

    inline constexpr auto image_type_names = create_name_map<image_type>({
        NAME_ENTRY(image_type::_1d),
        NAME_ENTRY(image_type::_2d),
        NAME_ENTRY(image_type::_3d),
    });

    enum class image_view_type : ui32
    {
        _1d,
        _2d,
        _3d,
        cube,
        _1d_array,
        _2d_array,
        cube_array,
    };

    inline constexpr auto image_view_type_names = create_name_map<image_view_type>({
        NAME_ENTRY(image_view_type::_1d),
        NAME_ENTRY(image_view_type::_2d),
        NAME_ENTRY(image_view_type::_3d),
        NAME_ENTRY(image_view_type::cube),
        NAME_ENTRY(image_view_type::_1d_array),
        NAME_ENTRY(image_view_type::_2d_array),
        NAME_ENTRY(image_view_type::cube_array),
    });

    enum class component_swizzle : ui32
    {
        identity,
        zero,
        one,
        r,
        g,
        b,
        a,
    };

    inline constexpr auto component_swizzle_names = create_name_map<component_swizzle>({
        NAME_ENTRY(component_swizzle::identity),
        NAME_ENTRY(component_swizzle::zero),
        NAME_ENTRY(component_swizzle::one),
        NAME_ENTRY(component_swizzle::r),
        NAME_ENTRY(component_swizzle::g),
        NAME_ENTRY(component_swizzle::b),
        NAME_ENTRY(component_swizzle::a),
    });

    enum class sample_count_flag : ui32
    {
        _1  = 1 << 0,
        _2  = 1 << 1,
        _4  = 1 << 2,
        _8  = 1 << 3,
        _16 = 1 << 4,
        _32 = 1 << 5,
        _64 = 1 << 6,
    };

    inline constexpr auto sample_count_flag_names = create_name_map<sample_count_flag>({
        NAME_ENTRY(sample_count_flag::_1),
        NAME_ENTRY(sample_count_flag::_2),
        NAME_ENTRY(sample_count_flag::_4),
        NAME_ENTRY(sample_count_flag::_8),
        NAME_ENTRY(sample_count_flag::_16),
        NAME_ENTRY(sample_count_flag::_32),
        NAME_ENTRY(sample_count_flag::_64),
    });

    enum class descriptor_pool_create_flag
    {
        free_descriptor_set   = 1 << 0,
        update_after_bind     = 1 << 1,
        update_after_bind_ext = 1 << 1,
        host_only_ext         = 1 << 2,
        host_only_valve       = 1 << 2,
    };

    inline constexpr auto descriptor_pool_create_flag_names = create_name_map<descriptor_pool_create_flag>({
        NAME_ENTRY(descriptor_pool_create_flag::free_descriptor_set),
        NAME_ENTRY(descriptor_pool_create_flag::update_after_bind),
        NAME_ENTRY(descriptor_pool_create_flag::update_after_bind_ext),
        NAME_ENTRY(descriptor_pool_create_flag::host_only_ext),
        NAME_ENTRY(descriptor_pool_create_flag::host_only_valve),
    });

    enum class image_layout
    {
        undefined,
        general,
        color_attachment_optimal,
        depth_stencil_attachment_optimal,
        depth_stencil_read_only_optimal,
        shader_read_only_optimal,
        transfer_src_optimal,
        transfer_dst_optimal,
        preinitialized,
        depth_read_only_stencil_attachment_optimal = 1000117000,
        depth_attachment_stencil_read_only_optimal,
        depth_attachment_optimal = 1000241000,
        depth_read_only_optimal,
        stencil_attachment_optimal,
        stencil_read_only_optimal,
        read_only_optimal = 1000314000,
        attachment_optimal,
        present_src_khr      = 1000001002,
        video_decode_dst_khr = 1000024000,
        video_decode_src_khr,
        video_decode_dpb_khr,
        shared_present_khr                             = 1000111000,
        fragment_density_map_optimal_ext               = 1000218000,
        fragment_shading_rate_attachment_optimal_khr   = 1000164003,
        attachment_feedback_loop_optimal_ext           = 1000339000,
        depth_read_only_stencil_attachment_optimal_khr = 1000117000,
        depth_attachment_stencil_read_only_optimal_khr,
        shading_rate_optimal_nv      = 1000164003,
        depth_attachment_optimal_khr = 1000241000,
        depth_read_only_optimal_khr,
        stencil_attachment_optimal_khr,
        stencil_read_only_optimal_khr,
        read_only_optimal_khr = 1000314000,
        attachment_optimal_khr,
        video_encode_dst_khr = 1000299000,
        video_encode_src_khr,
        video_encode_dpb_khr,
    };

    inline constexpr auto image_layout_names = create_name_map<image_layout>({
        NAME_ENTRY(image_layout::undefined),
        NAME_ENTRY(image_layout::general),
        NAME_ENTRY(image_layout::color_attachment_optimal),
        NAME_ENTRY(image_layout::depth_stencil_attachment_optimal),
        NAME_ENTRY(image_layout::depth_stencil_read_only_optimal),
        NAME_ENTRY(image_layout::shader_read_only_optimal),
        NAME_ENTRY(image_layout::transfer_src_optimal),
        NAME_ENTRY(image_layout::transfer_dst_optimal),
        NAME_ENTRY(image_layout::preinitialized),
        NAME_ENTRY(image_layout::depth_read_only_stencil_attachment_optimal),
        NAME_ENTRY(image_layout::depth_attachment_stencil_read_only_optimal),
        NAME_ENTRY(image_layout::depth_attachment_optimal),
        NAME_ENTRY(image_layout::depth_read_only_optimal),
        NAME_ENTRY(image_layout::stencil_attachment_optimal),
        NAME_ENTRY(image_layout::stencil_read_only_optimal),
        NAME_ENTRY(image_layout::read_only_optimal),
        NAME_ENTRY(image_layout::attachment_optimal),
        NAME_ENTRY(image_layout::present_src_khr),
        NAME_ENTRY(image_layout::video_decode_dst_khr),
        NAME_ENTRY(image_layout::video_decode_src_khr),
        NAME_ENTRY(image_layout::video_decode_dpb_khr),
        NAME_ENTRY(image_layout::shared_present_khr),
        NAME_ENTRY(image_layout::fragment_density_map_optimal_ext),
        NAME_ENTRY(image_layout::fragment_shading_rate_attachment_optimal_khr),
        NAME_ENTRY(image_layout::attachment_feedback_loop_optimal_ext),
        NAME_ENTRY(image_layout::depth_read_only_stencil_attachment_optimal_khr),
        NAME_ENTRY(image_layout::depth_attachment_stencil_read_only_optimal_khr),
        NAME_ENTRY(image_layout::shading_rate_optimal_nv),
        NAME_ENTRY(image_layout::depth_attachment_optimal_khr),
        NAME_ENTRY(image_layout::depth_read_only_optimal_khr),
        NAME_ENTRY(image_layout::stencil_attachment_optimal_khr),
        NAME_ENTRY(image_layout::stencil_read_only_optimal_khr),
        NAME_ENTRY(image_layout::read_only_optimal_khr),
        NAME_ENTRY(image_layout::attachment_optimal_khr),
        NAME_ENTRY(image_layout::video_encode_dst_khr),
        NAME_ENTRY(image_layout::video_encode_src_khr),
        NAME_ENTRY(image_layout::video_encode_dpb_khr),
    });

    enum class command_pool_create_flag : ui32
    {
        transient            = 1 << 0,
        reset_command_buffer = 1 << 1,
        prot                 = 1 << 2,
    };

    inline constexpr auto command_pool_create_flag_names = create_name_map<command_pool_create_flag>({
        NAME_ENTRY(command_pool_create_flag::transient),
        NAME_ENTRY(command_pool_create_flag::reset_command_buffer),
        NAME_ENTRY(command_pool_create_flag::prot),
    });

    enum class memory_property_flag : ui32
    {
        device_local        = 1 << 0,
        host_visible        = 1 << 1,
        host_coherent       = 1 << 2,
        host_cached         = 1 << 3,
        lazily_allocated    = 1 << 4,
        prot                = 1 << 5,
        device_coherent_amd = 1 << 6,
        device_uncached_amd = 1 << 7,
        rdma_capable_nv     = 1 << 8,
    };

    inline constexpr auto memory_property_flag_names = create_name_map<memory_property_flag>({
        NAME_ENTRY(memory_property_flag::device_local),
        NAME_ENTRY(memory_property_flag::host_visible),
        NAME_ENTRY(memory_property_flag::host_coherent),
        NAME_ENTRY(memory_property_flag::host_cached),
        NAME_ENTRY(memory_property_flag::lazily_allocated),
        NAME_ENTRY(memory_property_flag::prot),
        NAME_ENTRY(memory_property_flag::device_coherent_amd),
        NAME_ENTRY(memory_property_flag::device_uncached_amd),
        NAME_ENTRY(memory_property_flag::rdma_capable_nv),
    });

    enum class memory_usage : ui32
    {
        /** No intended memory usage specified.
        Use other members of VmaAllocationCreateInfo to specify your requirements.
        */
        unknown,
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Prefers `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
        */
        gpu_only,
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT` and `VK_MEMORY_PROPERTY_HOST_COHERENT_BIT`.
        */
        cpu_only,
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT`, prefers `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
        */
        cpu_to_gpu,
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Guarantees `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT`, prefers `VK_MEMORY_PROPERTY_HOST_CACHED_BIT`.
        */
        gpu_to_cpu,
        /**
        \deprecated Obsolete, preserved for backward compatibility.
        Prefers not `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`.
        */
        cpu_copy,
        /**
        Lazily allocated GPU memory having `VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT`.
        Exists mostly on mobile platforms. Using it on desktop PC or other GPUs with no such memory type present will fail the allocation.

        Usage: Memory for transient attachment images (color attachments, depth attachments etc.), created with `VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT`.

        Allocations with this usage are always created as dedicated - it implies #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT.
        */
        gpu_lazily_allocated,
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
        usage_auto,
        /**
        Selects best memory type automatically with preference for GPU (device) memory.

        When using this flag, if you want to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT),
        you must pass one of the flags: #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
        in VmaAllocationCreateInfo::flags.

        It can be used only with functions that let the library know `VkBufferCreateInfo` or `VkImageCreateInfo`, e.g.
        vmaCreateBuffer(), vmaCreateImage(), vmaFindMemoryTypeIndexForBufferInfo(), vmaFindMemoryTypeIndexForImageInfo()
        and not with generic memory allocation functions.
        */
        auto_prefer_device,
        /**
        Selects best memory type automatically with preference for CPU (host) memory.

        When using this flag, if you want to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT),
        you must pass one of the flags: #VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT or #VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
        in VmaAllocationCreateInfo::flags.

        It can be used only with functions that let the library know `VkBufferCreateInfo` or `VkImageCreateInfo`, e.g.
        vmaCreateBuffer(), vmaCreateImage(), vmaFindMemoryTypeIndexForBufferInfo(), vmaFindMemoryTypeIndexForImageInfo()
        and not with generic memory allocation functions.
        */
        auto_prefer_host,
    };

    inline constexpr auto memory_usage_names = create_name_map<memory_usage>({
        NAME_ENTRY(memory_usage::unknown),
        NAME_ENTRY(memory_usage::gpu_only),
        NAME_ENTRY(memory_usage::cpu_only),
        NAME_ENTRY(memory_usage::cpu_to_gpu),
        NAME_ENTRY(memory_usage::gpu_to_cpu),
        NAME_ENTRY(memory_usage::cpu_copy),
        NAME_ENTRY(memory_usage::gpu_lazily_allocated),
        NAME_ENTRY(memory_usage::usage_auto),
        NAME_ENTRY(memory_usage::auto_prefer_device),
        NAME_ENTRY(memory_usage::auto_prefer_host),
    });

    enum class memory_flag : ui32
    {
        /** \brief Set this flag if the allocation should have its own memory block.

        Use it for special, big resources, like fullscreen images used as attachments.

        If you use this flag while creating a buffer or an image, `VkMemoryDedicatedAllocateInfo`
        structure is applied if possible.
        */
        dedicated_memory = 1 << 0,

        /** \brief Set this flag to only try to allocate from existing `VkDeviceMemory` blocks and never create new such block.

        If new allocation cannot be placed in any of the existing blocks, allocation
        fails with `VK_ERROR_OUT_OF_DEVICE_MEMORY` error.

        You should not use #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT and
        #VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT at the same time. It makes no sense.
        */
        never_allocate = 1 << 1,

        /** \brief Set this flag to use a memory that will be persistently mapped and retrieve pointer to it.

        Pointer to mapped memory will be returned through VmaAllocationInfo::pMappedData.

        It is valid to use this flag for allocation made from memory type that is not
        `HOST_VISIBLE`. This flag is then ignored and memory is not mapped. This is
        useful if you need an allocation that is efficient to use on GPU
        (`DEVICE_LOCAL`) and still want to map it directly if possible on platforms that
        support it (e.g. Intel GPU).
        */
        mapped = 1 << 2,

        /** \deprecated Preserved for backward compatibility. Consider using vmaSetAllocationName() instead.

        Set this flag to treat VmaAllocationCreateInfo::pUserData as pointer to a
        null-terminated string. Instead of copying pointer value, a local copy of the
        string is made and stored in allocation's `pName`. The string is automatically
        freed together with the allocation. It is also used in vmaBuildStatsString().
        */
        user_data_copy_string = 1 << 5,

        /** Allocation will be created from upper stack in a double stack pool.

        This flag is only allowed for custom pools created with #VMA_POOL_CREATE_LINEAR_ALGORITHM_BIT flag.
        */
        upper_address = 1 << 6,

        /** Create both buffer/image and allocation, but don't bind them together.
        It is useful when you want to bind yourself to do some more advanced binding, e.g. using some extensions.
        The flag is meaningful only with functions that bind by default: vmaCreateBuffer(), vmaCreateImage().
        Otherwise it is ignored.

        If you want to make sure the new buffer/image is not tied to the new memory allocation
        through `VkMemoryDedicatedAllocateInfoKHR` structure in case the allocation ends up in its own memory block,
        use also flag #VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT.
        */
        dont_bind = 1 << 7,

        /** Create allocation only if additional device memory required for it, if any, won't exceed
        memory budget. Otherwise return `VK_ERROR_OUT_OF_DEVICE_MEMORY`.
        */
        within_budget = 1 << 8,

        /** \brief Set this flag if the allocated memory will have aliasing resources.

        Usage of this flag prevents supplying `VkMemoryDedicatedAllocateInfoKHR` when #VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT is specified.
        Otherwise created dedicated memory will not be suitable for aliasing resources, resulting in Vulkan Validation Layer errors.
        */
        can_alias = 1 << 9,

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
        host_access_sequential_write = 1 << 10,

        /**
        Requests possibility to map the allocation (using vmaMapMemory() or #VMA_ALLOCATION_CREATE_MAPPED_BIT).

        - If you use #VMA_MEMORY_USAGE_AUTO or other `VMA_MEMORY_USAGE_AUTO*` value,
          you must use this flag to be able to map the allocation. Otherwise, mapping is incorrect.
        - If you use other value of #VmaMemoryUsage, this flag is ignored and mapping is always possible in memory types that are `HOST_VISIBLE`.
          This includes allocations created in \ref custom_memory_pools.

        Declares that mapped memory can be read, written, and accessed in random order,
        so a `HOST_CACHED` memory type is preferred.
        */
        host_access_random = 1 << 11,

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
        host_access_allow_transfer_instead = 1 << 12,

        /** Allocation strategy that chooses smallest possible free range for the allocation
        to minimize memory usage and fragmentation, possibly at the expense of allocation time.
        */
        strategy_min_memory = 1 << 16,

        /** Allocation strategy that chooses first suitable free range for the allocation -
        not necessarily in terms of the smallest offset but the one that is easiest and fastest to find
        to minimize allocation time, possibly at the expense of allocation quality.
        */
        strategy_min_time = 1 << 17,

        /** Allocation strategy that chooses always the lowest offset in available space.
        This is not the most efficient strategy but achieves highly packed data.
        Used internally by defragmentation, not recommended in typical usage.
        */
        strategy_min_offset = 1 << 18,

        /** Alias to #VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT.
         */
        strategy_best_fit = 1 << 16,

        /** Alias to #VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT.
         */
        strategy_first_fit = 1 << 17,

        /** A bit mask to extract only `STRATEGY` bits from entire set of flags.
         */
        strategy_mask = strategy_min_memory | strategy_min_time | strategy_min_offset,
    };

    inline constexpr auto memory_flag_names = create_name_map<memory_flag>({
        NAME_ENTRY(memory_flag::dedicated_memory),
        NAME_ENTRY(memory_flag::never_allocate),
        NAME_ENTRY(memory_flag::mapped),
        NAME_ENTRY(memory_flag::user_data_copy_string),
        NAME_ENTRY(memory_flag::upper_address),
        NAME_ENTRY(memory_flag::dont_bind),
        NAME_ENTRY(memory_flag::within_budget),
        NAME_ENTRY(memory_flag::can_alias),
        NAME_ENTRY(memory_flag::host_access_sequential_write),
        NAME_ENTRY(memory_flag::host_access_random),
        NAME_ENTRY(memory_flag::host_access_allow_transfer_instead),
        NAME_ENTRY(memory_flag::strategy_min_memory),
        NAME_ENTRY(memory_flag::strategy_min_time),
        NAME_ENTRY(memory_flag::strategy_min_offset),
        NAME_ENTRY(memory_flag::strategy_best_fit),
        NAME_ENTRY(memory_flag::strategy_first_fit),
        NAME_ENTRY(memory_flag::strategy_mask),
    });

    enum class filter : ui32
    {
        nearest,
        linear,
        cubic_ext = 1000015000,
        cubic_img = cubic_ext,
    };

    inline constexpr auto filter_names = create_name_map<filter>({
        NAME_ENTRY(filter::nearest),
        NAME_ENTRY(filter::linear),
        NAME_ENTRY(filter::cubic_ext),
        NAME_ENTRY(filter::cubic_img),
    });

    enum class debug_utils_message_severity_flag : ui32
    {
        verbose = 1 << 0,
        info    = 1 << 4,
        warning = 1 << 8,
        error   = 1 << 12,
    };

    inline constexpr auto debug_utils_message_severity_flag_names = create_name_map<debug_utils_message_severity_flag>({
        NAME_ENTRY(debug_utils_message_severity_flag::verbose),
        NAME_ENTRY(debug_utils_message_severity_flag::info),
        NAME_ENTRY(debug_utils_message_severity_flag::warning),
        NAME_ENTRY(debug_utils_message_severity_flag::error),
    });

    enum class debug_utils_message_type_flag : ui32
    {
        general                = 1 << 0,
        validation             = 1 << 1,
        performance            = 1 << 2,
        device_address_binding = 1 << 3,
    };

    inline constexpr auto debug_utils_message_type_flag_names = create_name_map<debug_utils_message_type_flag>({
        NAME_ENTRY(debug_utils_message_type_flag::general),
        NAME_ENTRY(debug_utils_message_type_flag::validation),
        NAME_ENTRY(debug_utils_message_type_flag::performance),
        NAME_ENTRY(debug_utils_message_type_flag::device_address_binding),
    });

    enum class command_buffer_usage_flag : ui32
    {
        one_time_submit      = 1 << 0,
        render_pass_continue = 1 << 1,
        simultaneous_use     = 1 << 2,
    };

    inline constexpr auto command_buffer_usage_flag_names = create_name_map<command_buffer_usage_flag>({
        NAME_ENTRY(command_buffer_usage_flag::one_time_submit),
        NAME_ENTRY(command_buffer_usage_flag::render_pass_continue),
        NAME_ENTRY(command_buffer_usage_flag::simultaneous_use),
    });

    inline constexpr ui32 subpass_external = UINT32_MAX;

    enum class subpass_description_flag
    {
        per_view_attributes                           = 1 << 0,
        per_view_position_x_only                      = 1 << 1,
        fragment_region                               = 1 << 2,
        shader_resolve                                = 1 << 3,
        rasterization_order_attachment_color_access   = 1 << 4,
        rasterization_order_attachment_depth_access   = 1 << 5,
        rasterization_order_attachment_stencil_access = 1 << 6,
        enable_legacy_dithering                       = 1 << 7,
    };

    inline constexpr auto subpass_description_flag_names = create_name_map<subpass_description_flag>({
        NAME_ENTRY(subpass_description_flag::per_view_attributes),
        NAME_ENTRY(subpass_description_flag::per_view_position_x_only),
        NAME_ENTRY(subpass_description_flag::fragment_region),
        NAME_ENTRY(subpass_description_flag::shader_resolve),
        NAME_ENTRY(subpass_description_flag::rasterization_order_attachment_color_access),
        NAME_ENTRY(subpass_description_flag::rasterization_order_attachment_depth_access),
        NAME_ENTRY(subpass_description_flag::rasterization_order_attachment_stencil_access),
        NAME_ENTRY(subpass_description_flag::enable_legacy_dithering),
    });

    enum class shader_stage_flag : ui32
    {
        vertex                 = 1 << 0,
        tesselation_control    = 1 << 1,
        tesselation_evaluation = 1 << 2,
        geometry               = 1 << 3,
        fragment               = 1 << 4,
        compute                = 1 << 5,
        all_graphics           = vertex | tesselation_control | tesselation_evaluation | geometry | fragment,
        all                    = 0x7FFFFFFF,
        raygen                 = 1 << 8,
        any_hit                = 1 << 9,
        closest_hit            = 1 << 10,
        miss                   = 1 << 11,
        intersection           = 1 << 12,
        callable               = 1 << 13,
        task                   = 1 << 6,
        mesh                   = 1 << 7,
        subpass_shading_huawei = 1 << 14,
        cluster_culling_huawei = 1 << 19,
        raygen_nv              = raygen,
        any_hit_nv             = any_hit,
        closest_hit_nv         = closest_hit,
        miss_nv                = miss,
        intersection_nv        = intersection,
        callable_nv            = callable,
        task_nv                = task,
        mesh_nv                = mesh,
    };

    inline constexpr auto shader_stage_flag_names = create_name_map<shader_stage_flag>({
        NAME_ENTRY(shader_stage_flag::vertex),
        NAME_ENTRY(shader_stage_flag::tesselation_control),
        NAME_ENTRY(shader_stage_flag::tesselation_evaluation),
        NAME_ENTRY(shader_stage_flag::geometry),
        NAME_ENTRY(shader_stage_flag::fragment),
        NAME_ENTRY(shader_stage_flag::compute),
        NAME_ENTRY(shader_stage_flag::all_graphics),
        NAME_ENTRY(shader_stage_flag::all),
        NAME_ENTRY(shader_stage_flag::raygen),
        NAME_ENTRY(shader_stage_flag::any_hit),
        NAME_ENTRY(shader_stage_flag::closest_hit),
        NAME_ENTRY(shader_stage_flag::miss),
        NAME_ENTRY(shader_stage_flag::intersection),
        NAME_ENTRY(shader_stage_flag::callable),
        NAME_ENTRY(shader_stage_flag::task),
        NAME_ENTRY(shader_stage_flag::mesh),
        NAME_ENTRY(shader_stage_flag::subpass_shading_huawei),
        NAME_ENTRY(shader_stage_flag::cluster_culling_huawei),
        NAME_ENTRY(shader_stage_flag::raygen_nv),
        NAME_ENTRY(shader_stage_flag::any_hit_nv),
        NAME_ENTRY(shader_stage_flag::closest_hit_nv),
        NAME_ENTRY(shader_stage_flag::miss_nv),
        NAME_ENTRY(shader_stage_flag::intersection_nv),
        NAME_ENTRY(shader_stage_flag::callable_nv),
        NAME_ENTRY(shader_stage_flag::task_nv),
        NAME_ENTRY(shader_stage_flag::mesh_nv),
    });

    enum class dynamic_state : ui32
    {
        viewport,
        scissor,
        line_width,
        depth_bias,
        blend_constants,
        depth_bounds,
        stencil_compare_mask,
        stencil_write_mask,
        stencil_reference,
        cull_mode = 1000267000,
        front_face,
        primitive_topology,
        viewport_with_count,
        scissor_with_count,
        vertex_input_binding_stride,
        depth_test_enable,
        depth_write_enable,
        depth_compare_op,
        depth_bounds_test_enable,
        stencil_test_enable,
        stencil_op,
        rasterizer_discard_enable = 1000377001,
        depth_bias_enable,
        primitive_restart_enable            = 1000377004,
        viewport_w_scaling_nv               = 1000087000,
        discard_rectangle_ext               = 1000099000,
        sample_locations_ext                = 1000143000,
        ray_tracing_pipeline_stack_size_khr = 1000347000,
        viewport_shading_rate_palette_nv    = 1000164004,
        viewport_coarse_sample_order_nv     = 1000164006,
        exclusive_scissor_nv                = 1000205001,
        fragment_shading_rate_khr           = 1000226000,
        line_stipple_ext                    = 1000259000,
        vertex_input_ext                    = 1000352000,
        patch_control_points_ext            = 1000377000,
        logic_op_ext                        = 1000377003,
        color_write_enable_ext              = 1000381000,
        tessellation_domain_origin_ext      = 1000455002,
        depth_clamp_enable_ext,
        polygon_mode_ext,
        rasterization_samples_ext,
        sample_mask_ext,
        alpha_to_coverage_enable_ext,
        alpha_to_one_enable_ext,
        logic_op_enable_ext,
        color_blend_enable_ext,
        color_blend_equation_ext,
        color_write_mask_ext,
        rasterization_stream_ext,
        conservative_rasterization_mode_ext,
        extra_primitive_overestimation_size_ext,
        depth_clip_enable_ext,
        sample_locations_enable_ext,
        color_blend_advanced_ext,
        provoking_vertex_mode_ext,
        line_rasterization_mode_ext,
        line_stipple_enable_ext,
        depth_clip_negative_one_to_one_ext,
        viewport_w_scaling_enable_nv,
        viewport_swizzle_nv,
        coverage_to_color_enable_nv,
        coverage_to_color_location_nv,
        coverage_modulation_mode_nv,
        coverage_modulation_table_enable_nv,
        coverage_modulation_table_nv,
        shading_rate_image_enable_nv,
        representative_fragment_test_enable_nv,
        coverage_reduction_mode_nv,
        cull_mode_ext                   = cull_mode,
        front_face_ext                  = front_face,
        primitive_topology_ext          = primitive_topology,
        viewport_with_count_ext         = viewport_with_count,
        scissor_with_count_ext          = scissor_with_count,
        vertex_input_binding_stride_ext = vertex_input_binding_stride,
        depth_test_enable_ext           = depth_test_enable,
        depth_write_enable_ext          = depth_write_enable,
        depth_compare_op_ext            = depth_compare_op,
        depth_bounds_test_enable_ext    = depth_bounds_test_enable,
        stencil_test_enable_ext         = stencil_test_enable,
        stencil_op_ext                  = stencil_op,
        rasterizer_discard_enable_ext   = rasterizer_discard_enable,
        depth_bias_enable_ext           = depth_bias_enable,
        primitive_restart_enable_ext    = primitive_restart_enable,
    };

    inline constexpr auto dynamic_state_names = create_name_map<dynamic_state>({
        NAME_ENTRY(dynamic_state::viewport),
        NAME_ENTRY(dynamic_state::scissor),
        NAME_ENTRY(dynamic_state::line_width),
        NAME_ENTRY(dynamic_state::depth_bias),
        NAME_ENTRY(dynamic_state::blend_constants),
        NAME_ENTRY(dynamic_state::depth_bounds),
        NAME_ENTRY(dynamic_state::stencil_compare_mask),
        NAME_ENTRY(dynamic_state::stencil_write_mask),
        NAME_ENTRY(dynamic_state::stencil_reference),
        NAME_ENTRY(dynamic_state::cull_mode),
        NAME_ENTRY(dynamic_state::front_face),
        NAME_ENTRY(dynamic_state::primitive_topology),
        NAME_ENTRY(dynamic_state::viewport_with_count),
        NAME_ENTRY(dynamic_state::scissor_with_count),
        NAME_ENTRY(dynamic_state::vertex_input_binding_stride),
        NAME_ENTRY(dynamic_state::depth_test_enable),
        NAME_ENTRY(dynamic_state::depth_write_enable),
        NAME_ENTRY(dynamic_state::depth_compare_op),
        NAME_ENTRY(dynamic_state::depth_bounds_test_enable),
        NAME_ENTRY(dynamic_state::stencil_test_enable),
        NAME_ENTRY(dynamic_state::stencil_op),
        NAME_ENTRY(dynamic_state::rasterizer_discard_enable),
        NAME_ENTRY(dynamic_state::depth_bias_enable),
        NAME_ENTRY(dynamic_state::primitive_restart_enable),
        NAME_ENTRY(dynamic_state::viewport_w_scaling_nv),
        NAME_ENTRY(dynamic_state::discard_rectangle_ext),
        NAME_ENTRY(dynamic_state::sample_locations_ext),
        NAME_ENTRY(dynamic_state::ray_tracing_pipeline_stack_size_khr),
        NAME_ENTRY(dynamic_state::viewport_shading_rate_palette_nv),
        NAME_ENTRY(dynamic_state::viewport_coarse_sample_order_nv),
        NAME_ENTRY(dynamic_state::exclusive_scissor_nv),
        NAME_ENTRY(dynamic_state::fragment_shading_rate_khr),
        NAME_ENTRY(dynamic_state::line_stipple_ext),
        NAME_ENTRY(dynamic_state::vertex_input_ext),
        NAME_ENTRY(dynamic_state::patch_control_points_ext),
        NAME_ENTRY(dynamic_state::logic_op_ext),
        NAME_ENTRY(dynamic_state::color_write_enable_ext),
        NAME_ENTRY(dynamic_state::tessellation_domain_origin_ext),
        NAME_ENTRY(dynamic_state::depth_clamp_enable_ext),
        NAME_ENTRY(dynamic_state::polygon_mode_ext),
        NAME_ENTRY(dynamic_state::rasterization_samples_ext),
        NAME_ENTRY(dynamic_state::sample_mask_ext),
        NAME_ENTRY(dynamic_state::alpha_to_coverage_enable_ext),
        NAME_ENTRY(dynamic_state::alpha_to_one_enable_ext),
        NAME_ENTRY(dynamic_state::logic_op_enable_ext),
        NAME_ENTRY(dynamic_state::color_blend_enable_ext),
        NAME_ENTRY(dynamic_state::color_blend_equation_ext),
        NAME_ENTRY(dynamic_state::color_write_mask_ext),
        NAME_ENTRY(dynamic_state::rasterization_stream_ext),
        NAME_ENTRY(dynamic_state::conservative_rasterization_mode_ext),
        NAME_ENTRY(dynamic_state::extra_primitive_overestimation_size_ext),
        NAME_ENTRY(dynamic_state::depth_clip_enable_ext),
        NAME_ENTRY(dynamic_state::sample_locations_enable_ext),
        NAME_ENTRY(dynamic_state::color_blend_advanced_ext),
        NAME_ENTRY(dynamic_state::provoking_vertex_mode_ext),
        NAME_ENTRY(dynamic_state::line_rasterization_mode_ext),
        NAME_ENTRY(dynamic_state::line_stipple_enable_ext),
        NAME_ENTRY(dynamic_state::depth_clip_negative_one_to_one_ext),
        NAME_ENTRY(dynamic_state::viewport_w_scaling_enable_nv),
        NAME_ENTRY(dynamic_state::viewport_swizzle_nv),
        NAME_ENTRY(dynamic_state::coverage_to_color_enable_nv),
        NAME_ENTRY(dynamic_state::coverage_to_color_location_nv),
        NAME_ENTRY(dynamic_state::coverage_modulation_mode_nv),
        NAME_ENTRY(dynamic_state::coverage_modulation_table_enable_nv),
        NAME_ENTRY(dynamic_state::coverage_modulation_table_nv),
        NAME_ENTRY(dynamic_state::shading_rate_image_enable_nv),
        NAME_ENTRY(dynamic_state::representative_fragment_test_enable_nv),
        NAME_ENTRY(dynamic_state::coverage_reduction_mode_nv),
        NAME_ENTRY(dynamic_state::cull_mode_ext),
        NAME_ENTRY(dynamic_state::front_face_ext),
        NAME_ENTRY(dynamic_state::primitive_topology_ext),
        NAME_ENTRY(dynamic_state::viewport_with_count_ext),
        NAME_ENTRY(dynamic_state::scissor_with_count_ext),
        NAME_ENTRY(dynamic_state::vertex_input_binding_stride_ext),
        NAME_ENTRY(dynamic_state::depth_test_enable_ext),
        NAME_ENTRY(dynamic_state::depth_write_enable_ext),
        NAME_ENTRY(dynamic_state::depth_compare_op_ext),
        NAME_ENTRY(dynamic_state::depth_bounds_test_enable_ext),
        NAME_ENTRY(dynamic_state::stencil_test_enable_ext),
        NAME_ENTRY(dynamic_state::stencil_op_ext),
        NAME_ENTRY(dynamic_state::rasterizer_discard_enable_ext),
        NAME_ENTRY(dynamic_state::depth_bias_enable_ext),
        NAME_ENTRY(dynamic_state::primitive_restart_enable_ext),
    });

    enum class primitive_topology : ui32
    {
        point_list,
        line_list,
        line_strip,
        triangle_list,
        triangle_strip,
        triangle_fan,
        line_list_with_adjacency,
        line_strip_with_adjacency,
        triangle_list_with_adjacency,
        triangle_strip_with_adjacency,
        patch_list,
    };

    inline constexpr auto priitive_topology_names = create_name_map<primitive_topology>({
        NAME_ENTRY(primitive_topology::point_list),
        NAME_ENTRY(primitive_topology::line_list),
        NAME_ENTRY(primitive_topology::line_strip),
        NAME_ENTRY(primitive_topology::triangle_list),
        NAME_ENTRY(primitive_topology::triangle_strip),
        NAME_ENTRY(primitive_topology::triangle_fan),
        NAME_ENTRY(primitive_topology::line_list_with_adjacency),
        NAME_ENTRY(primitive_topology::line_strip_with_adjacency),
        NAME_ENTRY(primitive_topology::triangle_list_with_adjacency),
        NAME_ENTRY(primitive_topology::triangle_strip_with_adjacency),
        NAME_ENTRY(primitive_topology::patch_list),
    });

    enum class polygon_mode : ui32
    {
        fill,
        line,
        point,
        fill_rectangle_nv = 1000153000,
    };

    inline constexpr auto polygon_mode_names = create_name_map<polygon_mode>({
        NAME_ENTRY(polygon_mode::fill),
        NAME_ENTRY(polygon_mode::line),
        NAME_ENTRY(polygon_mode::point),
        NAME_ENTRY(polygon_mode::fill_rectangle_nv),
    });

    enum class cull_mode : ui32
    {
        none,
        front,
        back,
        front_and_back,
    };

    inline constexpr auto cull_mode_names = create_name_map<cull_mode>({
        NAME_ENTRY(cull_mode::none),
        NAME_ENTRY(cull_mode::front),
        NAME_ENTRY(cull_mode::back),
        NAME_ENTRY(cull_mode::front_and_back),
    });

    enum class front_face : ui32
    {
        counter_clockwise,
        clockwise,
    };

    inline constexpr auto front_face_names = create_name_map<front_face>({
        NAME_ENTRY(front_face::counter_clockwise),
        NAME_ENTRY(front_face::clockwise),
    });

    enum class color_component : ui32
    {
        r = 1 << 0,
        g = 1 << 1,
        b = 1 << 2,
        a = 1 << 3,
    };

    inline constexpr auto color_component_names = create_name_map<color_component>({
        NAME_ENTRY(color_component::r),
        NAME_ENTRY(color_component::g),
        NAME_ENTRY(color_component::b),
        NAME_ENTRY(color_component::a),
    });

    enum class blend_factor : ui32
    {
        zero,
        one,
        src_color,
        one_minus_src_color,
        dst_color,
        one_minus_dst_color,
        src_alpha,
        one_minus_src_alpha,
        dst_alpha,
        one_minus_dst_alpha,
        constant_color,
        one_minus_constant_color,
        constant_alpha,
        one_minus_constant_alpha,
        src_alpha_saturate,
        src1_color,
        one_minus_src1_color,
        src1_alpha,
        one_minus_src1_alpha,
    };

    inline constexpr auto blend_factor_names = create_name_map<blend_factor>({
        NAME_ENTRY(blend_factor::zero),
        NAME_ENTRY(blend_factor::one),
        NAME_ENTRY(blend_factor::src_color),
        NAME_ENTRY(blend_factor::one_minus_src_color),
        NAME_ENTRY(blend_factor::dst_color),
        NAME_ENTRY(blend_factor::one_minus_dst_color),
        NAME_ENTRY(blend_factor::src_alpha),
        NAME_ENTRY(blend_factor::one_minus_src_alpha),
        NAME_ENTRY(blend_factor::dst_alpha),
        NAME_ENTRY(blend_factor::one_minus_dst_alpha),
        NAME_ENTRY(blend_factor::constant_color),
        NAME_ENTRY(blend_factor::one_minus_constant_color),
        NAME_ENTRY(blend_factor::constant_alpha),
        NAME_ENTRY(blend_factor::one_minus_constant_alpha),
        NAME_ENTRY(blend_factor::src_alpha_saturate),
        NAME_ENTRY(blend_factor::src1_color),
        NAME_ENTRY(blend_factor::one_minus_src1_color),
        NAME_ENTRY(blend_factor::src1_alpha),
        NAME_ENTRY(blend_factor::one_minus_src1_alpha),
    });

    enum class blend_op : ui32
    {
        add,
        subtract,
        reverse_subtract,
        min,
        max,
        zero_ext = 1000148000,
        src_ext,
        dst_ext,
        src_over_ext,
        dst_over_ext,
        src_in_ext,
        dst_in_ext,
        src_out_ext,
        dst_out_ext,
        src_atop_ext,
        dst_atop_ext,
        xor_ext,
        multiply_ext,
        screen_ext,
        overlay_ext,
        darken_ext,
        lighten_ext,
        colordodge_ext,
        colorburn_ext,
        hardlight_ext,
        softlight_ext,
        difference_ext,
        exclusion_ext,
        invert_ext,
        invert_rgb_ext,
        lineardodge_ext,
        linearburn_ext,
        vividlight_ext,
        linearlight_ext,
        pinlight_ext,
        hardmix_ext,
        hsl_hue_ext,
        hsl_saturation_ext,
        hsl_color_ext,
        hsl_luminosity_ext,
        plus_ext,
        plus_clamped_ext,
        plus_clamped_alpha_ext,
        plus_darker_ext,
        minus_ext,
        minus_clamped_ext,
        contrast_ext,
        invert_ovg_ext,
        red_ext,
        green_ext,
        blue_ext,
    };

    inline constexpr auto blend_op_names = create_name_map<blend_op>({
        NAME_ENTRY(blend_op::add),
        NAME_ENTRY(blend_op::subtract),
        NAME_ENTRY(blend_op::reverse_subtract),
        NAME_ENTRY(blend_op::min),
        NAME_ENTRY(blend_op::max),
        NAME_ENTRY(blend_op::zero_ext),
        NAME_ENTRY(blend_op::src_ext),
        NAME_ENTRY(blend_op::dst_ext),
        NAME_ENTRY(blend_op::src_over_ext),
        NAME_ENTRY(blend_op::dst_over_ext),
        NAME_ENTRY(blend_op::src_in_ext),
        NAME_ENTRY(blend_op::dst_in_ext),
        NAME_ENTRY(blend_op::src_out_ext),
        NAME_ENTRY(blend_op::dst_out_ext),
        NAME_ENTRY(blend_op::src_atop_ext),
        NAME_ENTRY(blend_op::dst_atop_ext),
        NAME_ENTRY(blend_op::xor_ext),
        NAME_ENTRY(blend_op::multiply_ext),
        NAME_ENTRY(blend_op::screen_ext),
        NAME_ENTRY(blend_op::overlay_ext),
        NAME_ENTRY(blend_op::darken_ext),
        NAME_ENTRY(blend_op::lighten_ext),
        NAME_ENTRY(blend_op::colordodge_ext),
        NAME_ENTRY(blend_op::colorburn_ext),
        NAME_ENTRY(blend_op::hardlight_ext),
        NAME_ENTRY(blend_op::softlight_ext),
        NAME_ENTRY(blend_op::difference_ext),
        NAME_ENTRY(blend_op::exclusion_ext),
        NAME_ENTRY(blend_op::invert_ext),
        NAME_ENTRY(blend_op::invert_rgb_ext),
        NAME_ENTRY(blend_op::lineardodge_ext),
        NAME_ENTRY(blend_op::linearburn_ext),
        NAME_ENTRY(blend_op::vividlight_ext),
        NAME_ENTRY(blend_op::linearlight_ext),
        NAME_ENTRY(blend_op::pinlight_ext),
        NAME_ENTRY(blend_op::hardmix_ext),
        NAME_ENTRY(blend_op::hsl_hue_ext),
        NAME_ENTRY(blend_op::hsl_saturation_ext),
        NAME_ENTRY(blend_op::hsl_color_ext),
        NAME_ENTRY(blend_op::hsl_luminosity_ext),
        NAME_ENTRY(blend_op::plus_ext),
        NAME_ENTRY(blend_op::plus_clamped_ext),
        NAME_ENTRY(blend_op::plus_clamped_alpha_ext),
        NAME_ENTRY(blend_op::plus_darker_ext),
        NAME_ENTRY(blend_op::minus_ext),
        NAME_ENTRY(blend_op::minus_clamped_ext),
        NAME_ENTRY(blend_op::contrast_ext),
        NAME_ENTRY(blend_op::invert_ovg_ext),
        NAME_ENTRY(blend_op::red_ext),
        NAME_ENTRY(blend_op::green_ext),
        NAME_ENTRY(blend_op::blue_ext),
    });

    enum class shader_kind : ui32
    {
        vertex,
        fragment,
        compute,
        geometry,
        tess_control,
        tess_evaluation,
        raygen = 14,
        anyhit,
        closesthit,
        miss,
        intersection,
        callable,
        task = 26,
        mesh,
        glsl_vertex = 0,
        glsl_fragment,
        glsl_compute,
        glsl_geometry,
        glsl_tess_control,
        glsl_tess_evaluation,
        glsl_raygen = 14,
        glsl_anyhit,
        glsl_closesthit,
        glsl_miss,
        glsl_intersection,
        glsl_callable,
        glsl_task = 26,
        glsl_mesh,
        glsl_infer = 6,
    };

    inline constexpr auto shader_kind_names = create_name_map<shader_kind>({
        NAME_ENTRY(shader_kind::vertex),
        NAME_ENTRY(shader_kind::fragment),
        NAME_ENTRY(shader_kind::compute),
        NAME_ENTRY(shader_kind::geometry),
        NAME_ENTRY(shader_kind::tess_control),
        NAME_ENTRY(shader_kind::tess_evaluation),
        NAME_ENTRY(shader_kind::raygen),
        NAME_ENTRY(shader_kind::anyhit),
        NAME_ENTRY(shader_kind::closesthit),
        NAME_ENTRY(shader_kind::miss),
        NAME_ENTRY(shader_kind::intersection),
        NAME_ENTRY(shader_kind::callable),
        NAME_ENTRY(shader_kind::task),
        NAME_ENTRY(shader_kind::mesh),
        NAME_ENTRY(shader_kind::glsl_vertex),
        NAME_ENTRY(shader_kind::glsl_fragment),
        NAME_ENTRY(shader_kind::glsl_compute),
        NAME_ENTRY(shader_kind::glsl_geometry),
        NAME_ENTRY(shader_kind::glsl_tess_control),
        NAME_ENTRY(shader_kind::glsl_tess_evaluation),
        NAME_ENTRY(shader_kind::glsl_raygen),
        NAME_ENTRY(shader_kind::glsl_anyhit),
        NAME_ENTRY(shader_kind::glsl_closesthit),
        NAME_ENTRY(shader_kind::glsl_miss),
        NAME_ENTRY(shader_kind::glsl_intersection),
        NAME_ENTRY(shader_kind::glsl_callable),
        NAME_ENTRY(shader_kind::glsl_task),
        NAME_ENTRY(shader_kind::glsl_mesh),
        NAME_ENTRY(shader_kind::glsl_infer),
    });

    enum class vertex_input_rate : ui32
    {
        vertex,
        instance,
    };

    inline constexpr auto vertex_input_rate_names = create_name_map<vertex_input_rate>({
        NAME_ENTRY(vertex_input_rate::vertex),
        NAME_ENTRY(vertex_input_rate::instance),
    });

    enum class vertex_format : ui32
    {
        float_t  = 100,
        vec2_t   = 103,
        vec3_t   = 106,
        vec4_t   = 109,
        int_t    = 99,
        ivec2_t  = 105,
        ivec3_t  = 105,
        ivec4_t  = 108,
        uint_t   = 98,
        uvec2_t  = 101,
        uvec3_t  = 104,
        uvec4_t  = 107,
        short_t  = 75,
        svec2_t  = 82,
        svec3_t  = 89,
        svec4_t  = 96,
        ushort_t = 74,
        usvec2_t = 81,
        usvec3_t = 88,
        usvec4_t = 95,
        byte_t   = 14,
        bvec2_t  = 21,
        bvec3_t  = 28,
        bvec4_t  = 42,
        ubyte_t  = 13,
        ubvec2_t = 20,
        ubvec3_t = 27,
        ubvec4_t = 41,
        double_t = 112,
        dvec2_t  = 115,
        dvec3_t  = 118,
        dvec4_t  = 121,
    };

    inline constexpr auto vertex_format_names = create_name_map<vertex_format>({
        NAME_ENTRY(vertex_format::float_t),
        NAME_ENTRY(vertex_format::vec2_t),
        NAME_ENTRY(vertex_format::vec3_t),
        NAME_ENTRY(vertex_format::vec4_t),
        NAME_ENTRY(vertex_format::int_t),
        NAME_ENTRY(vertex_format::ivec2_t),
        NAME_ENTRY(vertex_format::ivec3_t),
        NAME_ENTRY(vertex_format::ivec4_t),
        NAME_ENTRY(vertex_format::uint_t),
        NAME_ENTRY(vertex_format::uvec2_t),
        NAME_ENTRY(vertex_format::uvec3_t),
        NAME_ENTRY(vertex_format::uvec4_t),
        NAME_ENTRY(vertex_format::short_t),
        NAME_ENTRY(vertex_format::svec2_t),
        NAME_ENTRY(vertex_format::svec3_t),
        NAME_ENTRY(vertex_format::svec4_t),
        NAME_ENTRY(vertex_format::ushort_t),
        NAME_ENTRY(vertex_format::usvec2_t),
        NAME_ENTRY(vertex_format::usvec3_t),
        NAME_ENTRY(vertex_format::usvec4_t),
        NAME_ENTRY(vertex_format::byte_t),
        NAME_ENTRY(vertex_format::bvec2_t),
        NAME_ENTRY(vertex_format::bvec3_t),
        NAME_ENTRY(vertex_format::bvec4_t),
        NAME_ENTRY(vertex_format::ubyte_t),
        NAME_ENTRY(vertex_format::ubvec2_t),
        NAME_ENTRY(vertex_format::ubvec3_t),
        NAME_ENTRY(vertex_format::ubvec4_t),
        NAME_ENTRY(vertex_format::double_t),
        NAME_ENTRY(vertex_format::dvec2_t),
        NAME_ENTRY(vertex_format::dvec3_t),
        NAME_ENTRY(vertex_format::dvec4_t),
    });

    enum class buffer_usage_flag : ui32
    {
        transfer_source      = 1 << 0,
        transfer_destination = 1 << 1,
        uniform_texel_buffer = 1 << 2,
        storage_texel_buffer = 1 << 3,
        uniform_buffer       = 1 << 4,
        storage_buffer       = 1 << 5,
        index_buffer         = 1 << 6,
        vertex_buffer        = 1 << 7,
        indirect_buffer      = 1 << 8,
    };

    inline constexpr auto buffer_usage_flag_names = create_name_map<buffer_usage_flag>({
        NAME_ENTRY(buffer_usage_flag::transfer_source),
        NAME_ENTRY(buffer_usage_flag::transfer_destination),
        NAME_ENTRY(buffer_usage_flag::uniform_texel_buffer),
        NAME_ENTRY(buffer_usage_flag::storage_texel_buffer),
        NAME_ENTRY(buffer_usage_flag::uniform_buffer),
        NAME_ENTRY(buffer_usage_flag::storage_buffer),
        NAME_ENTRY(buffer_usage_flag::index_buffer),
        NAME_ENTRY(buffer_usage_flag::vertex_buffer),
        NAME_ENTRY(buffer_usage_flag::indirect_buffer),
    });

    template <typename T>
    concept is_vkflag = std::is_same_v<T, access_flag>
                     || std::is_same_v<T, queue_family>
                     || std::is_same_v<T, instance_create>
                     || std::is_same_v<T, image_aspect_flag>
                     || std::is_same_v<T, pipeline_stage_flag>
                     || std::is_same_v<T, image_usage_flag>
                     || std::is_same_v<T, surface_transform_flag>
                     || std::is_same_v<T, composite_alpha_flag>
                     || std::is_same_v<T, sample_count_flag>
                     || std::is_same_v<T, descriptor_pool_create_flag>
                     || std::is_same_v<T, command_pool_create_flag>
                     || std::is_same_v<T, memory_property_flag>
                     || std::is_same_v<T, memory_flag>
                     || std::is_same_v<T, debug_utils_message_severity_flag>
                     || std::is_same_v<T, debug_utils_message_type_flag>
                     || std::is_same_v<T, command_buffer_usage_flag>
                     || std::is_same_v<T, subpass_description_flag>
                     || std::is_same_v<T, shader_stage_flag>
                     || std::is_same_v<T, color_component>
                     || std::is_same_v<T, buffer_usage_flag>;

    template <typename T>
    concept is_vkenum = std::is_same_v<T, gpu_type>
                     || std::is_same_v<T, descriptor_type>
                     || std::is_same_v<T, attachment_load_op>
                     || std::is_same_v<T, attachment_store_op>
                     || std::is_same_v<T, pipeline_bind_point>
                     || std::is_same_v<T, cmd_buffer_level>
                     || std::is_same_v<T, format>
                     || std::is_same_v<T, present_mode>
                     || std::is_same_v<T, color_space>
                     || std::is_same_v<T, sharing_mode>
                     || std::is_same_v<T, image_tiling>
                     || std::is_same_v<T, image_type>
                     || std::is_same_v<T, image_view_type>
                     || std::is_same_v<T, component_swizzle>
                     || std::is_same_v<T, image_layout>
                     || std::is_same_v<T, memory_usage>
                     || std::is_same_v<T, filter>
                     || std::is_same_v<T, dynamic_state>
                     || std::is_same_v<T, primitive_topology>
                     || std::is_same_v<T, polygon_mode>
                     || std::is_same_v<T, cull_mode>
                     || std::is_same_v<T, front_face>
                     || std::is_same_v<T, blend_factor>
                     || std::is_same_v<T, blend_op>
                     || std::is_same_v<T, shader_kind>
                     || std::is_same_v<T, vertex_input_rate>
                     || std::is_same_v<T, vertex_format>;

    template <typename T>
    concept is_vktype = is_vkflag<T> || is_vkenum<T>;

    template <is_vkflag T>
    static auto operator|(T lhs, T rhs) -> T
    {
        return static_cast<T>(static_cast<ui32>(lhs) | static_cast<ui32>(rhs));
    }

    template <is_vkflag T>
    static auto operator&(T lhs, T rhs) -> T
    {
        return static_cast<T>(static_cast<ui32>(lhs) & static_cast<ui32>(rhs));
    }
} // namespace orb::vk
