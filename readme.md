This is my attempt to create a general-purpose custom memory for high-performance applications

### Basic Usage

```c
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
```

For more extensive documentation, please refer [here](https://ashwinhprasad.github.io/perfloc-docs/)