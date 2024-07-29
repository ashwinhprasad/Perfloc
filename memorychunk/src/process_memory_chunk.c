#include "memory_chunk.h"
#include "vector.h"
#include <stdbool.h>
#include "top_memory_chunk.h"


MemoryChunk get_process_memory_chunk()
{

	if (!IS_TOP_MEM_CHUNK_VEC_INITIALISED)
	{
		/**
		* The calling program is using the library for the first time. Need to initalize 
		* the top memory chunk vector and create the current top memory chunk
		*/
		TOP_MEM_CHUNK_VEC = create_vector(4);
		MemoryChunk* top_memory_chunk = get_initial_top_memory_chunk();
		vector_add(&TOP_MEM_CHUNK_VEC, top_memory_chunk);
		IS_TOP_MEM_CHUNK_VEC_INITIALISED = true;
	}

	MemoryChunk process_mem_chunk = allocate_pmc_and_return();
	return process_mem_chunk;
}



void drop_memory_chunk(MemoryChunk pmc)
{
	pmc.head_child_meta->previous_child_meta->next_child_meta = pmc.head_child_meta->next_child_meta;
	pmc.head_child_meta->next_child_meta->previous_child_meta = pmc.head_child_meta->previous_child_meta;
	pmc.parent_header->total_size -= pmc.header->total_size;
}
