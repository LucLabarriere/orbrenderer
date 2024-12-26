
add_library(orbrenderer           STATIC src/vk/instance.cpp
                                         src/vk/gpu.cpp
                                         src/vk/device.cpp
                                         src/vk/render.cpp
                                         src/vk/swapchain.cpp
                                         src/vk/render_pass.cpp
                                         src/vk/vma.cpp
                                         src/vk/image.cpp
                                         src/vk/imgui.cpp
                                         src/vk/sync_objects.cpp
                                         src/vk/cmd_pool.cpp
                                         src/vk/framebuffers.cpp
                                         src/glfw/driver.cpp
                                         src/glfw/vk_interoperability.cpp
                                         src/glfw/window.cpp)
add_library(orb::orbrenderer      ALIAS  orbrenderer)

target_link_libraries(
    orbrenderer PUBLIC orb::orblib
                       orb::imgui
                       Vulkan::Vulkan
                       GPUOpen::VulkanMemoryAllocator)

target_include_directories(
    orbrenderer PUBLIC  include
                PRIVATE src)
