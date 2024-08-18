
add_library(orbrenderer      STATIC src/vkdriver.cpp)
add_library(orb::orbrenderer ALIAS  orbrenderer)

target_link_libraries(orbrenderer      PUBLIC  orb::orblib
                                               orb::imgui
                                               Vulkan::Vulkan)
target_include_directories(orbrenderer PUBLIC  include
                                       PRIVATE src)
