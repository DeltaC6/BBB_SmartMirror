cmake_minimum_required(VERSION 3.10)

set(SOURCES mmio.c)

add_library(MMIO ${SOURCES})

target_include_directories(MMIO PUBLIC .)
