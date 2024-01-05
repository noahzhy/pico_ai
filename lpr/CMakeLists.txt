cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(test_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_executable(lpr
  lpr.c
  ml_model.cpp
)

target_include_directories(lpr PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Uncomment if you want to flash your board via picotool.
# pico_enable_stdio_usb(lpr 1)
pico_add_extra_outputs(lpr)

target_link_libraries(lpr
  pico_stdlib
  pico_tflmicro
)

add_subdirectory("pico-tflmicro" EXCLUDE_FROM_ALL)
