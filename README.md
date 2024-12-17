# cmb: CMake for Babies
todo: add cool badges here
## What is cmb?
`cmb` is a simple CLI tool for project management and build automation using CMake. It makes CMake's difficult and repetitive commands or settings a little easier. Written in pure C.
Technically it's coded to work cross-platform, but not tested and CMakeLists.txt need to be fixed for it. Currently works and tested on Windows.
## Features
`cmb` is was designed with reference to [Cargo](https://github.com/rust-lang/cargo) and [CMake Kits](https://vector-of-bool.github.io/docs/vscode-cmake-tools/kits.html).
### Initialize (Generate build files)
**cmb**
```bash
cmb init [preset_name]
```
**CMake**
```bash
cmake -S . -B build
```
## Setup
### Required dependencies
- [CMake >= 3.16](https://cmake.org/)
- [C/C++ compiler that CMake supports](https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html#supported-compilers)
### Installation
- Download .exe from [Releases](https://google.com)
- Build manually. The project contains CMakeLists.txt.
## Usage
- All you need is `cmb_presets.json`, or you can make a project using [cmb_template](https://github.com/yz-5555/cmb_template).
### cmb_presets.json
