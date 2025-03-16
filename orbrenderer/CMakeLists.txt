
add_library(orbrenderer
  STATIC  src/vk/device.cpp
          src/vk/gpu.cpp
          src/vk/images.cpp
          src/vk/imgui.cpp
          src/vk/instance.cpp
          src/vk/swapchain.cpp
          src/vk/surface.cpp
          src/vk/vma.cpp
          src/glfw/driver.cpp
          src/glfw/window.cpp)

add_library(orb::orbrenderer
  ALIAS   orbrenderer)

target_link_libraries(orbrenderer
  PUBLIC  orb::orblib
          orb::imgui
          Vulkan::Vulkan
          Vulkan::shaderc_combined
          GPUOpen::VulkanMemoryAllocator)

target_include_directories(orbrenderer
  PUBLIC  include
  PRIVATE src)
