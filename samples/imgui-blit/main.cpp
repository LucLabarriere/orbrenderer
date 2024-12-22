#include <array>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <iostream>
#include <vector>

#include <orb/eval.hpp>
#include <orb/flux.hpp>

#include <GLFW/glfw3.h>
#include <orb/glfw.hpp>
#include <orb/vk.hpp>
#include <orb/vk/imgui.hpp>

static VkRenderPass create_imgui_pass(VkDevice device,
                                      VkFormat sc_img_format)
{
    // Create the render pass
    VkAttachmentDescription color_attachment = {};
    color_attachment.format                  = sc_img_format;
    color_attachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment            = 0;
    color_attachment_ref.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments    = &color_attachment_ref;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass          = 0;
    dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask       = 0;
    dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount        = 1;
    render_pass_info.pAttachments           = &color_attachment;
    render_pass_info.subpassCount           = 1;
    render_pass_info.pSubpasses             = &subpass;
    render_pass_info.dependencyCount        = 1;
    render_pass_info.pDependencies          = &dependency;

    VkRenderPass render_pass {};
    if (vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }

    return render_pass;
}

static auto create_imgui_fbs(VkDevice               device,
                             VkRenderPass           render_pass,
                             VkExtent2D             sc_extent,
                             std::span<VkImage>     sc_imgs,
                             std::span<VkImageView> sc_views) -> std::vector<VkFramebuffer>
{
    std::vector<VkFramebuffer> framebuffers;
    const size_t               sc_img_count = sc_imgs.size();

    framebuffers.resize(sc_img_count);
    for (size_t i = 0; i < sc_img_count; ++i)
    {
        std::array attachments = {
            sc_views[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass              = render_pass;
        framebufferInfo.attachmentCount         = 1;
        framebufferInfo.pAttachments            = attachments.data();
        framebufferInfo.width                   = sc_extent.width;
        framebufferInfo.height                  = sc_extent.height;
        framebufferInfo.layers                  = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }

    return framebuffers;
}

auto main() -> int
{
    using namespace orb;

    constexpr auto portability = vk::khr_extensions::portability_enumeration;

    try
    {
        // Initialize GLFW and the window
        glfw::driver_t::initialize().throw_if_error();
        glfw::window_t window = glfw::driver_t::create_window_for_vk().unwrap();

        // Initialize the vulkan instance
        auto instance_builder = vk::instance_builder_t::prepare().unwrap();

        if (instance_builder.is_ext_available(portability))
        {
            instance_builder.add_extension(portability);
            instance_builder.create_info.flags |= vk::instance_create::portability;
        }

        vk::instance_t instance = instance_builder.add_glfw_required_extensions()
                                      .add_extension(vk::khr_extensions::device_properties_2)
                                      .add_extension(vk::extensions::debug_utils)
                                      .debug_layer(vk::validation_layers::validation)
                                      .build()
                                      .unwrap();

        // Select the GPU
        box<vk::gpu_t> gpu = orb::eval | [&] {
            auto gpus = vk::available_gpus_t::create(instance.handle).unwrap();

            for (const auto& [i, gpu] : flux::enumerate(gpus))
            {
                if (gpu->device_type == vk::gpu_types::discrete) { return gpus.select(i); }
            }

            println("No discrete GPU found, trying out integrated GPUs");
            for (const auto& [i, gpu] : flux::enumerate(gpus))
            {
                if (gpu->device_type == vk::gpu_types::integrated) { return gpus.select(i); }
            }

            println("No integrated GPU found either, return first available device");

            return gpus.select(0);
        };

        // Print informations on select gpu
        describe(*gpu);

        // Select queue family
        vk::queue_family_t graphics_queue_family = orb::eval | [&]() -> vk::queue_family_t& {
            for (auto& qf : gpu->queue_families)
            {
                if (qf.properties.queueFlags & vk::queue_families::graphics) { return qf; }
            }

            panic("No suitable queue family found");
        };

        println("- Selected queue family {} with {} queues",
                graphics_queue_family.index,
                graphics_queue_family.properties.queueCount);

        // Create vulkan device
        constexpr std::array queue_priorities { 1.0f };

        vk::device_t device =
            vk::device_builder_t::prepare(instance.handle)
                .unwrap()
                .add_extension(vk::khr_extensions::swapchain)
                .add_queues(graphics_queue_family, queue_priorities)
                .build(gpu.getmut())
                .unwrap();

        println("- Created vulkan device");

        constexpr ui32 max_frames_in_flight = 2;

        // Create swapchain
        vk::swapchain_t swapchain = //
            vk::swapchain_builder_t::prepare(weak { &instance },
                                             gpu.getmut(),
                                             weak { &device },
                                             weak { &window })
                .unwrap()
                .fb_dimensions_from_window()
                .present_queue_family_index(0)

                .color_space(vk::color_spaces::srgb_nonlinear_khr)
                .format(vk::formats::b8g8r8a8_unorm)
                .format(vk::formats::r8g8b8a8_unorm)
                .format(vk::formats::b8g8r8_unorm)
                .format(vk::formats::r8g8b8_unorm)

                .present_mode(vk::present_modes::mailbox_khr)
                .present_mode(vk::present_modes::immediate_khr)
                .present_mode(vk::present_modes::fifo_khr)

                .semaphores(max_frames_in_flight)

                .build()
                .unwrap();

        println("- Created swapchain");

        // Create ImGui pass
        auto imgui_pass = create_imgui_pass(device.handle,
                                            swapchain.format.format);

        auto imgui_fbs = create_imgui_fbs(device.handle,
                                          imgui_pass,
                                          swapchain.extent,
                                          swapchain.images,
                                          swapchain.views);

        // Create ImGui descriptor pool
        std::array<VkDescriptorPoolSize, 1> pool_sizes {};
        pool_sizes[0] = {
            .type            = vk::desc_types::sampler,
            .descriptorCount = 100,
        };

        auto pool_info          = vk::structs::create::descriptor_pool();
        pool_info.flags         = vk::descriptor_pool_create_flags::free_descriptor_set_bit;
        pool_info.maxSets       = 1;
        pool_info.poolSizeCount = pool_sizes.size();
        pool_info.pPoolSizes    = pool_sizes.data();
        VkDescriptorPool desc_pool {};

        if (auto r = vkCreateDescriptorPool(device.handle, &pool_info, nullptr, &desc_pool);
            r != vk::vkres::ok)
        {
            println("Could not create descriptor pool: {}", vk::vkres::get_repr(r));
            return 1;
        }

        // Synchronization
        std::vector<VkSemaphore> img_avail_semaphores;
        std::vector<VkSemaphore> render_finished_semaphores;
        std::vector<VkFence>     fences;

        auto sem_info = vk::structs::create::semaphore();
        img_avail_semaphores.resize(max_frames_in_flight);
        render_finished_semaphores.resize(max_frames_in_flight);
        for (auto& sem : img_avail_semaphores)
        {
            if (auto res = vkCreateSemaphore(device.handle, &sem_info, nullptr, &sem); res != vk::vkres::ok)
            {
                println("Could not create semaphore: {}", vk::vkres::get_repr(res));
                return 1;
            }
        }

        for (auto& sem : render_finished_semaphores)
        {
            if (auto res = vkCreateSemaphore(device.handle, &sem_info, nullptr, &sem); res != vk::vkres::ok)
            {
                println("Could not create semaphore: {}", vk::vkres::get_repr(res));
                return 1;
            }
        }

        auto fence_info = vk::structs::create::fence();
        fences.resize(max_frames_in_flight);
        for (auto& fence : fences)
        {
            if (auto res = vkCreateFence(device.handle, &fence_info, nullptr, &fence); res != vk::vkres::ok)
            {
                println("Could not create fence: {}", vk::vkres::get_repr(res));
                return 1;
            }
        }

        // Execution
        VkCommandPool cmd_pool {};
        auto          cmd_pool_info    = vk::structs::create::cmd_pool();
        cmd_pool_info.queueFamilyIndex = gpu->queue_families.front().index;
        cmd_pool_info.flags            = vk::command_pool_create_flags::reset_command_buffer_bit;
        if (auto res = vkCreateCommandPool(device.handle, &cmd_pool_info, nullptr, &cmd_pool);
            res != vk::vkres::ok)
        {
            println("Could not create command pool: {}", vk::vkres::get_repr(res));
            return 1;
        }

        std::vector<VkCommandBuffer> cmd_buffers;
        cmd_buffers.resize(max_frames_in_flight);
        auto cmd_info               = vk::structs::create::cmd_buffer();
        cmd_info.commandBufferCount = cmd_buffers.size();
        cmd_info.commandPool        = cmd_pool;
        cmd_info.level              = vk::cmd_buffer_levels::primary;

        if (auto res = vkAllocateCommandBuffers(device.handle, &cmd_info, cmd_buffers.data());
            res != vk::vkres::ok)
        {
            println("Could not allocate command buffer: {}", vk::vkres::get_repr(res));
            return 1;
        }

        // Init ImGui
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
        ImGui_ImplGlfw_InitForVulkan(window.get_handle<GLFWwindow>(), true);
        ImGui_ImplVulkan_InitInfo init_info {};
        init_info.Instance        = instance.handle;
        init_info.PhysicalDevice  = gpu->handle;
        init_info.Device          = device.handle;
        init_info.QueueFamily     = 0;
        init_info.Queue           = device.queues[0];
        init_info.PipelineCache   = nullptr;
        init_info.DescriptorPool  = desc_pool;
        init_info.RenderPass      = imgui_pass;
        init_info.Subpass         = 0;
        init_info.MinImageCount   = swapchain.img_count;
        init_info.ImageCount      = swapchain.img_count;
        init_info.MSAASamples     = vk::sample_count_flags::_1;
        init_info.Allocator       = nullptr;
        init_info.CheckVkResultFn = nullptr;

        if (!ImGui_ImplVulkan_Init(&init_info))
        {
            println("Could not initialize ImGui");
            return 1;
        }

        // While loop
        uint32_t frame = 0;
        while (!window.should_close())
        {
            // Poll events (specific to your windowing library, e.g., GLFW)
            glfwPollEvents();

            // Start a new ImGui frame
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame(); // Assuming you're using GLFW for windowing
            ImGui::NewFrame();

            // Render the ImGui demo window
            ImGui::ShowDemoWindow();

            // Prepare ImGui for rendering
            ImGui::Render();

            // Wait fences
            const auto fence_res = vkWaitForFences(device.handle, 1, &fences[frame], VK_TRUE, UINT64_MAX);
            if (fence_res != VK_SUCCESS)
            {
                println("failed to wait for fence!");
                return 1;
            }
            // Reset the fence
            const auto reset_fence = vkResetFences(device.handle, 1, &fences[frame]);
            if (reset_fence != VK_SUCCESS)
            {
                println("failed to reset fence!");
                return 1;
            }

            // Acquire the next swapchain image
            uint32_t img_index {};
            VkResult result = vkAcquireNextImageKHR(device.handle,
                                                    swapchain.handle,
                                                    UINT64_MAX,
                                                    img_avail_semaphores[frame],
                                                    VK_NULL_HANDLE,
                                                    &img_index);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            {
                println("Requiring sc rebuilt");
                return 1;
            }

            // Begin command buffer recording
            VkCommandBuffer          cmd       = cmd_buffers[frame];
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(cmd, &beginInfo);

            // Begin the render pass
            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass            = imgui_pass;
            renderPassInfo.framebuffer           = imgui_fbs[img_index];
            renderPassInfo.renderArea.offset     = { .x = 0, .y = 0 };
            renderPassInfo.renderArea.extent     = swapchain.extent;

            VkClearValue clearColor        = { 0.0f, 0.0f, 0.0f, 1.0f };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues    = &clearColor;

            vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            // Render ImGui
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

            // End the render pass
            vkCmdEndRenderPass(cmd);

            // End command buffer recording
            vkEndCommandBuffer(cmd);

            // Submit the command buffer
            VkSubmitInfo submit_info = {};
            submit_info.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            std::array<VkPipelineStageFlags, 1> wait_stages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
            submit_info.waitSemaphoreCount                  = 1;
            submit_info.pWaitSemaphores                     = &img_avail_semaphores[frame];
            submit_info.pWaitDstStageMask                   = wait_stages.data();

            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers    = &cmd;

            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores    = &render_finished_semaphores[frame];

            if (vkQueueSubmit(device.queues[0], 1, &submit_info, fences[frame]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to submit draw command buffer!");
            }

            // Present the rendered image
            VkPresentInfoKHR present_info = {};
            present_info.sType            = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores    = &render_finished_semaphores[frame];

            present_info.swapchainCount = 1;
            present_info.pSwapchains    = &swapchain.handle;
            present_info.pImageIndices  = &img_index;

            result = vkQueuePresentKHR(device.queues[0], &present_info);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            {
                println("Requiring sc rebuilt");
            }

            frame = (frame + 1) % max_frames_in_flight;
        }

        vk::device_idle(device);

        // Terminate glfw
        window.destroy();
        println("- Destroyed window");

        // vk::destroy(imgui_pass);
        vk::destroy(swapchain);
        vk::destroy(device);
        vk::destroy(instance);
        println("- Terminated Vulkan");

        glfw::driver_t::terminate();
        println("- Terminated GLFW");
    }
    catch (const orb::exception& e)
    {
        println("Fatal error: {}", e.what());
        return 1;
    }

    return 0;
}
