#
# ANTTWEAKBAR_FOUND
# ANTTWEAKBAR_INCLUDE_PATH
# ANTTWEAKBAR_LIBRARY
#

IF (WIN32)
    FIND_PATH(ANTTWEAKBAR_INCLUDE_PATH AntTweakBar.h
        PATHS $ENV{ANTTWEAKBAR_ROOT}/include
        DOC "The directory where AntTweakBar.h resides"
    )

    FIND_LIBRARY(ANTTWEAKBAR_LIBRARY AntTweakBar
        PATHS $ENV{ANTTWEAKBAR_ROOT}/lib
        DOC "The AntTweakBar library"
    )
ELSE (WIN32)
    FIND_PATH(ANTTWEAKBAR_INCLUDE_PATH AntTweakBar.h
        PATHS
            /usr/local/include
            /usr/X11/include
            /usr/include
    )

    FIND_LIBRARY(ANTTWEAKBAR_LIBRARY AntTweakBar
        PATHS
            /usr/local
            /usr/X11
            /usr
        PATH_SUFFIXES
            lib64
            lib
            dylib
    )
ENDIF (WIN32)

SET(ANTTWEAKBAR_FOUND FALSE)
IF (ANTTWEAKBAR_INCLUDE_PATH AND ANTTWEAKBAR_LIBRARY)
    SET(ANTTWEAKBAR_FOUND TRUE)
ENDIF (ANTTWEAKBAR_INCLUDE_PATH AND ANTTWEAKBAR_LIBRARY)

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(AntTweakBar DEFAULT_MSG ANTTWEAKBAR_INCLUDE_PATH ANTTWEAKBAR_LIBRARY)
