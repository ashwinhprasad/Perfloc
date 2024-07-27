#include "perfloc.h"
#include "memory_chunk.h"
#include "string.h"

MemoryChunk getPerfMem()
{
    return get_process_memory_chunk();    
}


void dropPerfMem(MemoryChunk pmc)
{
    drop_memory_chunk(pmc);
}

/*
    Every object has an associated alloc object header before it. the void* object here
    represents the actual object and not the header.

    In the deletion operation, we trace back to the corresponding child meta in the process
    memory chunk and remove it.  
*/
void perffree(MemoryChunk mc, void* object)
{
    AllocObjHeader* object_header = (AllocObjHeader*)(object - ALLOC_OBJECT_SIZE);
    ChildMeta* child_meta = object_header->pmc_meta_list_object;

    child_meta->next_child_meta->previous_child_meta = child_meta->previous_child_meta;
    child_meta->previous_child_meta->next_child_meta = child_meta->next_child_meta;

    /*
    TODO: Traverse back to process memory chunks header and reduce the 
    size of the object. We can traverse to the process memory chunk's header through
    2 approaches.

    i) Store a back pointer to pmc's header
    
    ii) Get to the corresponding child object meta a traverse the linked list back
    to the pmc's header :((

    memory vs speed tradeoff. will decide what to do next;
    */
}

void* perfalloc(MemoryChunk pmc, size_t object_size) 
{
    Header* header = pmc.header;
    size_t allocating_object_size = ALLOC_OBJECT_SIZE + object_size;

    if (header->occupied_space + allocating_object_size > header->total_size)
    {
        /*
            Case where process memory chunk size is not enough.
            I need to subtract meta list and header sizes from the
            total size before comparison.
        */
    }

    ChildMeta* current_child_meta = pmc.head_child_meta;
    while (current_child_meta->next_child_meta != NULL)
    {

    }

    void* object_alloc_address = current_child_meta->object_ptr + current_child_meta->size;
    ChildMeta* object_child_meta = current_child_meta->object_ptr + CHILD_META_SIZE; 

    AllocObjHeader alloc_obj = {
        allocating_object_size,
        object_child_meta
    };
    memcpy(object_alloc_address, &alloc_obj, ALLOC_OBJECT_SIZE);

    object_child_meta->previous_child_meta = current_child_meta;
    object_child_meta->next_child_meta = current_child_meta->next_child_meta;
    if (current_child_meta->next_child_meta != NULL)
    {
        current_child_meta->next_child_meta->previous_child_meta = object_child_meta;
    }
    current_child_meta->next_child_meta = object_child_meta;
    object_child_meta->object_ptr = object_alloc_address;
    object_child_meta->is_head = false;
    object_child_meta->size = allocating_object_size;
    return object_alloc_address + ALLOC_OBJECT_SIZE;
}