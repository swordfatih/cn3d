file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(bgfx_example ${EXAMPLE_SOURCES})
target_link_libraries(bgfx_example PRIVATE cn3d)
target_compile_options(bgfx_example PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(bgfx_example PRIVATE cxx_std_23)
set_target_properties(bgfx_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/runtime")