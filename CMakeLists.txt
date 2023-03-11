cmake_minimum_required(VERSION 3.14)
project(opengl_examples CXX)

set(CMAKE_BUILD_TYPE Debug)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_library(settings INTERFACE)
target_compile_features(settings INTERFACE cxx_std_20)

find_package(glfw3 REQUIRED)
target_link_libraries(settings INTERFACE glfw)

find_package(glad REQUIRED)
target_link_libraries(settings INTERFACE glad::glad)

find_package(OpenGL REQUIRED)
target_link_libraries(settings INTERFACE OpenGL::GL)

find_package(glm REQUIRED)
target_link_libraries(settings INTERFACE glm::glm)

add_subdirectory(lib)
add_subdirectory(space)

add_subdirectory(helloTriangle)
add_subdirectory(shaders)
add_subdirectory(rectangle)
add_subdirectory(reg_polygon)
add_subdirectory(cube)
add_subdirectory(snake)