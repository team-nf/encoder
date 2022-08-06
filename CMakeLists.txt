# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

#include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(encoder C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# point out the CMake, where to find the executable source file
add_executable(${PROJECT_NAME}
        src/pico.c
)

# create map/bin/hex/uf2 files.
pico_add_extra_outputs(${PROJECT_NAME})
# Pull in our pico_stdlib which pulls in commonly used features (gpio, timer-delay etc)
target_link_libraries(${PROJECT_NAME}
            pico_stdlib
)
