#include "memory_chunk.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>


Vector TOP_MEM_CHUNK_VEC;
bool IS_TOP_MEM_CHUNK_VEC_INITIALISED = false;

/**
Creates a new top memory chunk with the necessary meta and returns the pointer to it.
*/
MemoryChunk* get_top_memory_chunk(size_t tmc_size)
{
	MemoryChunk* memory_chunk = (MemoryChunk*)malloc(sizeof(MemoryChunk));
	memory_chunk->memory = malloc(tmc_size);
	memory_chunk->allocation_start_location = memory_chunk->memory + (int)(tmc_size * 0.1);
	memory_chunk->parent_header = NULL;

	Header header = {
		true,
		tmc_size,
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
	
	memory_chunk->header = memory_chunk->memory;
	memory_chunk->head_child_meta = memory_chunk->memory + MC_HEADER_SIZE;
	
	return memory_chunk;
}


MemoryChunk* get_initial_top_memory_chunk()
{
	return get_top_memory_chunk(INITIAL_TOP_MEMORY_CHUNK_SIZE);
}


MemoryChunk allocate_pmc_and_return()
{
	/**
     * 
     * Need to handle case where top memory chunk size is not enough.
    */


    MemoryChunk* current_top_memory_chunk = (MemoryChunk*)vector_get(TOP_MEM_CHUNK_VEC, TOP_MEM_CHUNK_VEC.number_of_elements-1);
	ChildMeta* current_child_meta = current_top_memory_chunk->head_child_meta;

	while (current_child_meta->next_child_meta != NULL)
	{
        /**
            Case where children meta list size is exceeded needs to be handled.
        */

        if ((current_child_meta + 2 * CHILD_META_SIZE < current_child_meta->next_child_meta)
            || (current_child_meta->object_ptr + current_child_meta->size < (void*)(current_child_meta->next_child_meta)))
        {
            current_child_meta = current_child_meta->next_child_meta;
        }
        else 
        {
            break;
        }
	}

    void* memory_block = current_child_meta->object_ptr + current_child_meta->size;
	ChildMeta* child_meta_address = current_child_meta + CHILD_META_SIZE;

	ChildMeta child_meta = {
		memory_block,
		INITIAL_PROCESS_MEMORY_CHUNK_SIZE,
		false,
		current_child_meta->next_child_meta,
		current_child_meta
	};
	memcpy(child_meta_address, &child_meta, CHILD_META_SIZE);


	Header header = {
		false,
		INITIAL_PROCESS_MEMORY_CHUNK_SIZE,
		0,
		child_meta_address
	};
	memcpy(memory_block, &header, MC_HEADER_SIZE);


	void* allocation_start_location = memory_block + (int)(INITIAL_PROCESS_MEMORY_CHUNK_SIZE * 0.1);

	ChildMeta* head_child_meta = (ChildMeta*)(memory_block + MC_HEADER_SIZE);
	head_child_meta->object_ptr = allocation_start_location;
	head_child_meta->is_head = true;

	MemoryChunk pmc = {
		memory_block,
		current_top_memory_chunk->header,
		memory_block + MC_HEADER_SIZE,
		allocation_start_location,
		memory_block
	};
	current_top_memory_chunk->header->occupied_space += pmc.header->total_size;
	return pmc;
}