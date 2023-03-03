cmake_minimum_required(VERSION 3.14)
project(opengl_example CXX)
add_executable(opengl_example main.cxx)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/glfw)
target_link_libraries(opengl_example glfw)

add_subdirectory(ext/glad)
target_link_libraries(opengl_example glad)

find_package(OpenGL REQUIRED)
target_link_libraries(opengl_example OpenGL::GL)