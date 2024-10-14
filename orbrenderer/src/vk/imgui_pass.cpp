#include "orb/vk/imgui_pass.hpp"
#include "orb/glfw/window.hpp"
#include "orb/vk/device.hpp"
#include "orb/vk/gpu.hpp"
#include "orb/vk/instance.hpp"
#include "orb/vk/swapchain.hpp"

#include "glfw/glfw_header.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <orb/flux.hpp>

namespace orb::vk
{
    inline constexpr auto render_img_format = formats::b8g8r8a8_unorm;

    auto imgui_pass_builder_t::prepare(weak<glfw::window_t> window,
                                       weak<instance_t>     instance,
                                       weak<gpu_t>          gpu,
                                       weak<device_t>       device,
                                       weak<swapchain_t>    swapchain) -> result<imgui_pass_builder_t>
    {
        imgui_pass_builder_t b;
        b.window    = window;
        b.instance  = instance;
        b.gpu       = gpu;
        b.device    = device;
        b.swapchain = swapchain;

        return b;
    }

    auto imgui_pass_builder_t::desc_pool_size(desc_types::enum_t type, ui32 count) -> imgui_pass_builder_t&
    {
        pool_sizes.push_back({
            .type            = type,
            .descriptorCount = count,
        });

        return *this;
    }

    auto imgui_pass_builder_t::semaphores(size_t count) -> imgui_pass_builder_t&
    {
        semaphore_count = count;
        return *this;
    }

    auto imgui_pass_builder_t::fences(size_t count) -> imgui_pass_builder_t&
    {
        fence_count = count;
        return *this;
    }

    auto imgui_pass_builder_t::cmds(size_t count) -> imgui_pass_builder_t&
    {
        cmd_count = count;
        return *this;
    }

    auto imgui_pass_builder_t::framebuffers(size_t count) -> imgui_pass_builder_t&
    {
        framebuffer_count = count;
        return *this;
    }

