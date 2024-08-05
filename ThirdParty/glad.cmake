SET(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glad)

add_library(glad STATIC)

target_sources(glad PRIVATE
        ${GLAD_DIR}/src/glad.c
        )

target_include_directories(glad PUBLIC
        ${GLAD_DIR}/include
        )