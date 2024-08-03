#include "memory_chunk.h"
#include "utils.h"


/*
    Prints the details regarding the memory chunk (For debugging purposes).

    1. Header details
        - is top
        - total size
        - occupied space

    2. Allocation start address
    

    3. Each child's address and size

    4. Total number of objects or process memory chunks present
*/
void print_memory_chunk_layout(MemoryChunk mc)
{
    Header* header = mc.header;
    printf("Header details\n");
    printf("_________________\n");
    printf("is_top: %d\ntotal_size: %zu\noccupied_space: %zu\n",
        header->is_top, header->total_size, header->occupied_space);
    printf("__________________\n");
    printf("allocation_start_address: %p\n",mc.allocation_start_location);

    int number_of_children = 0;
    ChildMeta* current_node = mc.head_child_meta;
    while (current_node->next_child_meta != NULL) 
    {
        ChildMeta* node = current_node->next_child_meta;
        void* child_alloc_address = node->object_ptr;
        size_t child_size = node->size;
        printf("child_no: %d\nchild_address: %p\nchild_size: %zu\n\n", 
            number_of_children, child_alloc_address, child_size);
        number_of_children++;
        current_node = node;
    }
    printf("__________________\n");
    printf("number_of_children: %d\n", number_of_children);
}