cmake_minimum_required(VERSION 3.14)
project(opengl_examples CXX)

set(CMAKE_BUILD_TYPE Debug)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_library(settings INTERFACE)
target_compile_features(settings INTERFACE cxx_std_20)

add_subdirectory(ext/glfw)
target_link_libraries(settings INTERFACE glfw)

add_subdirectory(ext/glad)
target_link_libraries(settings INTERFACE glad)

add_subdirectory(ext/glm)
target_link_libraries(settings INTERFACE glm::glm)

find_package(OpenGL REQUIRED)
target_link_libraries(settings INTERFACE OpenGL::GL)

add_subdirectory(lib)
add_subdirectory(space)

add_subdirectory(helloTriangle)
add_subdirectory(shaders)
add_subdirectory(rectangle)
add_subdirectory(cube)