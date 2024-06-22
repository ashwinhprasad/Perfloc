#include "memory_chunk.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
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
		vector_add(&TOP_MEM_CHUNK_VEC, &top_memory_chunk);
		IS_TOP_MEM_CHUNK_VEC_INITIALISED = true;
	}

	MemoryChunk process_mem_chunk = allocate_pmc_and_return();
	return process_mem_chunk;

	/**
     * 
     * Need to handle case where top memory chunk size is not enough.
    */
}


/**
 * Each memory chunk will hava a pointer to it's object meta stored in the root memory chunk.
 * we traverse to the object meta and simply remove it.
 * 
 * To remove an object meta, previous object meta's next pointer needs to be pointed directly to the
 * current object meta's next pointer.
*/
void drop_memory_chunk(MemoryChunk pmc)
{

	Header* pmc_header = (Header*) pmc.memory;
	ChildMeta* pmc_root_obj_meta = (ChildMeta*) pmc_header->parent_memory_chunk_meta_list_object;

	pmc_root_obj_meta->previous_object_meta->next_object_meta = pmc_root_obj_meta->next_object_meta;

	if (pmc_root_obj_meta->next_object_meta == NULL)
	{
		/**
	 * Current memory chunks corresponding object meta in the root is the final object meta.
	*/
		return;
	}

	pmc_root_obj_meta->next_object_meta->previous_object_meta = pmc_root_obj_meta->previous_object_meta;

}
