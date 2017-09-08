# - Config file for the surface_splatting package
# It defines the following variables
#  SURFACE_SPLATTING_INCLUDE_DIRS - include directories for FooBar
#  SURFACE_SPLATTING_LIBRARIES    - libraries to link against
 
# Compute paths
get_filename_component(SURFACE_SPLATTING_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SURFACE_SPLATTING_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include/")
 
 
# These are IMPORTED targets created by surface_splattingTargets.cmake
set(SURFACE_SPLATTING_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/lib/surface_splatting)