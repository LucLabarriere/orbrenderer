include(FetchContent)

# orblib
add_subdirectory(orblib SYSTEM)

# GLFW
add_subdirectory(glfw   SYSTEM)

# VMA
add_subdirectory(VulkanMemoryAllocator SYSTEM)

# imgui
add_library(orb_imgui
  STATIC  imgui/imgui_widgets.cpp
          imgui/imgui_tables.cpp
          imgui/imgui_demo.cpp
          imgui/imgui_draw.cpp
          imgui/imgui.cpp
          imgui/backends/imgui_impl_vulkan.cpp
          imgui/backends/imgui_impl_glfw.cpp
          imgui/misc/cpp/imgui_stdlib.cpp)
add_library(orb::imgui
  ALIAS   orb_imgui)

target_include_directories(orb_imgui
  PUBLIC  imgui
          imgui/backends
          imgui/misc/cpp/)
target_link_libraries(orb_imgui
  PUBLIC  Vulkan::Vulkan
          glfw)
