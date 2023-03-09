
add_library(lib STATIC)
target_link_libraries(lib PUBLIC settings)

target_include_directories(lib PUBLIC .)

target_sources(lib
    PUBLIC
    camera.hpp
    model.hpp
    PRIVATE
    camera.cpp
    model.cpp
    )