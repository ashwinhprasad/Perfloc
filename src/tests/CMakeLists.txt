cmake_minimum_required(VERSION 3.10)
project(Test)
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCES src/test.c)
add_executable(test ${SOURCES})
# target_include_directories(test PUBLIC include)


find_library(MEMORY_CHUNK_LIB
    NAMES memorychunk
    PATHS ../memorychunk/build
)

find_library(COLLECTIONS_LIB
    NAMES collections
    PATHS ../collections/build
)

target_include_directories(test PUBLIC ../memorychunk/include PUBLIC ../collections/include)
target_link_libraries(test PUBLIC ${MEMORY_CHUNK_LIB} ${COLLECTIONS_LIB})
