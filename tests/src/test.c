#include "memory_chunk.h"
#include <assert.h>
#include "utils.h"


int main()
{
    MemoryChunk mc = get_new_process_memory_chunk();

    print_memory_chunk_layout(mc);

    int* a = (int*)perfalloc(mc, sizeof(int));
    *a = 20;
    
    print_memory_chunk_layout(mc);

    char* b = (char*)perfalloc(mc, sizeof(char));

    print_memory_chunk_layout(mc);

    drop_memory_chunk(mc);
    return 0;
}