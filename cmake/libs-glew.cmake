#
# GLEW library
#

# Messages
message(STATUS "################################################")
message(STATUS "Checking for GLEW")

# Find
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})
    function (GLEW_LINK TARGET)
        target_link_libraries(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/${GLEW_LIBRARIES})
    endfunction ()
endif ()
