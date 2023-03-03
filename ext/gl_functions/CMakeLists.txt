project(gl_functions CXX)

add_library(gl_functions STATIC)
target_include_directories(gl_functions PUBLIC include)

set(GLAD_FUNCTIONS_PUBLIC_HEADERS
        include/functions.hxx
)

set(GLAD_FUNCTIONS_SOURCES
        src/functions.cxx
)

target_sources(gl_functions
        PUBLIC  ${GLAD_FUNCTIONS_PUBLIC_HEADERS}
        PRIVATE ${GLAD_FUNCTIONS_SOURCES}
)