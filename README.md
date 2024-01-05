# Pico Project

## Requirements

- Brew

## Environment

Shell commands to install the environment:

```bash
brew uninstall --force arm-none-eabi-gcc
brew uninstall --force arm-none-eabi-binutils
brew install gcc-arm-embedded
brew install cmake
```

## Build

```bash
cd your_project
mkdir build
cd build
cmake ..
make -j4
```
