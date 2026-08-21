cmake_minimum_required(VERSION 3.10)
project(thesensorproject C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Where to find headers
include_directories(include)

# What to build
add_executable(thesensorproject
    src/main.c
    src/sensors.c
    src/config.c
    src/statemachine.c
    src/ringbuffer.c
    src/logger.c
)

target_link_libraries(thesensorproject m)