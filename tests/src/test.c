#include <assert.h>
#include "perfloc.h"
#include "utils.h"


typedef struct custom_type {
    int a;
    int arr[10];
} custom_type;

int main()
{
    MemoryChunk pmc = getPerfMem();
    custom_type* type = (custom_type*)perfalloc(pmc, sizeof(custom_type));
    type->a = 10;
    type->arr[0] = 1;
    print_memory_chunk_layout(pmc);
    perffree(pmc, type);
    dropPerfMem(pmc);
    return 0;
}