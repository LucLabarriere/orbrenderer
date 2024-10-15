cmake_minimum_required(VERSION 3.26)

project(orbrenderer VERSION 0.0)

find_package(Vulkan REQUIRED)

add_subdirectory(orbrenderer)
add_subdirectory(vendor)

option(ORBRENDERER_BUILD_SAMPLES "Build orbrenderer samples" ON)

if (${ORBRENDERER_BUILD_SAMPLES})
  add_subdirectory(samples)
endif ()
