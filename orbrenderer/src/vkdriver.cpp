#include "orbrenderer/vkdriver.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <orb/assert.hpp>
#include <orb/print.hpp>
#include <orb/utility.hpp>
#include <vector>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

using namespace orb;

namespace
{
    VkAllocationCallbacks*   g_Allocator      = nullptr;
    VkInstance               g_Instance       = VK_NULL_HANDLE;
    VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice                 g_Device         = VK_NULL_HANDLE;
    uint32_t                 g_QueueFamily    = (uint32_t)-1;
    VkQueue                  g_Queue          = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT g_DebugReport    = VK_NULL_HANDLE;
    VkPipelineCache          g_PipelineCache  = VK_NULL_HANDLE;
    VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

    ImGui_ImplVulkanH_Window g_MainWindowData;
    int                      g_MinImageCount    = 2;
    bool                     g_SwapChainRebuild = false;

    void glfw_error_callback(int error, const char* description)
    {
        orb::println("GLFW Error {}: {}", error, description);
    }

    void check_vk_result(VkResult err)
    {
        if (err == 0)
        {
            return;
        }

        panic("Vulkan error: {}", (i64)err);
    }

    VKAPI_ATTR auto VKAPI_CALL debug_report(VkDebugReportFlagsEXT      flags,
                                            VkDebugReportObjectTypeEXT objectType,
                                            uint64_t                   object,
                                            size_t                     location,
                                            int32_t                    messageCode,
                                            const char*                pLayerPrefix,
                                            const char*                pMessage,
                                            void*                      pUserData) -> VkBool32
    {
        (void)flags;
        (void)object;
        (void)location;
        (void)messageCode;
        (void)pUserData;
        (void)pLayerPrefix;
        orb::println("Vulkan error from {}: {}", (i64)objectType, pMessage);
        return VK_FALSE;
    }

    auto is_ext_avail(const std::vector<VkExtensionProperties>& properties, const char* extension) -> bool
    {
        for (const VkExtensionProperties& p : properties)
        {
            if (strcmp(p.extensionName, extension) == 0)
            {
                return true;
            }
        }
        return false;
    }

    auto sel_phy_dev() -> VkPhysicalDevice
    {
        uint32_t gpu_count {};
        VkResult err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, nullptr);
        check_vk_result(err);
        IM_ASSERT(gpu_count > 0);

