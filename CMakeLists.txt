cmake_minimum_required(VERSION 3.16)

project(cmb LANGUAGES C)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_GENERATOR "Ninja")
set(CMAKE_C_COMPILER clang-cl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB SOURCES "src/*.c")
file(GLOB HEADERS "include/*.h")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(external/cJSON)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE cJSON)

target_compile_options(${PROJECT_NAME} PRIVATE
    -fms-extensions
    -fms-compatibility
    $<$<CONFIG:Debug>:/Od>       # Debug: Disable optimizations
    $<$<CONFIG:Release>:/O2>     # Release: Optimize for speed
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
target_link_options(${PROJECT_NAME} PRIVATE
    "/SUBSYSTEM:CONSOLE"
    "/MANIFEST:EMBED"
    "/DEBUG"
)

