#ifndef TOP_MEMORY_CHUNK_H

#define TOP_MEMORY_CHUNK_H

#include "vector.h"
#include "memory_chunk.h"

extern Vector TOP_MEM_CHUNK_VEC;
extern bool IS_TOP_MEM_CHUNK_VEC_INITIALISED;
extern size_t CURRENT_TOP_MEMORY_CHUNK_SIZE;

MemoryChunk allocate_pmc_and_return(size_t);
MemoryChunk* get_initial_top_memory_chunk();
MemoryChunk* get_top_memory_chunk(size_t);

#endif