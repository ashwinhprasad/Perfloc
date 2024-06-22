#ifndef MEMORY_CHUNK_H

#define MEMORY_CHUNK_H

#include <stdio.h>
#include <stdbool.h>
#include "vector.h"

#define PAGE_SIZE 4096
#define INITIAL_NUMBER_OF_PAGES_FOR_TOP_CHUNK 8
#define INITIAL_TOP_MEMORY_CHUNK_SIZE INITIAL_NUMBER_OF_PAGES_FOR_TOP_CHUNK * PAGE_SIZE
#define INITIAL_NUMBER_OF_PAGES_FOR_PROCESS_MEMORY_CHUNK 1
#define INITIAL_PROCESS_MEMORY_CHUNK_SIZE INITIAL_NUMBER_OF_PAGES_FOR_PROCESS_MEMORY_CHUNK * PAGE_SIZE
#define MEMORY_CHUNK_HEADER_SIZE_FACTOR 0.1 

Vector TOP_MEM_CHUNK_VEC;
bool IS_TOP_MEM_CHUNK_VEC_INITIALISED = false;

typedef struct ChildMeta {
    void* object_ptr;
    size_t size;
    bool is_head;
    struct ChildMeta* next_child_meta;
    struct ChildMeta* previous_child_meta;
} ChildMeta;

#define CHILD_META_SIZE sizeof(ChildMeta)


typedef struct Header {
    bool is_top;
    size_t total_size;
    size_t occupied_space;
    void* parent_memory_chunk_meta_list_object;
} Header;

#define MC_HEADER_SIZE sizeof(Header)


typedef struct MemoryChunk {
    Header* header;
    ChildMeta* head_child_meta;
    void* allocation_start_location;
    void* memory;
} MemoryChunk;


MemoryChunk get_process_memory_chunk();
void drop_memory_chunk(MemoryChunk);

#endif