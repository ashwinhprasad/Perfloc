#include "memory_chunk.h"
#include <stdlib.h>
#include <string.h>


/**
Creates a new top memory chunk with the necessary meta and returns the pointer to it.
*/
MemoryChunk* get_initial_top_memory_chunk()
{

	MemoryChunk* memory_chunk = (MemoryChunk*)malloc(sizeof(MemoryChunk));
	memory_chunk->memory = malloc(INITIAL_TOP_MEMORY_CHUNK_SIZE);
	memory_chunk->allocation_start_location = memory_chunk->memory + (int)(INITIAL_TOP_MEMORY_CHUNK_SIZE * 0.1);

	Header header = {
		true,
		INITIAL_TOP_MEMORY_CHUNK_SIZE,
		0,
		NULL
	};

	ChildMeta head_object_meta = {
		memory_chunk->allocation_start_location,
		0,
		true,
		NULL,
		NULL
	};

	memcpy(memory_chunk->memory,&header, MC_HEADER_SIZE);
	memcpy(memory_chunk->memory + MC_HEADER_SIZE, &head_object_meta, CHILD_META_SIZE);
	return memory_chunk;
}




MemoryChunk allocate_pmc_and_return()
{
    MemoryChunk* current_top_memory_chunk = vector_get(TOP_MEM_CHUNK_VEC, TOP_MEM_CHUNK_VEC.number_of_elements-1);
	ChildMeta* current_child_meta = current_top_memory_chunk->head_child_meta;

	while (current_child_meta->next_child_meta != NULL)
	{
        /**
            Case where children meta list size is exceeded needs to be handled.
        */

        if ((current_child_meta + 2 * CHILD_META_SIZE < current_child_meta->next_child_meta)
            || (current_child_meta->object_ptr + current_child_meta->size < current_child_meta->next_child_meta))
        {
            current_child_meta = current_child_meta->next_child_meta;
        }
        else 
        {
            break;
        }
	}

    void* memory_block = current_child_meta->object_ptr + current_child_meta->size;

    /**
        Need to create header and head object meta and all those stuff and add to memory block and return.
        will do tomorrow.
     */
}