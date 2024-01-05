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

## Convert TF Lite model to C array

```bash
echo "alignas(8) const unsigned char model[] = {" > tflite_model.h
cat tflite_model.tflite | xxd -i >> tflite_model.h
echo "};" >> tflite_model.h
```

## Build

```bash
cd your_project
mkdir build
cd build
cmake ..
make -j4
```
