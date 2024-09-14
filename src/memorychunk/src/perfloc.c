#include "perfloc.h"
#include "memory_chunk.h"
#include "string.h"
#include "stdlib.h"

MemoryChunk getPerfMem()
{
    return get_process_memory_chunk(INITIAL_PROCESS_MEMORY_CHUNK_SIZE);    
}


void dropPerfMem(MemoryChunk pmc)
{
    drop_memory_chunk(pmc);
}

/*
    Every object has an associated alloc object header before it. the void* object here
    represents the actual object and not the header.

    In the deletion operation, we trace back to the corresponding child meta in the 
    process memory chunk and remove it.  
*/
void perffree(MemoryChunk mc, void* object)
{
    AllocObjHeader* object_header = (AllocObjHeader*)(object - ALLOC_OBJECT_SIZE);
    ChildMeta* child_meta = object_header->pmc_meta_list_object;

    if (child_meta->next_child_meta != NULL)
    {
        /*
        This is the lastly allocated object in the process memory chunk
        */
        child_meta->next_child_meta->previous_child_meta = child_meta->previous_child_meta;
        child_meta->previous_child_meta->next_child_meta = child_meta->next_child_meta;
    }
    else 
    {
        child_meta->previous_child_meta->next_child_meta = NULL;
    }
    object_header->pmc_header->occupied_space -= object_header->size;
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
        MemoryChunk associated_pmc = get_process_memory_chunk(pmc.header->total_size * 2);
        MemoryChunk*  pmc_heap = (MemoryChunk*) malloc(sizeof(MemoryChunk));
        memcpy(pmc_heap, &associated_pmc, sizeof(MemoryChunk));
        pmc.associated_next_pmc = pmc_heap;
        return perfalloc(*pmc_heap, object_size);
    }

    ChildMeta* current_child_meta = pmc.head_child_meta;
    while (current_child_meta->next_child_meta != NULL)
    {
        
        //IS SPACE AVAILABLE FOR THE CHILD META
        bool child_meta_space_available = ((void*)current_child_meta) + (2 * CHILD_META_SIZE) < current_child_meta->next_child_meta;

        //IS SPACE AVAILABLE FOR THE OBJECT ITSELF
        bool object_allocation_space_available = current_child_meta->object_ptr + current_child_meta->size + allocating_object_size < current_child_meta->next_child_meta->object_ptr; 
        
        if (child_meta_space_available && object_allocation_space_available)
        {
            break;
        }

        current_child_meta = current_child_meta->next_child_meta;
    }

    void* object_alloc_address = current_child_meta->object_ptr + current_child_meta->size;
    ChildMeta* object_child_meta = ((void*)current_child_meta) + CHILD_META_SIZE; 

    if ((void*)object_child_meta > pmc.allocation_start_location) 
    {
        /*
            PMC memory Exceeded. 
            TODO:Need to split this into a separate function later.
        */
        MemoryChunk associated_pmc = get_process_memory_chunk(pmc.header->total_size * 2);
        MemoryChunk*  pmc_heap = (MemoryChunk*) malloc(sizeof(MemoryChunk));
        memcpy(pmc_heap, &associated_pmc, sizeof(MemoryChunk));
        pmc.associated_next_pmc = pmc_heap;
        return perfalloc(*pmc_heap, object_size);
    }

    AllocObjHeader alloc_obj = {
        allocating_object_size,
        header,
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
    object_child_meta->next_child_meta = NULL;
    header->occupied_space += allocating_object_size;
    return object_alloc_address + ALLOC_OBJECT_SIZE;
}