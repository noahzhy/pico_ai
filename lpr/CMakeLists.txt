cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(lpr C CXX ASM)
pico_sdk_init()

set(ARM_CPU "cortex-m0plus")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_executable(lpr
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/ml_model.cpp
)

target_link_libraries(lpr
    pico_stdlib
    pico-tflmicro
)

# Uncomment if you want to flash your board via picotool.
# pico_enable_stdio_usb(lpr 1)
pico_add_extra_outputs(lpr)

add_subdirectory("lib/pico-tflmicro" EXCLUDE_FROM_ALL)
