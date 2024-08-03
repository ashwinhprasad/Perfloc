#ifndef MEMORY_CHUNK_H

#define MEMORY_CHUNK_H

#include <stdio.h>
#include <stdbool.h>

#define PAGE_SIZE (size_t)4096
#define INITIAL_NUMBER_OF_PAGES_FOR_TOP_CHUNK 8
#define INITIAL_TOP_MEMORY_CHUNK_SIZE (size_t)INITIAL_NUMBER_OF_PAGES_FOR_TOP_CHUNK * PAGE_SIZE
#define INITIAL_NUMBER_OF_PAGES_FOR_PROCESS_MEMORY_CHUNK 1
#define INITIAL_PROCESS_MEMORY_CHUNK_SIZE INITIAL_NUMBER_OF_PAGES_FOR_PROCESS_MEMORY_CHUNK * PAGE_SIZE
#define MEMORY_CHUNK_HEADER_SIZE_FACTOR 0.1 


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
    ChildMeta* parent_memory_chunk_meta_list_object;
} Header;

#define MC_HEADER_SIZE sizeof(Header)


typedef struct MemoryChunk {
    Header* header;
    Header* parent_header;
    ChildMeta* head_child_meta;
    void* allocation_start_location;
    void* memory;
    struct MemoryChunk* associated_next_pmc;
} MemoryChunk;


MemoryChunk get_process_memory_chunk(size_t);
void drop_memory_chunk(MemoryChunk);

#endif