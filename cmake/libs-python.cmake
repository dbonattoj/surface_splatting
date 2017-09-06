#
# Python library
#

# Messages
message(STATUS "################################################")
message(STATUS "Checking for Python")

# Find
find_package(PythonLibs REQUIRED)
if (PYTHONLIBS_FOUND)
    include_directories(SYSTEM ${PYTHON_INCLUDE_DIR})
    function (PYTHON_LINK TARGET)
        target_link_libraries(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_LIBRARY})
    endfunction ()
endif ()
