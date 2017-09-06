#
# OpenGL library
#

# Messages
message(STATUS "################################################")
message(STATUS "Checking for OpenGL")

# Find
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
    function (OPENGL_LINK TARGET)
        target_link_libraries(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/${OPENGL_LIBRARIES})
    endfunction ()
endif ()
