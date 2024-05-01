#include "memory_chunk.h"
#include <assert.h>


int main()
{
    MemoryChunk mc = get_new_process_memory_chunk();
    int* a = (int*)perfalloc(mc, sizeof(int));
    *a = 20;
    drop_memory_chunk(mc);
    assert(*a == 20);
    return 0;
}