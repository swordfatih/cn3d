file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(vulkan_example ${EXAMPLE_SOURCES})
target_link_libraries(vulkan_example PRIVATE cn3d)
target_compile_options(vulkan_example PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(vulkan_example PRIVATE cxx_std_23)
set_target_properties(vulkan_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/runtime")