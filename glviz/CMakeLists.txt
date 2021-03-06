# Module
set(GLVIZ_NAME glviz)

# Sources
file(GLOB_RECURSE GLVIZ_SOURCES ${GLVIZ_ROOT} *.cpp ${VENDORS_INCLUDES}/glad/src/)
file(GLOB_RECURSE GLVIZ_HEADERS ${GLVIZ_ROOT} *.hpp)
file(GLOB_RECURSE GLVIZ_SHADERS ${GLVIZ_ROOT} *.glsl)

# Includes
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GLVIZ_INCLUDE_DIR}
    ${GLVIZ_SOURCE_DIR}
)

include_directories(
	${VENDORS_INCLUDES}/Eigen/
	${VENDORS_INCLUDES}/glad/include/
)

# Shaders
set(GLVIZ_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/glviz_resources.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/glviz_resources.cpp")
wrap_resources("${GLVIZ_SHADERS}" ${GLVIZ_NAME} ${GLVIZ_RESOURCES})

# Target
add_library(${GLVIZ_NAME} STATIC ${GLVIZ_SOURCES} ${GLVIZ_HEADERS} ${GLVIZ_SHADERS} ${GLVIZ_RESOURCES})
set_property(TARGET ${GLVIZ_NAME} PROPERTY CXX_STANDARD 11)

# Libraries
#EIGEN3_LINK(${GLVIZ_NAME})
#GLEW_LINK(${GLVIZ_NAME})
OPENGL_LINK(${GLVIZ_NAME})
SDL2_LINK(${GLVIZ_NAME})

#install(TARGETS ${CMAKE_CURRENT_BINARY_DIR}/glviz
#                LIBRARY DESTINATION lib
#                RUNTIME DESTINATION lib
#                ARCHIVE DESTINATION lib/static)
#install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/glviz DESTINATION include)