    auto imgui_pass_builder_t::build() -> result<imgui_pass_t>
    {
        imgui_pass_t pass { .device = device };

        auto pool_info          = structs::create::descriptor_pool();
        pool_info.flags         = descriptor_pool_create_flags::free_descriptor_set_bit;
        pool_info.maxSets       = 1;
        pool_info.poolSizeCount = pool_sizes.size();
        pool_info.pPoolSizes    = pool_sizes.data();

        if (auto res = vkCreateDescriptorPool(device->handle, &pool_info, nullptr, &pass.desc_pool);
            res != vkres::ok)
        {
            return error_t { "Could not create ImGui descriptor pool" };
        }

        VkAttachmentDescription attachment = {
            .format         = render_img_format,
            .samples        = sample_count_flags::_1,
            .loadOp         = attachment_load_ops::clear,
            .storeOp        = attachment_store_ops::store,
            .stencilLoadOp  = attachment_load_ops::dont_care,
            .stencilStoreOp = attachment_store_ops::dont_care,
            .initialLayout  = image_layouts::undefined,
            .finalLayout    = image_layouts::present_src_khr,
        };

        VkAttachmentReference color_attachment = {
            .attachment = 0,
            .layout     = image_layouts::color_attachment_optimal,
        };

        VkSubpassDescription subpass = {
            .pipelineBindPoint    = pipeline_bind_points::graphics,
            .colorAttachmentCount = 1,
            .pColorAttachments    = &color_attachment,
        };

        VkSubpassDependency dependency = {
            .srcSubpass    = VK_SUBPASS_EXTERNAL,
            .dstSubpass    = 0,
            .srcStageMask  = pipeline_stage_flags::color_attachment_output,
            .dstStageMask  = pipeline_stage_flags::color_attachment_output,
            .srcAccessMask = 0,
            .dstAccessMask = access_flags::color_attachment_write,
        };

        auto pass_info = structs::create::render_pass();

        pass_info.attachmentCount = 1;
        pass_info.pAttachments    = &attachment;
        pass_info.subpassCount    = 1;
        pass_info.pSubpasses      = &subpass;
        pass_info.dependencyCount = 1;
        pass_info.pDependencies   = &dependency;

        if (auto res = vkCreateRenderPass(device->handle, &pass_info, nullptr, &pass.handle);
            res != vkres::ok)
        {
            return error_t { "Could not create render pass: {}", (i64)res };
        }

        auto cmd_pool_info             = structs::create::cmd_pool();
        cmd_pool_info.queueFamilyIndex = gpu->queue_families.front().index;
        cmd_pool_info.flags            = command_pool_create_flags::reset_command_buffer_bit;
        if (auto res = vkCreateCommandPool(device->handle, &cmd_pool_info, nullptr, &pass.cmd_pool);
            res != vkres::ok)
        {
            return error_t { "Could not create ImGui command pool: {}", (i64)res };
        }

        if (cmd_count != 0)
        {
            pass.cmds.resize(cmd_count);
            auto cmd_info               = structs::create::cmd_buffer();
            cmd_info.commandBufferCount = pass.cmds.size();
            cmd_info.commandPool        = pass.cmd_pool;
            cmd_info.level              = cmd_buffer_levels::primary;

            if (auto res = vkAllocateCommandBuffers(device->handle, &cmd_info, pass.cmds.data());
                res != vkres::ok)
            {
                return error_t { "Could not allocate ImGui command buffer: {}", (i64)res };
            }
        }

        auto sem_info = structs::create::semaphore();
        pass.semaphores.resize(semaphore_count);
        for (auto& sem : pass.semaphores)
        {
            if (auto res = vkCreateSemaphore(device->handle, &sem_info, nullptr, &sem); res != vkres::ok)
            {
                return error_t { "Could not create ImGui semaphore: {}", (i64)res };
            }
        }

        auto fence_info = structs::create::fence();
        pass.fences.resize(fence_count);
        for (auto& fence : pass.fences)
        {
            if (auto res = vkCreateFence(device->handle, &fence_info, nullptr, &fence); res != vkres::ok)
            {
                return error_t { "Could not create ImGui fence: {}", (i64)res };
            }
        }

        auto img_info          = structs::create::image();
        img_info.imageType     = image_types::_2d;
        img_info.format        = render_img_format;
        img_info.extent.width  = swapchain->width;
        img_info.extent.height = swapchain->height;
        img_info.extent.depth  = 1;
        img_info.mipLevels     = 1;
        img_info.arrayLayers   = 1;
        img_info.samples       = sample_count_flags::_1;
        img_info.tiling        = image_tilings::optimal;
        img_info.usage         = image_usage_flags::transfer_src | image_usage_flags::color_attachment;
        img_info.sharingMode   = sharing_modes::exclusive;
        img_info.initialLayout = image_layouts::undefined;

        auto mem_info  = structs::create::allocation();
        mem_info.usage = memory_usages::usage_auto_prefer_device;

        auto view_info                        = structs::create::image_view();
        view_info.viewType                    = image_view_types::type_2d;
        view_info.format                      = render_img_format;
        view_info.subresourceRange.aspectMask = image_aspect_flags::color;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.layerCount = 1;

        auto fb_info            = structs::create::framebuffer();
        fb_info.attachmentCount = 1;
        fb_info.width           = swapchain->width;
        fb_info.height          = swapchain->height;
        fb_info.layers          = 1;
        fb_info.renderPass      = pass.handle;

        pass.images.resize(framebuffer_count);
        pass.mems.resize(framebuffer_count);
        pass.views.resize(framebuffer_count);
        pass.fbs.resize(framebuffer_count);

        for (auto [img, mem, view, fb] : flux::zip_all_mut(pass.images, pass.mems, pass.views, pass.fbs))
        {

            if (auto r = vmaCreateImage(device->allocator, &img_info, &mem_info, &img, &mem, nullptr);
                r != vkres::ok)
            {
                return error_t { "Could not create image: {}", (i64)r };
            }

            view_info.image = img;
            if (auto r = vkCreateImageView(device->handle, &view_info, nullptr, &view); r != vkres::ok)
            {
                return error_t { "Could not create view: {}", (i64)r };
            }

            fb_info.pAttachments = &view;
            if (auto r = vkCreateFramebuffer(device->handle, &fb_info, nullptr, &fb); r != vkres::ok)
            {
                return error_t { "Could not create framebuffer: {}", (i64)r };
            }
        }

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
        ImGui_ImplGlfw_InitForVulkan(window->get_handle<GLFWwindow>(), true);
        ImGui_ImplVulkan_InitInfo init_info {};
        init_info.Instance        = instance->handle;
        init_info.PhysicalDevice  = gpu->handle;
        init_info.Device          = device->handle;
        init_info.QueueFamily     = 0;
        init_info.Queue           = device->queues[0];
        init_info.PipelineCache   = nullptr;
        init_info.DescriptorPool  = pass.desc_pool;
        init_info.RenderPass      = pass.handle;
        init_info.Subpass         = 0;
        init_info.MinImageCount   = swapchain->min_img_count;
        init_info.ImageCount      = swapchain->img_count;
        init_info.MSAASamples     = sample_count_flags::_1;
        init_info.Allocator       = nullptr;
        init_info.CheckVkResultFn = nullptr;
        ImGui_ImplVulkan_Init(&init_info);

        return pass;
    }

    void destroy(imgui_pass_t& pass)
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyCommandPool(pass.device->handle, pass.cmd_pool, nullptr);

        for (auto& sem : pass.semaphores)
        {
            vkDestroySemaphore(pass.device->handle, sem, nullptr);
        }

        for (auto& fence : pass.fences)
        {
            vkDestroyFence(pass.device->handle, fence, nullptr);
        }

        for (auto [img, mem, view, fb] : flux::zip_all_mut(pass.images, pass.mems, pass.views, pass.fbs))
        {

            vkDestroyFramebuffer(pass.device->handle, fb, nullptr);
            vkDestroyImageView(pass.device->handle, view, nullptr);
            vmaDestroyImage(pass.device->allocator, img, mem);
        }

        vkDestroyDescriptorPool(pass.device->handle, pass.desc_pool, nullptr);
        vkDestroyRenderPass(pass.device->handle, pass.handle, nullptr);
    }

} // namespace orb::vk
