#ifndef MEMORY_CHUNK_H

#define MEMORY_CHUNK_H

#include <stdio.h>
#include <stdbool.h>

#define PAGE_SIZE 4096
#define INITIAL_NUMBER_OF_PAGES_FOR_TOP_CHUNK 8
#define INITIAL_TOP_MEMORY_CHUNK_SIZE INITIAL_NUMBER_OF_PAGES_FOR_TOP_CHUNK * PAGE_SIZE
#define INITIAL_NUMBER_OF_PAGES_FOR_PROCESS_MEMORY_CHUNK 1
#define INITIAL_PROCESS_MEMORY_CHUNK_SIZE INITIAL_NUMBER_OF_PAGES_FOR_PROCESS_MEMORY_CHUNK * PAGE_SIZE
#define MEMORY_CHUNK_HEADER_SIZE_FACTOR 0.1 

typedef struct ObjectMeta {
    void* start_address;
    size_t size;
    bool is_head;
    struct ObjectMeta* next_object_meta;
    struct ObjectMeta* previous_object_meta;
} ObjectMeta;

#define OBJECT_META_SIZE sizeof(ObjectMeta)


typedef struct Header {
    bool is_top_memory_chunk;
    size_t total_size;
    size_t occupied_space;
    void* parent_memory_chunk_meta_list_object;
} Header;

#define MC_HEADER_SIZE sizeof(Header)


typedef struct MemoryChunk {
    Header* header;
    ObjectMeta* head_object_meta;
    void* allocation_start_location;
    void* memory;
} MemoryChunk;


MemoryChunk get_new_process_memory_chunk();
void* perfalloc(MemoryChunk, size_t);
void perffree(MemoryChunk,void*,size_t);
void drop_memory_chunk(MemoryChunk);

#endif