#
# AntTweakBar library
#

# Messages
message(STATUS "################################################")
message(STATUS "Checking for AntTweakBar")

# Find
find_package(AntTweakBar REQUIRED)
if (ANTTWEAKBAR_FOUND)
    include_directories(SYSTEM ${ANTTWEAKBAR_INCLUDE_DIR})
    function (ANTTWEAKBAR_LINK TARGET)
        target_link_libraries(${TARGET} ${ANTTWEAKBAR_LIBRARY})
    endfunction ()
endif ()