        std::vector<VkPhysicalDevice> gpus;
        gpus.resize(gpu_count);
        err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus.data());
        check_vk_result(err);

        // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This
        // covers most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated
        // setups (multiple dedicated GPUs) is out of scope of this sample.
        for (VkPhysicalDevice& device : gpus)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                return device;
            }
        }

        // Use first GPU (Integrated) is a Discrete one is not available.
        if (gpu_count > 0)
        {
            return gpus[0];
        }
        return VK_NULL_HANDLE;
    }

    void setup_vk(std::vector<const char*> instance_extensions)
    {
        VkResult err {};

        // Create Vulkan Instance
        {
            VkInstanceCreateInfo create_info = {};
            create_info.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

            // Enumerate available extensions
            uint32_t                           properties_count {};
            std::vector<VkExtensionProperties> properties;
            vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
            properties.resize(properties_count);
            err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.data());
            check_vk_result(err);

            // Enable required extensions
            if (is_ext_avail(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
            {
                instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            }
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
            if (is_ext_avail(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
            {
                instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
                create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
            }
#endif

            // Enabling validation layers
            std::array<const char*, 1> layers = { "VK_LAYER_KHRONOS_validation" };
            create_info.enabledLayerCount     = 1;
            create_info.ppEnabledLayerNames   = layers.data();
            instance_extensions.push_back("VK_EXT_debug_report");

            // Create Vulkan Instance
            create_info.enabledExtensionCount   = (uint32_t)instance_extensions.size();
            create_info.ppEnabledExtensionNames = instance_extensions.data();
            err                                 = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
            check_vk_result(err);

            // Setup the debug report callback
            auto f_vkCreateDebugReportCallbackEXT =
                (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, // NOLINT
                                                                          "vkCreateDebugReportCallbackEXT");
            orbassert(f_vkCreateDebugReportCallbackEXT != nullptr,
                      "Coult not retrieve CreateDebugReportCallback function");
            VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
            debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
            debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT
                                  | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
            debug_report_ci.pfnCallback = debug_report;
            debug_report_ci.pUserData   = nullptr;
            err = f_vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
            check_vk_result(err);
        }

        // Select Physical Device (GPU)
        g_PhysicalDevice = sel_phy_dev();

        // Select graphics queue family
        {
            uint32_t count {};
            vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, nullptr);
            std::vector<VkQueueFamilyProperties> queues(count);
            vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues.data());

            for (uint32_t i = 0; i < count; i++)
            {
                if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    g_QueueFamily = i;
                    break;
                }
            }
            orbassert(g_QueueFamily != (uint32_t)-1, "Could not retrieve queue family propoerties");
        }

        // Create Logical Device (with 1 queue)
        {
            ImVector<const char*> device_extensions;
            device_extensions.push_back("VK_KHR_swapchain");

            // Enumerate physical device extension
            uint32_t                           properties_count {};
            std::vector<VkExtensionProperties> properties;
            vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, nullptr);
            properties.resize(properties_count);
            vkEnumerateDeviceExtensionProperties(g_PhysicalDevice,
                                                 nullptr,
                                                 &properties_count,
                                                 properties.data());
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
            if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
            {
                device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
            }
#endif

            std::array                             queue_priority = { 1.0f };
            std::array<VkDeviceQueueCreateInfo, 1> queue_info {};
            queue_info[0].sType                 = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info[0].queueFamilyIndex      = g_QueueFamily;
            queue_info[0].queueCount            = 1;
            queue_info[0].pQueuePriorities      = queue_priority.data();
            VkDeviceCreateInfo create_info      = {};
            create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            create_info.queueCreateInfoCount    = queue_info.size();
            create_info.pQueueCreateInfos       = queue_info.data();
            create_info.enabledExtensionCount   = (uint32_t)device_extensions.Size;
            create_info.ppEnabledExtensionNames = device_extensions.Data;
            err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
            check_vk_result(err);
            vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
        }

        // Create Descriptor Pool
        // The example only requires a single combined image sampler descriptor for the font image and only
        // uses one descriptor set (for that) If you wish to load e.g. additional textures you may need to
        // alter pools sizes.
        {
            std::array<VkDescriptorPoolSize, 1> pool_sizes = {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
            };
            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.flags                      = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets                    = 1;
            pool_info.poolSizeCount              = (uint32_t)pool_sizes.size();
            pool_info.pPoolSizes                 = pool_sizes.data();
            err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
            check_vk_result(err);
        }
    }

    void setup_vk_win(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
    {
        wd->Surface = surface;

        // Check for WSI support
        VkBool32 res {};
        vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
        if (res != VK_TRUE)
        {
            panic("Error no WSI support on physical device 0");
        }

        // Select Surface Format
        const std::array<VkFormat, 4> requestSurfaceImageFormat = { VK_FORMAT_B8G8R8A8_UNORM,
                                                                    VK_FORMAT_R8G8B8A8_UNORM,
                                                                    VK_FORMAT_B8G8R8_UNORM,
                                                                    VK_FORMAT_R8G8B8_UNORM };

        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice,
                                                                  wd->Surface,
                                                                  requestSurfaceImageFormat.data(),
                                                                  requestSurfaceImageFormat.size(),
                                                                  requestSurfaceColorSpace);

        // Select Present Mode
        const std::array<VkPresentModeKHR, 3> present_modes = { VK_PRESENT_MODE_MAILBOX_KHR,
                                                                VK_PRESENT_MODE_IMMEDIATE_KHR,
                                                                VK_PRESENT_MODE_FIFO_KHR };
        wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice,
                                                              wd->Surface,
                                                              present_modes.data(),
                                                              present_modes.size());
        // printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

        // Create SwapChain, RenderPass, Framebuffer, etc.
        orbassert(g_MinImageCount >= 2, "Swapchain invalid");
        ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance,
                                               g_PhysicalDevice,
                                               g_Device,
                                               wd,
                                               g_QueueFamily,
                                               g_Allocator,
                                               width,
                                               height,
                                               g_MinImageCount);
    }

    void cleanup_vk()
    {
        vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

        // Remove the debug report callback
        auto f_vkDestroyDebugReportCallbackEXT =
            (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, // NOLINT
                                                                       "vkDestroyDebugReportCallbackEXT");
        f_vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);

        vkDestroyDevice(g_Device, g_Allocator);
        vkDestroyInstance(g_Instance, g_Allocator);
    }

    void cleanup_vk_win()
    {
        ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator);
    }

    void frame_render(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
    {
        VkResult err {};

        VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore render_complete_semaphore =
            wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        err = vkAcquireNextImageKHR(g_Device,
                                    wd->Swapchain,
                                    UINT64_MAX,
                                    image_acquired_semaphore,
                                    VK_NULL_HANDLE,
                                    &wd->FrameIndex);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            g_SwapChainRebuild = true;
            return;
        }
        check_vk_result(err);

        ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
        {
            err = vkWaitForFences(g_Device,
                                  1,
                                  &fd->Fence,
                                  VK_TRUE,
                                  UINT64_MAX); // wait indefinitely instead of periodically checking
            check_vk_result(err);

            err = vkResetFences(g_Device, 1, &fd->Fence);
            check_vk_result(err);
        }
        {
            err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
            check_vk_result(err);
            VkCommandBufferBeginInfo info = {};
            info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
            check_vk_result(err);
        }
        {
            VkRenderPassBeginInfo info    = {};
            info.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass               = wd->RenderPass;
            info.framebuffer              = fd->Framebuffer;
            info.renderArea.extent.width  = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount          = 1;
            info.pClearValues             = &wd->ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo         info       = {};
            info.sType                      = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount         = 1;
            info.pWaitSemaphores            = &image_acquired_semaphore;
            info.pWaitDstStageMask          = &wait_stage;
            info.commandBufferCount         = 1;
            info.pCommandBuffers            = &fd->CommandBuffer;
            info.signalSemaphoreCount       = 1;
            info.pSignalSemaphores          = &render_complete_semaphore;

            err = vkEndCommandBuffer(fd->CommandBuffer);
            check_vk_result(err);
            err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
            check_vk_result(err);
        }
    }

    void frame_present(ImGui_ImplVulkanH_Window* wd)
    {
        if (g_SwapChainRebuild)
        {
            return;
        }
        VkSemaphore render_complete_semaphore =
            wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info   = {};
        info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores    = &render_complete_semaphore;
        info.swapchainCount     = 1;
        info.pSwapchains        = &wd->Swapchain;
        info.pImageIndices      = &wd->FrameIndex;
        VkResult err            = vkQueuePresentKHR(g_Queue, &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            g_SwapChainRebuild = true;
            return;
        }
        check_vk_result(err);
        wd->SemaphoreIndex =
            (wd->SemaphoreIndex + 1) % wd->SemaphoreCount; // Now we can use the next set of semaphores
    }
} // namespace

