cmake_minimum_required(VERSION 3.10)
project(Collections)
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCES src/vector.c)
add_library(collections SHARED ${SOURCES})
target_include_directories(collections PUBLIC include)
