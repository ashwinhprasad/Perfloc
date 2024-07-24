#include <assert.h>
#include "perfloc.h"


int main()
{
    MemoryChunk pmc = getPerfMem();
    int* a = perfalloc(pmc, sizeof(int));
    perffree(pmc, a);
    return 0;
}