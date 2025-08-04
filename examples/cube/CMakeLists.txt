file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(cube_example ${EXAMPLE_SOURCES})
target_link_libraries(cube_example PRIVATE cn3d)
target_compile_options(cube_example PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(cube_example PRIVATE cxx_std_23)
set_target_properties(cube_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/runtime")