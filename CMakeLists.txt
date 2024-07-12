cmake_minimum_required(VERSION 3.13)

# Include Pico Functions
include(pico-sdk/pico_sdk_init.cmake)

# GCC Versions
set(CMAKE_C_STANDARD 11)

# Project Settings
project(pico-builtin-rgb)

set(PICO_BOARD "waveshare_rp2040_zero")

# Init SDK
pico_sdk_init()

# Add src
add_subdirectory(./picorgb)