add_executable(shaders_example main.cxx)
target_link_libraries(shaders_example settings)

add_custom_target(shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMENT "copying ${CMAKE_CURRENT_SOURCE_DIR}/shaders to ${CMAKE_CURRENT_BINARY_DIR}/shaders"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

add_dependencies(shaders_example shaders)