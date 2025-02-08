add_executable(triangle main.cpp)

target_compile_definitions(triangle
  PRIVATE SAMPLE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(triangle
  PRIVATE orb::orbrenderer)
