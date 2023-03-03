project(glad C)

add_library(glad STATIC)
target_include_directories(glad PUBLIC include)

set(GLAD_PUBLIC_HEADERS
        include/glad/glad.h
        include/KHR/khrplatform.h
    )
set(GLAD_SOURCES
        src/glad.c
)

target_sources(glad
        PUBLIC  ${GLAD_PUBLIC_HEADERS}
        PRIVATE ${GLAD_SOURCES}
)