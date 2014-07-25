cmake_minimum_required (VERSION 2.6)
project (Chip8)

include_directories ("${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")

add_executable (chip8
    main.cpp
    chip8.cpp
    cpu.cpp
    graphics.cpp
    memory.cpp
    audio.cpp
)

target_link_libraries (chip8 SDL2)
