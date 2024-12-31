add_executable(imgui-single-pass main.cpp)

target_link_libraries(imgui-single-pass
  PRIVATE orb::orbrenderer)
