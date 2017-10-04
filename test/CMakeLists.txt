# Module
set(TEST_NAME TEST)

# Directories
set(TEST_SOURCE_DIR "${TEST_ROOT}")

# Sources
file(GLOB TEST_SOURCES ${TEST_SOURCE_DIR}/*.cpp  ${VENDORS_INCLUDES}/glad/src/ ${GLVIZ_SOURCE_DIR}/*.cpp ${SURFACE_SPLATTING_SOURCE_DIR}/*.cpp)
file(GLOB TEST_HEADERS ${TEST_SOURCE_DIR}/*.hpp)

# Includes
include_directories(
    ${GLVIZ_INCLUDE_DIR}
	${SURFACE_SPLATTING_INCLUDE_DIR}
	${TEST_HEADERS}
)

message(warning ${TEST_HEADERS})
message(warning ${TEST_SOURCES})

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

include_directories(
	${VENDORS_INCLUDES}/Eigen/
	${VENDORS_INCLUDES}/glad/include/
)
# TESTS MAIN
add_executable(${TEST_NAME} ${TEST_SOURCES})

# Libraries
#EIGEN3_LINK("${TEST_NAME}")
#GLEW_LINK("${TEST_NAME}")
#OPENGL_LINK(${TEST_NAME})

#add_dependencies(${TEST_NAME} glviz surface_splatting)
#target_link_libraries(${TEST_NAME} glviz surface_splatting)
target_link_libraries(${TEST_NAME} ${OPENGL_LIBRARIES})
set_property(TARGET ${TEST_NAME} PROPERTY CXX_STANDARD 11)


#install(TARGETS surface_splatting ${TEST_NAME}
#                LIBRARY DESTINATION lib
#                RUNTIME DESTINATION lib
#                ARCHIVE DESTINATION lib/static)
#install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/surface_splatting/ DESTINATION include)


