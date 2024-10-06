
add_library(orbrenderer      STATIC src/vk/instance.cpp
                                    src/vk/gpu.cpp
                                    src/vk/device.cpp
                                    src/vk/swapchain.cpp
                                    src/glfw/driver.cpp
                                    src/glfw/vk_interoperability.cpp
                                    src/glfw/window.cpp)
add_library(orb::orbrenderer ALIAS  orbrenderer)

target_link_libraries(orbrenderer      PUBLIC  orb::orblib
                                               orb::imgui
                                               Vulkan::Vulkan)
target_include_directories(orbrenderer PUBLIC  include
                                       PRIVATE src)
