#include "orb/vk/imgui.hpp"

#include "orb/glfw/window.hpp"
#include "orb/vk/desc_pool.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/gpu.hpp"
#include "orb/vk/instance.hpp"
#include "orb/vk/render_pass.hpp"
#include "orb/vk/swapchain.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace orb::vk
{
    imgui_driver_t::~imgui_driver_t()
    {
        destroy();
    }

    imgui_driver_t::imgui_driver_t(imgui_driver_t&& other) noexcept
    {
        destroy();

        m_initialized       = other.m_initialized;
        other.m_initialized = false;
    }

    auto imgui_driver_t::operator=(imgui_driver_t&& other) noexcept -> imgui_driver_t&
    {
        destroy();

        m_initialized       = other.m_initialized;
        other.m_initialized = false;
        return *this;
    }

    void imgui_driver_t::destroy()
    {
        if (!m_initialized) return;

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    auto imgui_driver_t::new_frame() -> void
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void imgui_driver_t::render()
    {
        ImGui::Render();
    }

    void imgui_driver_t::submit_render(VkCommandBuffer cmd)
    {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    }

    auto imgui_driver_builder_t::prepare(weak<glfw::window_t> win,
                                         weak<instance_t>     instance,
                                         weak<gpu_t>          gpu,
                                         weak<device_t>       device,
                                         weak<swapchain_t>    sc,
                                         weak<desc_pool_t>    desc_pool,
                                         weak<render_pass_t>  pass) -> imgui_driver_builder_t
    {
        imgui_driver_builder_t builder {};
        builder.m_window    = win;
        builder.m_instance  = instance;
        builder.m_gpu       = gpu;
        builder.m_device    = device;
        builder.m_swapchain = sc;
        builder.m_desc_pool = desc_pool;

        builder.m_info.Instance        = instance->handle;
        builder.m_info.PhysicalDevice  = gpu->handle;
        builder.m_info.Device          = device->handle;
        builder.m_info.QueueFamily     = 0;
        builder.m_info.Queue           = device->queues[0];
        builder.m_info.PipelineCache   = nullptr;
        builder.m_info.DescriptorPool  = desc_pool->handle;
        builder.m_info.RenderPass      = pass->handle;
        builder.m_info.Subpass         = 0;
        builder.m_info.MinImageCount   = sc->info.minImageCount;
        builder.m_info.ImageCount      = sc->img_count;
        builder.m_info.MSAASamples     = vk::sample_count_flags::_1;
        builder.m_info.Allocator       = nullptr;
        builder.m_info.CheckVkResultFn = nullptr;

        return builder;
    }

    auto imgui_driver_builder_t::build() -> result<imgui_driver_t>
    {
        imgui_driver_t driver {};

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io    = ImGui::GetIO();
        io.ConfigFlags = m_flags;

        if (m_dark_theme)
        {
            ImGui::StyleColorsDark();
        }
        else
        {
            ImGui::StyleColorsLight();
        }

        // Setup Platform/Renderer backends
        if (!ImGui_ImplGlfw_InitForVulkan(m_window->get_handle<GLFWwindow>(), true))
        {
            return error_t { "Could not initialize GLFW for ImGui" };
        }

        if (!ImGui_ImplVulkan_Init(&m_info))
        {
            return error_t { "Could not initialize Vulkan for ImGui" };
        }

        driver.m_initialized = true;

        return driver;
    }

} // namespace orb::vk
