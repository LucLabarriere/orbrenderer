#include "orb/vk/imgui.hpp"
#include "orb/glfw/window.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/gpu.hpp"
#include "orb/vk/instance.hpp"
#include "orb/vk/render_pass.hpp"
#include "orb/vk/swapchain.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace orb::vk::imgui
{

    auto initialize(imgui_init_args_t args) -> result<void>
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(args.window->get_handle<GLFWwindow>(), true);
        ImGui_ImplVulkan_InitInfo init_info {};
        init_info.Instance        = args.instance->handle;
        init_info.PhysicalDevice  = args.gpu->handle;
        init_info.Device          = args.device->handle;
        init_info.QueueFamily     = 0;
        init_info.Queue           = args.device->queues[0];
        init_info.PipelineCache   = nullptr;
        init_info.DescriptorPool  = args.pass->desc_pool;
        init_info.RenderPass      = args.pass->handle;
        init_info.Subpass         = 0;
        init_info.MinImageCount   = args.swapchain->min_img_count;
        init_info.ImageCount      = args.swapchain->img_count;
        init_info.MSAASamples     = vk::sample_count_flags::_1;
        init_info.Allocator       = nullptr;
        init_info.CheckVkResultFn = nullptr;

        if (!ImGui_ImplVulkan_Init(&init_info)) { return error_t { "Could not initialize ImGui" }; }

        return {};
    }

    void new_frame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void render()
    {
        ImGui::Render();
    }

    void submit_render(VkCommandBuffer cmd)
    {
        ImDrawData* draw_data = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(draw_data, cmd);
    }

    void terminate()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

} // namespace orb::imgui
