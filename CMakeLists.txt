cmake_minimum_required(VERSION 3.26)

project(orbrenderer VERSION 0.0)

find_package(Vulkan REQUIRED)

add_subdirectory(orbrenderer)
add_subdirectory(vendor)
add_subdirectory(sandbox)
