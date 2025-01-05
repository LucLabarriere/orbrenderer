#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <vulkan/vulkan_core.h>

#include <orb/box.hpp>
#include <orb/result.hpp>

using VkCommandBuffer = struct VkCommandBuffer_T*;

namespace orb
{
    namespace glfw
    {
        class window_t;
    }

    namespace vk
    {
        struct gpu_t;
        struct instance_t;
        struct device_t;
        struct swapchain_t;
        struct render_pass_t;

        namespace imgui
        {
            struct imgui_init_args_t
            {
                weak<glfw::window_t>  window;
                weak<vk::instance_t>  instance;
                weak<vk::gpu_t>       gpu;
                weak<vk::device_t>    device;
                weak<vk::swapchain_t> swapchain;

                VkRenderPass     pass;
                VkDescriptorPool desc_pool;
            };

            [[nodiscard]] auto initialize(imgui_init_args_t) -> result<void>;
            void               new_frame();
            void               render();
            void               submit_render(VkCommandBuffer cmd);
            void               terminate();

        } // namespace imgui

    } // namespace vk

} // namespace orb
