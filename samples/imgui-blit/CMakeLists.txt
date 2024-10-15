add_executable(imgui-blit main.cpp)

target_link_libraries(imgui-blit
  PRIVATE orb::orbrenderer)
