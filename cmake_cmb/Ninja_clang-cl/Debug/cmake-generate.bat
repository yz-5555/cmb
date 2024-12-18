@echo off
echo Setting up build environment for D:\devs\projects\cmb
if not exist "D:\devs\projects\cmb\target\Debug" (
    mkdir "D:\devs\projects\cmb\target\Debug"
)
cd /d D:\devs\projects\cmb\target\Debug
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-cl -DCMAKE_EXPORT_COMPILE_COMMANDS=1 D:\devs\projects\cmb
pause
