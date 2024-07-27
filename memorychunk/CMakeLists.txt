cmake_minimum_required(VERSION 3.10)
project(memorychunk)
set(CMAKE_C_STANDARD 17)
set(SOURCES src/top_memory_chunk.c src/utils.c src/process_memory_chunk.c src/perfloc.c)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_library(memorychunk SHARED ${SOURCES})
target_include_directories(memorychunk PUBLIC include)
target_include_directories(memorychunk PRIVATE ../collections/include)
find_library(
    COLLECTIONS_LIB
    NAMES collections
    PATHS ../collections/build
)
target_link_libraries(memorychunk PUBLIC ${COLLECTIONS_LIB})
 