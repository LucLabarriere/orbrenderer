add_executable(descriptor-sets main.cpp)

target_compile_definitions(descriptor-sets
  PRIVATE SAMPLE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(descriptor-sets
  PRIVATE orb::orbrenderer)
