project(asm51)

include_directories(inc)

file(GLOB SOURCES "src/*.cpp")

add_executable(asm51 ${SOURCES})