# Pico AI

- [Pico AI](#pico-ai)
  - [Introduction](#introduction)
  - [Requirements](#requirements)
    - [SDK](#sdk)
    - [Environment](#environment)
  - [Start with TF-micro on Pico](#start-with-tf-micro-on-pico)
    - [Demo](#demo)
    - [Run your own model](#run-your-own-model)
  - [Build](#build)
  - [Flash](#flash)

## Introduction

This is a project collection for Raspberry Pi Pico with TensorFlow Micro.

## Requirements

### SDK

Download the SDK from [here](https://github.com/raspberrypi/pico-sdk).

Or use bash commands to download the SDK:

```bash
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
```

Set `PICO_SDK_PATH` environment variable to the path of the SDK.

```bash
export PICO_SDK_PATH=/path/to/pico-sdk
```

Check environment variable:

```bash
echo $PICO_SDK_PATH
```

### Environment

Make sure you have the following tools installed:

- Brew

Download and install Brew from [here](https://brew.sh/).

Then use Shell commands to install the environment:

```bash
brew install cmake
brew install flatbuffers
brew tap ArmMbed/homebrew-formulae
brew uninstall --force arm-none-eabi-gcc
brew uninstall --force arm-none-eabi-binutils
brew install gcc-arm-embedded
```

## Start with TF-micro on Pico

### Demo

Choose a demo which you are interested in from the folder first. Then follow the instructions to [Build](#build) the demo.

Now you can find the `demo.uf2` file in the `build` folder. Flash it to your Pico board.

### Run your own model

Prepare your own model first. And make sure it is a TFLite model. Then use the following command to convert it to a C array.

```bash
echo "alignas(8) const unsigned char tflite_model[] = {" > tflite_model.h
cat tflite_model.tflite | xxd -i >> tflite_model.h
echo "};" >> tflite_model.h
```

Copy the pico-sdk cmake file to your project folder.

```bash
cp $PICO_SDK_PATH/external/pico_sdk_import.cmake .
```

Download the `pico-tflmicro` library from [here](https://github.com/raspberrypi/pico-tflmicro.git). Or use the following command to download it.

```bash
cd your_project_folder
mkdir lib
cd lib
git clone https://github.com/raspberrypi/pico-tflmicro.git
```

Then create a `CMakeLists.txt` file in your project folder. And add the following content to it.

```cmake
cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(demo C CXX ASM)
pico_sdk_init()

set(ARM_CPU "cortex-m0plus")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_executable(demo
  # Add your source files here.
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

target_link_libraries(demo
    pico_stdlib
    pico-tflmicro
)

# Uncomment if you want to flash your board via picotool.
# pico_enable_stdio_usb(demo 1)
pico_add_extra_outputs(demo)

add_subdirectory("lib/pico-tflmicro" EXCLUDE_FROM_ALL)
```

Make sure the folder structure is like this:

```bash
-- your_project_folder
    |-- build
    |-- CMakeLists.txt
    |-- pico_sdk_import.cmake
    |-- lib
        |-- pico-tflmicro
    |-- src
        |-- main.c
        ...
```

Now you can [Build](#build) your project. Then you can find the `.uf2` file in the `build` folder. Flash it to your Pico board. Enjoy it!

## Build

```bash
cd demo
mkdir build
cd build
cmake .. -DPICO_BOARD=pico
make -j4
```

## Flash

For Raspberry Pi Pico

1. Plug the USB Micro cable into your PC, but do NOT plug in the Pico side.
2. While holding down the white BOOTSEL button, plug in the micro USB cable to the Pico.
