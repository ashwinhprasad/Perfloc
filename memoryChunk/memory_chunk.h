#ifndef MEMORY_CHUNK_H

#define MEMORY_CHUNK_H

#include <stdio.h>
#include "../collections/vector.h"
#include <stdbool.h>

#define PAGE_SIZE 4096
#define INITIAL_NUMBER_OF_PAGES_FOR_ROOT_CHUNK 4
#define INITIAL_ROOT_MEMORY_CHUNK_SIZE INITIAL_NUMBER_OF_PAGES_FOR_ROOT_CHUNK * PAGE_SIZE


typedef struct ObjectMeta {
    void* start_address;
    size_t size;
    bool is_root;
    ObjectMeta* next_object_meta;
} ObjectMeta;

#define OBJECT_META_SIZE sizeof(ObjectMeta)


typedef struct Header {
    bool is_root;
    size_t mc_size;
    size_t occupied_space;
} Header;

#define MC_HEADER_SIZE sizeof(Header)


typedef struct MemoryChunk {
    void* memory;
} MemoryChunk;


MemoryChunk get_New_Process_Memory_Chunk();


#endif