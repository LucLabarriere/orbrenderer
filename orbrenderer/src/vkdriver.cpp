#include "orbrenderer/vkdriver.hpp"

#include "vkdriver_details.hpp"

// Main code
namespace orb::vk
{
    using namespace details;

    driver::~driver() = default;

    auto driver::create() -> result<driver>
    {
        box<handles_t> handles = make_box<handles_t>();

        // Initialize GLFW
        if (auto res = init_glfw(); !res) return res.error();

        // Create window with Vulkan context
        if (auto res = create_window(); !res) return res.error();
        else handles->window = res.value();

        // Check if Vulkan is supported
        if (!vk_supported()) return error_t { "Vulkan not supported" };

        // Setting up Vulkan instance
        auto extensions = get_req_instance_exts();
        setup_vk(*handles, extensions);

        // Create Window Surface
        if (auto res = create_window_surface(*handles); !res) return res.error();
        else handles->surface = res.value();

        // Create Framebuffers
        setup_vk_win(*handles);

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
        ImGui_ImplGlfw_InitForVulkan(handles->window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance                  = handles->instance;
        init_info.PhysicalDevice            = handles->phy_dev;
        init_info.Device                    = handles->dev;
        init_info.QueueFamily               = handles->queue_fam;
        init_info.Queue                     = handles->queue;
        init_info.PipelineCache             = handles->pipeline_cache;
        init_info.DescriptorPool            = handles->desc_pool;
        init_info.RenderPass                = handles->main_win_data.RenderPass;
        init_info.Subpass                   = 0;
        init_info.MinImageCount             = handles->min_img_count;
        init_info.ImageCount                = handles->main_win_data.ImageCount;
        init_info.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator                 = handles->allocator;
        init_info.CheckVkResultFn           = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info);

        return driver { std::move(handles) };
    }

    void driver::loop()
    {
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

        ImGuiIO& io = ImGui::GetIO();

        // Main loop
        while (!glfwWindowShouldClose(m_handles->window))
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
            int fb_width {}, fb_height {};
            glfwGetFramebufferSize(m_handles->window, &fb_width, &fb_height);
            if (fb_width > 0 && fb_height > 0
                && (m_handles->sc_rebuild || m_handles->main_win_data.Width != fb_width
                    || m_handles->main_win_data.Height != fb_height))
            {
                ImGui_ImplVulkan_SetMinImageCount(m_handles->min_img_count);
                ImGui_ImplVulkanH_CreateOrResizeWindow(m_handles->instance,
                                                       m_handles->phy_dev,
                                                       m_handles->dev,
                                                       &m_handles->main_win_data,
                                                       m_handles->queue_fam,
                                                       m_handles->allocator,
                                                       fb_width,
                                                       fb_height,
                                                       m_handles->min_img_count);
                m_handles->main_win_data.FrameIndex = 0;
                m_handles->sc_rebuild               = false;
            }
            if (glfwGetWindowAttrib(m_handles->window, GLFW_ICONIFIED) != 0)
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
                m_handles->main_win_data.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
                m_handles->main_win_data.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
                m_handles->main_win_data.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
                m_handles->main_win_data.ClearValue.color.float32[3] = clear_color.w;
                frame_render(*m_handles, draw_data);
                frame_present(*m_handles);
            }
        }
    }

    auto driver::terminate() -> result<void>
    {
        // Cleanup
        auto err = vkDeviceWaitIdle(m_handles->dev);
        check_vk_result(err);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        cleanup_vk_win(*m_handles);
        cleanup_vk(*m_handles);

        glfwDestroyWindow(m_handles->window);
        glfwTerminate();
        return {};
    }

    driver::driver(box<handles_t> h) : m_handles(std::move(h))
    {
    }

} // namespace orb::vk
