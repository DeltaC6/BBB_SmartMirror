cmake_minimum_required(VERSION 3.10)

project(
    SmartMirror
    VERSION 1.0
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(lib)

add_subdirectory(core)
