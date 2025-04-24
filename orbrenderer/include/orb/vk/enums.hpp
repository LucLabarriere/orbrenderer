#pragma once

#include <orb/utility.hpp>

#include <array>
#include <string_view>
#include <vulkan/vulkan_core.h>

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

    inline constexpr std::array<std::string_view, 5> gpu_type_names = {
        "Other type",
        "Integrated",
        "Discrete",
        "Virtual",
        "CPU"
    };

    enum class queue_family : ui32
    {
        graphics     = 1 << 0,
        compute      = 1 << 1,
        transfer     = 1 << 2,
        sparse       = 1 << 3,
        prot         = 1 << 4,
        video_decode = 1 << 5,
    };

    enum class instance_create : ui32
    {
        portability = 1 << 0,
    };

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

    enum class attachment_load_op : ui32
    {
        load = 0,
        clear,
        dont_care,
        none = 1000400000,
    };

    enum class attachment_store_op : ui32
    {
        store = 0,
        dont_care,
        none = 1000301000,
    };

    enum class pipeline_bind_point : ui32
    {
        graphics = 0,
        compute,
        ray_tracing_khr = 1000165000,
        ray_tracing_nv  = 1000165000,
    };

    enum class cmd_buffer_level : ui32
    {
        primary   = 0,
        secondary = 1,
    };

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

    enum class present_mode : ui32
    {
        immediate_khr,
        mailbox_khr,
        fifo_khr,
        fifo_relaxed_khr,
        shared_demand_refresh_khr = 1000111000,
        shared_continuous_refresh_khr,
    };

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

    enum class sharing_mode : ui32
    {
        exclusive,
        concurrent,
    };

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

    enum class composite_alpha_flag : ui32
    {
        opaque_khr          = 1 << 0,
        pre_multiplied_khr  = 1 << 1,
        post_multiplied_khr = 1 << 2,
        inherit_khr         = 1 << 3,
    };

    enum class image_tiling : ui32
    {
        optimal,
        linear,
        drm_format_modifier_ext = 1000158000,
    };

    enum class image_type : ui32
    {
        _1d,
        _2d,
        _3d,
    };

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

    enum class descriptor_pool_create_flag
    {
        free_descriptor_set   = 1 << 0,
        update_after_bind     = 1 << 1,
        update_after_bind_ext = 1 << 1,
        host_only_ext         = 1 << 2,
        host_only_valve       = 1 << 2,
    };

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

    enum class command_pool_create_flag : ui32
    {
        transient            = 1 << 0,
        reset_command_buffer = 1 << 1,
        prot                 = 1 << 2,
    };

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

    enum class filter : ui32
    {
        nearest,
        linear,
        cubic_ext = 1000015000,
        cubic_img = cubic_ext,
    };

    enum class debug_utils_message_severity_flag : ui32
    {
        verbose = 1 << 0,
        info    = 1 << 4,
        warning = 1 << 8,
        error   = 1 << 12,
    };

    enum class debug_utils_message_type_flag : ui32
    {
        general                = 1 << 0,
        validation             = 1 << 1,
        performance            = 1 << 2,
        device_address_binding = 1 << 3,
    };

    enum class command_buffer_usage_flag : ui32
    {
        one_time_submit      = 1 << 0,
        render_pass_continue = 1 << 1,
        simultaneous_use     = 1 << 2,
    };

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

    // namespace debug_report_object_type
    // {
    //     using enum_t = VkDebugReportObjectTypeEXT;

    //     template <enum_t res>
    //     inline constexpr std::string_view repr = "unknown type";

    //#def // ine define_repr_objtype(orbvar, vkvar) \
    // inline constexpr auto orbvar = vkvar;  \
    // template <>                            \
    // inline constexpr std::string_view repr<orbvar> = #orbvar;

    //#def // ine define_only_objtype(orbvar, vkvar) \
    // inline constexpr auto orbvar = vkvar;

    //     define_repr_objtype(unknown, VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT);
    //     define_repr_objtype(instance, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT);
    //     define_repr_objtype(physical_device, VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT);
    //     define_repr_objtype(device, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT);
    //     define_repr_objtype(queue, VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT);
    //     define_repr_objtype(semaphore, VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT);
    //     define_repr_objtype(command_buffer, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT);
    //     define_repr_objtype(fence, VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT);
    //     define_repr_objtype(device_memory, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT);
    //     define_repr_objtype(buffer, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT);
    //     define_repr_objtype(image, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT);
    //     define_repr_objtype(event, VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT);
    //     define_repr_objtype(query_pool, VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT);
    //     define_repr_objtype(buffer_view, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT);
    //     define_repr_objtype(image_view, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT);
    //     define_repr_objtype(shader_module, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT);
    //     define_repr_objtype(pipeline_cache, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT);
    //     define_repr_objtype(pipeline_layout, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT);
    //     define_repr_objtype(render_pass, VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT);
    //     define_repr_objtype(pipeline, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT);
    //     define_repr_objtype(descriptor_set_layout, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT);
    //     define_repr_objtype(sampler, VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT);
    //     define_repr_objtype(descriptor_pool, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT);
    //     define_repr_objtype(descriptor_set, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT);
    //     define_repr_objtype(framebuffer, VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT);
    //     define_repr_objtype(command_pool, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT);
    //     define_repr_objtype(surface_khr, VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT);
    //     define_repr_objtype(swapchain_khr, VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT);
    //     define_repr_objtype(debug_report_callback, VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT);
    //     define_repr_objtype(display_khr, VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT);
    //     define_repr_objtype(display_mode_khr, VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT);
    //     define_repr_objtype(validation_cache, VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT);
    //     define_repr_objtype(sampler_ycbcr_conversion, VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_EXT);
    //     define_repr_objtype(descriptor_update_template, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_EXT);
    //     define_repr_objtype(cu_module_nvx, VK_DEBUG_REPORT_OBJECT_TYPE_CU_MODULE_NVX_EXT);
    //     define_repr_objtype(cu_function_nvx, VK_DEBUG_REPORT_OBJECT_TYPE_CU_FUNCTION_NVX_EXT);
    //     define_repr_objtype(acceleration_structure_khr, VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR_EXT);
    //     define_repr_objtype(acceleration_structure_nv, VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV_EXT);
    //     define_repr_objtype(buffer_collection_fuchsia, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA_EXT);
    //     define_only_objtype(debug_report, VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT);
    //     define_only_objtype(descriptor_update_template_khr, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_KHR_EXT);
    //     define_only_objtype(sampler_ycbcr_conversion_khr, VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR_EXT);

    //     inline constexpr auto get_repr(enum_t e) -> std::string_view
    //     {
    //         switch (e)
    //         {
    //         case unknown: return repr<unknown>;
    //         case instance: return repr<instance>;
    //         case physical_device: return repr<physical_device>;
    //         case device: return repr<device>;
    //         case queue: return repr<queue>;
    //         case semaphore: return repr<semaphore>;
    //         case command_buffer: return repr<command_buffer>;
    //         case fence: return repr<fence>;
    //         case device_memory: return repr<device_memory>;
    //         case buffer: return repr<buffer>;
    //         case image: return repr<image>;
    //         case event: return repr<event>;
    //         case query_pool: return repr<query_pool>;
    //         case buffer_view: return repr<buffer_view>;
    //         case image_view: return repr<image_view>;
    //         case shader_module: return repr<shader_module>;
    //         case pipeline_cache: return repr<pipeline_cache>;
    //         case pipeline_layout: return repr<pipeline_layout>;
    //         case render_pass: return repr<render_pass>;
    //         case pipeline: return repr<pipeline>;
    //         case descriptor_set_layout: return repr<descriptor_set_layout>;
    //         case sampler: return repr<sampler>;
    //         case descriptor_pool: return repr<descriptor_pool>;
    //         case descriptor_set: return repr<descriptor_set>;
    //         case framebuffer: return repr<framebuffer>;
    //         case command_pool: return repr<command_pool>;
    //         case surface_khr: return repr<surface_khr>;
    //         case swapchain_khr: return repr<swapchain_khr>;
    //         case debug_report_callback: return repr<debug_report_callback>;
    //         case display_khr: return repr<display_khr>;
    //         case display_mode_khr: return repr<display_mode_khr>;
    //         case validation_cache: return repr<validation_cache>;
    //         case sampler_ycbcr_conversion: return repr<sampler_ycbcr_conversion>;
    //         case descriptor_update_template: return repr<descriptor_update_template>;
    //         case cu_module_nvx: return repr<cu_module_nvx>;
    //         case cu_function_nvx: return repr<cu_function_nvx>;
    //         case acceleration_structure_khr: return repr<acceleration_structure_khr>;
    //         case acceleration_structure_nv: return repr<acceleration_structure_nv>;
    //         case buffer_collection_fuchsia: return repr<buffer_collection_fuchsia>;

    //         default: return repr<VK_DEBUG_REPORT_OBJECT_TYPE_MAX_ENUM_EXT>;
    //         }
    //     }
    // } // namespace debug_report_object_type

    // namespace obj_types
    // {
    //     using enum_t = VkObjectType;

    //     inline constexpr auto unknown = VK_OBJECT_TYPE_UNKNOWN;
    //     template <typename T>
    //     inline constexpr enum_t obj_type = unknown;

    //#def // ine define_obj_type(name, obj_type_name, vk_type_name) \
    // inline constexpr auto name = obj_type_name;            \
    // template <>                                            \
    // inline constexpr auto obj_type<vk_type_name> = name;

    //     define_obj_type(instance, VK_OBJECT_TYPE_INSTANCE, VkInstance);
    //     define_obj_type(physical_device, VK_OBJECT_TYPE_PHYSICAL_DEVICE, VkPhysicalDevice);
    //     define_obj_type(device, VK_OBJECT_TYPE_DEVICE, VkDevice);
    //     define_obj_type(queue, VK_OBJECT_TYPE_QUEUE, VkQueue);
    //     define_obj_type(semaphore, VK_OBJECT_TYPE_SEMAPHORE, VkSemaphore);
    //     define_obj_type(command_buffer, VK_OBJECT_TYPE_COMMAND_BUFFER, VkCommandBuffer);
    //     define_obj_type(fence, VK_OBJECT_TYPE_FENCE, VkFence);
    //     define_obj_type(device_memory, VK_OBJECT_TYPE_DEVICE_MEMORY, VkDeviceMemory);
    //     define_obj_type(buffer, VK_OBJECT_TYPE_BUFFER, VkBuffer);
    //     define_obj_type(image, VK_OBJECT_TYPE_IMAGE, VkImage);
    //     define_obj_type(event, VK_OBJECT_TYPE_EVENT, VkEvent);
    //     define_obj_type(query_pool, VK_OBJECT_TYPE_QUERY_POOL, VkQueryPool);
    //     define_obj_type(buffer_view, VK_OBJECT_TYPE_BUFFER_VIEW, VkBufferView);
    //     define_obj_type(image_view, VK_OBJECT_TYPE_IMAGE_VIEW, VkImageView);
    //     define_obj_type(shader_module, VK_OBJECT_TYPE_SHADER_MODULE, VkShaderModule);
    //     define_obj_type(pipeline_cache, VK_OBJECT_TYPE_PIPELINE_CACHE, VkPipelineCache);
    //     define_obj_type(pipeline_layout, VK_OBJECT_TYPE_PIPELINE_LAYOUT, VkPipelineLayout);
    //     define_obj_type(render_pass, VK_OBJECT_TYPE_RENDER_PASS, VkRenderPass);
    //     define_obj_type(pipeline, VK_OBJECT_TYPE_PIPELINE, VkPipeline);
    //     define_obj_type(descriptor_set_layout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, VkDescriptorSetLayout);
    //     define_obj_type(sampler, VK_OBJECT_TYPE_SAMPLER, VkSampler);
    //     define_obj_type(descriptor_pool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, VkDescriptorPool);
    //     define_obj_type(descriptor_set, VK_OBJECT_TYPE_DESCRIPTOR_SET, VkDescriptorSet);
    //     define_obj_type(framebuffer, VK_OBJECT_TYPE_FRAMEBUFFER, VkFramebuffer);
    //     define_obj_type(command_pool, VK_OBJECT_TYPE_COMMAND_POOL, VkCommandPool);
    //     define_obj_type(sampler_ycbcr_conversion, VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION, VkSamplerYcbcrConversion);
    //     define_obj_type(descriptor_update_template, VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE, VkDescriptorUpdateTemplate);
    //     define_obj_type(private_data_slot, VK_OBJECT_TYPE_PRIVATE_DATA_SLOT, VkPrivateDataSlot);
    //     define_obj_type(surface_khr, VK_OBJECT_TYPE_SURFACE_KHR, VkSurfaceKHR);
    //     define_obj_type(swapchain_khr, VK_OBJECT_TYPE_SWAPCHAIN_KHR, VkSwapchainKHR);
    //     define_obj_type(display_khr, VK_OBJECT_TYPE_DISPLAY_KHR, VkDisplayKHR);
    //     define_obj_type(display_mode_khr, VK_OBJECT_TYPE_DISPLAY_MODE_KHR, VkDisplayModeKHR);
    //     define_obj_type(debug_report_callback_ext, VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT, VkDebugReportCallbackEXT);
    //     define_obj_type(video_session_khr, VK_OBJECT_TYPE_VIDEO_SESSION_KHR, VkVideoSessionKHR);
    //     define_obj_type(video_session_parameters_khr, VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR, VkVideoSessionParametersKHR);
    //     define_obj_type(cu_module_nvx, VK_OBJECT_TYPE_CU_MODULE_NVX, VkCuModuleNVX);
    //     define_obj_type(cu_function_nvx, VK_OBJECT_TYPE_CU_FUNCTION_NVX, VkCuFunctionNVX);
    //     define_obj_type(debug_utils_messenger_ext, VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT, VkDebugUtilsMessengerEXT);
    //     define_obj_type(acceleration_structure_khr, VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, VkAccelerationStructureKHR);
    //     define_obj_type(validation_cache_ext, VK_OBJECT_TYPE_VALIDATION_CACHE_EXT, VkValidationCacheEXT);
    //     define_obj_type(performance_configuration_intel, VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL, VkPerformanceConfigurationINTEL);
    //     define_obj_type(deferred_operation_khr, VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR, VkDeferredOperationKHR);
    //     define_obj_type(indirect_commands_layout_nv, VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV, VkIndirectCommandsLayoutNV);
    //     define_obj_type(micromap_ext, VK_OBJECT_TYPE_MICROMAP_EXT, VkMicromapEXT);
    //     define_obj_type(optical_flow_session_nv, VK_OBJECT_TYPE_OPTICAL_FLOW_SESSION_NV, VkOpticalFlowSessionNV);

    //     template <typename T>
    //     concept vk_type = obj_type<T> != unknown;
    // } // namespace obj_types

    // template <typename T>
    // concept vk_type = obj_types::vk_type<T>;

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

    enum class polygon_mode : ui32
    {
        fill,
        line,
        point,
        fill_rectangle_nv = 1000153000,
    };

    enum class cull_mode : ui32
    {
        none,
        front,
        back,
        front_and_back,
    };

    enum class front_face : ui32
    {
        counter_clockwise,
        clockwise,
    };

    enum class color_component : ui32
    {
        r = 1 << 0,
        g = 1 << 1,
        b = 1 << 2,
        a = 1 << 3,
    };

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

    enum class vertex_input_rate : ui32
    {
        vertex,
        instance,
    };

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