// Main code
namespace orb::vk
{
    auto create_app() -> int
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
        {
            return 1;
        }

        // Create window with Vulkan context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
        if (!glfwVulkanSupported())
        {
            printf("GLFW: Vulkan Not Supported\n");
            return 1;
        }

        std::vector<const char*> extensions;
        uint32_t                 extensions_count = 0;
        const char**             glfw_extensions  = glfwGetRequiredInstanceExtensions(&extensions_count);
        for (uint32_t i = 0; i < extensions_count; i++)
        {
            extensions.push_back(glfw_extensions[i]);
        }
        setup_vk(extensions);

        // Create Window Surface
        VkSurfaceKHR surface {};
        VkResult     err = glfwCreateWindowSurface(g_Instance, window, g_Allocator, &surface);
        check_vk_result(err);

        // Create Framebuffers
        int w {}, h {};
        glfwGetFramebufferSize(window, &w, &h);
        ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
        setup_vk_win(wd, surface, w, h);

        // Setup Dear ImGui context
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
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance                  = g_Instance;
        init_info.PhysicalDevice            = g_PhysicalDevice;
        init_info.Device                    = g_Device;
        init_info.QueueFamily               = g_QueueFamily;
        init_info.Queue                     = g_Queue;
        init_info.PipelineCache             = g_PipelineCache;
        init_info.DescriptorPool            = g_DescriptorPool;
        init_info.RenderPass                = wd->RenderPass;
        init_info.Subpass                   = 0;
        init_info.MinImageCount             = g_MinImageCount;
        init_info.ImageCount                = wd->ImageCount;
        init_info.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator                 = g_Allocator;
        init_info.CheckVkResultFn           = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts
        // and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font
        // among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in
        // your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when
        // calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font
        // rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write
        // a double backslash \\ !
        // io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr,
        // io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != nullptr);

