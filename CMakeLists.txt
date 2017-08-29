# CMake
cmake_minimum_required(VERSION 3.1)

set(LIBRARY_NAME "phongSplatting")
set(PACKAGE_VERSION "1.0")
project (${LIBRARY_NAME} C CXX)

# Compile flags:
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    #vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

# force static runtime libraries for msvc builds
if(MSVC)
  set(variables 
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
	if(${variable} MATCHES "/MD")
	  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	endif()
  endforeach()
endif()

# Paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(GLVIZ_ROOT "${CMAKE_SOURCE_DIR}/glviz")
set(GLVIZ_INCLUDE_DIR "${GLVIZ_ROOT}/include")
set(GLVIZ_SOURCE_DIR "${GLVIZ_ROOT}/src")
set(SURFACE_SPLATTING_ROOT "${CMAKE_SOURCE_DIR}/surface_splatting")

# Tools
include(tools-compilers)
include(tools-resources)

# Libraries
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
