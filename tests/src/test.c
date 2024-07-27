#include <assert.h>
#include "perfloc.h"


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
    perffree(pmc, type);
    dropPerfMem(pmc);
    return 0;
}