cmake_minimum_required(VERSION 3.10)

project(NOOC)

add_library(
    nooc
    STATIC
    nooc.c
)

target_include_directories(
    nooc
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(
    TARGETS
    nooc
    ARCHIVE DESTINATION lib
)

install(
    FILES
    nooc.h
    DESTINATION
    include
)
