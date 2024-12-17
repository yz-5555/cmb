# cmb: CMake for Babies
todo: add cool badges here
## What is cmb?
`cmb` is a simple CLI tool for project management and build automation using CMake. It makes CMake's difficult and repetitive commands or settings a little easier. Written in pure C.
Technically it's coded to work cross-platform, but not tested and CMakeLists.txt need to be fixed for it. Currently works and tested on Windows.
## Features
`cmb` is was designed with reference to [Cargo](https://github.com/rust-lang/cargo) and [CMake Kits](https://vector-of-bool.github.io/docs/vscode-cmake-tools/kits.html).
### Initialize (Generate build files)
**CMake**
**cmb**
```bash
cmb init
```
```bash
cmake -S . -B target\Debug -DCMAKE_BUILD_TYPE=Debug
```
### Build
**cmb**
```bash
cmb build
```
**CMake**
```bash
cmake --build target\Debug
```
**Cargo**
```bash
cargo build --debug
```
### Run
**cmb**
```bash
cmb run
```
**Cargo**
```bash
cargo run --debug
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
```json
[
    {
        "name": "foo", (required)
        "compilers": { (required for the language you use)
            "C": "clang-cl",
            "CXX": "clang-cl"
        },
        "generator": "Ninja", (optional)
        "source_dir": ".", (optional, default=".")
        "build_dir": "target", (optional, default="target")
        "build_target": "Debug", (optional, default="Debug")
        "run": "myprogram.exe", (required)
        "generate_cmd": [], (optional)
        "build_cmd": [], (optional)
        "run_cmd": [] (optional)
    }
]
```
### Options & Parameters
You can specify which preset you will use.
```bash
cmb build foo
cmb build      (The first preset in the array will be chosen)
```
You can add custom parameters.
cmb_preset.json
```json
"generate_cmd": [ "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON" ]
```
```bash
cmb init                                        (cmb will detect generate_cmd from cmd_presets.json and put them as parameters)
cmb init -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```
## Todo
- [ ] Error when both C and CXX compilers are empty.
- [ ] More features (test, install, ...)
