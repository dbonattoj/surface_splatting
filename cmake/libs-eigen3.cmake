#
# Eigen3 library
#

# Messages
message(STATUS "################################################")
message(STATUS "Checking for Eigen3")

# Find
find_package(Eigen3 3.0.0 REQUIRED)
if (EIGEN3_FOUND)
    include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})
    add_definitions(-DEIGEN_USE_NEW_STDVECTOR -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET)
    function (EIGEN3_LINK TARGET)

    endfunction ()
endif ()
