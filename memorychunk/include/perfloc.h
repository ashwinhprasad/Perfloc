#ifndef PERFLOC_H

#define PERFLOC_H

#include "memory_chunk.h"

typedef struct AllocObj {
    size_t size;
    ChildMeta* pmc_meta_list_object;
    void* object_address;
} AllocObj;

#define ALLOC_OBJECT_OBJECT_ADDRESS_OFFSET sizeof(size_t) + sizeof(ChildMeta*)

void* perfalloc(MemoryChunk, size_t);
void perffree(MemoryChunk,void*);
MemoryChunk getPerfMem();
void dropPerfMem(MemoryChunk);

#endif