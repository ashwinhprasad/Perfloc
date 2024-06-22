#ifndef TOP_MEMORY_CHUNK_H

#define TOP_MEMORY_CHUNK_H

#include "memory_chunk.h"

MemoryChunk allocate_pmc_and_return();
MemoryChunk* get_initial_top_memory_chunk();

#endif