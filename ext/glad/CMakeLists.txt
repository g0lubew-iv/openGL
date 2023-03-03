project(glad C)

add_library(${PROJECT_NAME} STATIC)
target_include_directories(${PROJECT_NAME} PUBLIC include)

set(GLAD_PUBLIC_HEADERS
        include/glad/glad.h
        include/KHR/khrplatform.h
)

set(GLAD_SOURCES
        src/glad.c
)

target_sources(${PROJECT_NAME}
        PUBLIC  ${GLAD_PUBLIC_HEADERS}
        PRIVATE ${GLAD_SOURCES}
)