        // Our state
        bool   show_demo_window    = true;
        bool   show_another_window = false;
        ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants
            // to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application,
            // or clear/overwrite your copy of the mouse data.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main
            // application, or clear/overwrite your copy of the keyboard data. Generally you may always pass
            // all inputs to dear imgui, and hide them from your application based on those two flags.
            glfwPollEvents();

            // Resize swap chain?
            int fb_width{}, fb_height{};
            glfwGetFramebufferSize(window, &fb_width, &fb_height);
            if (fb_width > 0 && fb_height > 0
                && (g_SwapChainRebuild || g_MainWindowData.Width != fb_width
                    || g_MainWindowData.Height != fb_height))
            {
                ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance,
                                                       g_PhysicalDevice,
                                                       g_Device,
                                                       &g_MainWindowData,
                                                       g_QueueFamily,
                                                       g_Allocator,
                                                       fb_width,
                                                       fb_height,
                                                       g_MinImageCount);
                g_MainWindowData.FrameIndex = 0;
                g_SwapChainRebuild          = false;
            }
            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            // Start the Dear ImGui frame
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can
            // browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
            {
                ImGui::ShowDemoWindow(&show_demo_window);
            }

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named
            // window.
            {
                static float f       = 0.0f;
                static int   counter = 0;

                ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

                ImGui::Text(
                    "This is some useful text."); // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window",
                                &show_demo_window); // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (f32*)&clear_color.x); // Edit 3 floats representing a color

                if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true
                                             // when edited/activated)
                {
                    counter++;
                }
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / io.Framerate,
                            io.Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin(
                    "Another Window",
                    &show_another_window); // Pass a pointer to our bool variable (the window will have a
                                           // closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                {
                    show_another_window = false;
                }
                ImGui::End();
            }

            // Rendering
            ImGui::Render();
            ImDrawData* draw_data    = ImGui::GetDrawData();
            const bool  is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
            if (!is_minimized)
            {
                wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
                wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
                wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
                wd->ClearValue.color.float32[3] = clear_color.w;
                frame_render(wd, draw_data);
                frame_present(wd);
            }
        }

        // Cleanup
        err = vkDeviceWaitIdle(g_Device);
        check_vk_result(err);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        cleanup_vk_win();
        cleanup_vk();

        glfwDestroyWindow(window);
        glfwTerminate();

        return 0;
    }
} // namespace orb::vk
