#ifndef PERFLOC_H

#define PERFLOC_H

#include "memory_chunk.h"

typedef struct AllocObjHeader {
    size_t size;
    Header* pmc_header;
    ChildMeta* pmc_meta_list_object;
} AllocObjHeader;

#define ALLOC_OBJECT_OBJECT_ADDRESS_OFFSET sizeof(size_t) + sizeof(ChildMeta*)
#define ALLOC_OBJECT_SIZE sizeof(AllocObjHeader)

void* perfalloc(MemoryChunk, size_t);
void perffree(MemoryChunk,void*);
MemoryChunk getPerfMem();
void dropPerfMem(MemoryChunk);

#endif