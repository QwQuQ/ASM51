cmake_minimum_required(VERSION 3.28)
project(asm51)
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

include_directories(inc)

file(GLOB SOURCES "src/*.cpp")

add_executable(asm51 ${SOURCES})

set_target_properties(asm51 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
