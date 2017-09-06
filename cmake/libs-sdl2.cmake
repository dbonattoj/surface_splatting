#
# SDL2 library
#

# Messages
message(STATUS "################################################")
message(STATUS "Checking for SDL2")

# Find
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
    function (SDL2_LINK TARGET)
        target_link_libraries(${TARGET} ${SDL2_LIBRARY})
    endfunction ()
endif ()