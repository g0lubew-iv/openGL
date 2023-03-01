cmake_minimum_required(VERSION 3.14)
project(opengl_example CXX)
add_executable(opengl_example main.cxx)

find_package(glfw CONFIG REQUIRED)
target_link_libraries(opengl_example glfw::glfw)