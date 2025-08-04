cmake_minimum_required(VERSION 3.28)
project(cn3d LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CN3D_WARNING_FLAGS -Wall -Wextra -Wpedantic -Werror)

find_package(Vulkan REQUIRED)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

# Third-party library options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE BOOL "" FORCE)
set(TINYGLTF_BUILD_VALIDATOR OFF CACHE BOOL "" FORCE)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
FetchContent_Declare(sfml      GIT_REPOSITORY https://github.com/SFML/SFML.git      GIT_TAG master)
FetchContent_Declare(glfw      GIT_REPOSITORY https://github.com/glfw/glfw.git      GIT_TAG latest)
FetchContent_Declare(glm       GIT_REPOSITORY https://github.com/g-truc/glm.git     GIT_TAG 1.0.1)
FetchContent_Declare(tinygltf  GIT_REPOSITORY https://github.com/syoyo/tinygltf.git GIT_TAG release)
FetchContent_Declare(stb       GIT_REPOSITORY https://github.com/nothings/stb.git   GIT_TAG master)
FetchContent_MakeAvailable(sfml glfw glm tinygltf stb)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/bgfx.cmake)

# Core cn3d library
file(GLOB_RECURSE CN3D_SOURCES CONFIGURE_DEPENDS src/*.cpp)
set(PUBLIC_INCLUDE "${CMAKE_SOURCE_DIR}/include")

add_library(cn3d STATIC ${CN3D_SOURCES})
target_include_directories(cn3d PUBLIC ${PUBLIC_INCLUDE} ${glm_SOURCE_DIR} ${stb_SOURCE_DIR} ${bx_SOURCE_DIR}/include)
target_compile_options(cn3d PRIVATE ${CN3D_WARNING_FLAGS})
target_compile_features(cn3d PUBLIC cxx_std_23)
target_link_libraries(cn3d PUBLIC Vulkan::Vulkan glfw tinygltf bgfx SFML::Window SFML::System)

option(CN3D_BUILD_EXAMPLES "Build cn3d examples" ON)

if(CN3D_BUILD_EXAMPLES)
    file(GLOB EXAMPLE_DIRS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/examples/*")
    foreach(EXAMPLE_DIR ${EXAMPLE_DIRS})
        if(IS_DIRECTORY ${EXAMPLE_DIR})
            get_filename_component(EXAMPLE_NAME ${EXAMPLE_DIR} NAME)
            add_subdirectory(${EXAMPLE_DIR} examples/${EXAMPLE_NAME})
        endif()
    endforeach()
endif()