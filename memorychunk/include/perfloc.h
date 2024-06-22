#ifndef PERFLOC_H

#define PERFLOC_H

#include "memory_chunk.h"

void* perfalloc(MemoryChunk, size_t);
void perffree(MemoryChunk,void*,size_t);

#endif