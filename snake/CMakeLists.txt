add_executable(snake main.cxx)
target_link_libraries(snake settings)

target_include_directories(snake PUBLIC include)

set(SNAKE_PUBLIC_HEADERS
        include/glfw_system.hxx
        include/window.hxx
)

set(SNAKE_SOURCES
        src/glfw_system.cxx
        src/window.cxx
)

target_sources(snake
        PUBLIC  ${SNAKE_PUBLIC_HEADERS}
        PRIVATE ${SNAKE_SOURCES}
)

#add_custom_target(shaders
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
#    COMMENT "copying ${CMAKE_CURRENT_SOURCE_DIR}/shaders to ${CMAKE_CURRENT_BINARY_DIR}/shaders"
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#    )
#
#add_dependencies(snake shaders)