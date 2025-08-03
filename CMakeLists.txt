cmake_minimum_required(VERSION 3.28)
project(cn3d LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CN3D_WARNING_FLAGS -Wall -Wextra -Wpedantic -Werror)

find_package(Vulkan REQUIRED)

# Static linking setup
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

# GLFW as static
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# TinyGLTF options
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE BOOL "" FORCE)
set(TINYGLTF_BUILD_VALIDATOR OFF CACHE BOOL "" FORCE)

# Fetch dependencies
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
FetchContent_Declare(glfw      GIT_REPOSITORY https://github.com/glfw/glfw.git      GIT_TAG latest)
FetchContent_Declare(glm       GIT_REPOSITORY https://github.com/g-truc/glm.git     GIT_TAG 1.0.1)
FetchContent_Declare(tinygltf  GIT_REPOSITORY https://github.com/syoyo/tinygltf.git GIT_TAG release)
FetchContent_Declare(stb       GIT_REPOSITORY https://github.com/nothings/stb.git   GIT_TAG master)
FetchContent_MakeAvailable(glfw glm tinygltf stb)

# Sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
set(PUBLIC_INCLUDE "${CMAKE_SOURCE_DIR}/include")

# Executable
add_executable(cn3d ${SOURCES})
target_include_directories(cn3d PRIVATE ${PUBLIC_INCLUDE} ${glm_SOURCE_DIR} ${stb_SOURCE_DIR})
target_compile_options(cn3d PRIVATE ${CN3D_WARNING_FLAGS})
target_link_libraries(cn3d PRIVATE Vulkan::Vulkan glfw tinygltf)

# ---- Examples ----
option(CN3D_BUILD_EXAMPLES "Build cn3d examples" ON)

if(CN3D_BUILD_EXAMPLES)
    file(GLOB EXAMPLE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/examples/*/main.cpp")

    foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} DIRECTORY)
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_NAME} NAME)

        add_executable(${EXAMPLE_NAME}_example ${EXAMPLE_FILE})
        target_include_directories(${EXAMPLE_NAME}_example PRIVATE ${PUBLIC_INCLUDE} ${glm_SOURCE_DIR} ${stb_SOURCE_DIR})
        target_link_libraries(${EXAMPLE_NAME}_example PRIVATE Vulkan::Vulkan glfw tinygltf)
        target_compile_options(${EXAMPLE_NAME}_example PRIVATE ${CN3D_WARNING_FLAGS})
        target_compile_features(${EXAMPLE_NAME}_example PRIVATE cxx_std_23)
    endforeach()
endif()