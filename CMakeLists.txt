# CMake
cmake_minimum_required(VERSION 3.1)

# Paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(GLVIZ_ROOT "${CMAKE_SOURCE_DIR}/glviz")
set(GLVIZ_INCLUDE_DIR "${GLVIZ_ROOT}/include")
set(GLVIZ_SOURCE_DIR "${GLVIZ_ROOT}/src")
set(SURFACE_SPLATTING_ROOT "${CMAKE_SOURCE_DIR}/surface_splatting")

# Project
project(surface_splatting)

# Tools
include(tools-compilers)
include(tools-resources)

# Libraries
include(libs-anttweakbar)
include(libs-eigen3)
include(libs-glew)
include(libs-opengl)
include(libs-sdl2)

# Put all executables and libraries into a common directory
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH    "${PROJECT_BINARY_DIR}/bin")

# Source
add_subdirectory(glviz)
add_subdirectory(surface_splatting)
