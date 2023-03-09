
add_executable(space main.cpp)
target_include_directories(space PUBLIC .)
target_link_libraries(space settings)
target_link_libraries(space lib)

add_custom_target(copy_shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMENT "copying ${CMAKE_CURRENT_SOURCE_DIR}/shaders to ${CMAKE_CURRENT_BINARY_DIR}/shaders"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

add_dependencies(space copy_shaders)