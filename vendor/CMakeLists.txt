include(FetchContent)

# orblib
message(STATUS "Getting orblib")
FetchContent_Declare(
    orblib
    GIT_REPOSITORY https://github.com/LucLabarriere/orblib
    GIT_TAG        main
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(orblib)

# GLFW
add_subdirectory(glfw   SYSTEM)

# VMA
add_subdirectory(VulkanMemoryAllocator SYSTEM)

# imgui
add_library(orbimgui STATIC  imgui/imgui_widgets.cpp
                             imgui/imgui_tables.cpp
                             imgui/imgui_demo.cpp
                             imgui/imgui_draw.cpp
                             imgui/imgui.cpp
                             imgui/backends/imgui_impl_vulkan.cpp
                             imgui/backends/imgui_impl_glfw.cpp
                             imgui/misc/cpp/imgui_stdlib.cpp)
add_library(orb::imgui ALIAS orbimgui)

target_include_directories(orbimgui PUBLIC imgui
                                           imgui/backends
                                           imgui/misc/cpp/)
target_link_libraries(orbimgui PUBLIC Vulkan::Vulkan
                                      glfw)
