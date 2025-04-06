add_executable(quad main.cpp)

target_compile_definitions(quad
  PRIVATE SAMPLE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(quad
  PRIVATE orb::orbrenderer)
