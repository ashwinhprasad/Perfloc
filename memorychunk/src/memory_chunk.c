#include "memory_chunk.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Vector TOP_MEM_CHUNK_VEC;
bool IS_TOP_MEM_CHUNK_VEC_INITIALISED = false;

/**
Creates a new top memory chunk with the necessary meta and returns the pointer to it.
*/
MemoryChunk* get_initial_top_memory_chunk()
{
	MemoryChunk* top_memory_chunk = (MemoryChunk*) malloc(sizeof(MemoryChunk));
	top_memory_chunk->memory = (void*)malloc(INITIAL_TOP_MEMORY_CHUNK_SIZE);

	Header top_memory_chunk_header = {
		true,
		INITIAL_TOP_MEMORY_CHUNK_SIZE,
		0,
		NULL
	};

	ObjectMeta head_object_meta = {
		NULL,
		0,
		true,
		NULL,
		NULL
	};

	memcpy(top_memory_chunk->memory,&top_memory_chunk_header, MC_HEADER_SIZE);
	memcpy(top_memory_chunk->memory + MC_HEADER_SIZE, &head_object_meta, OBJECT_META_SIZE);
	return top_memory_chunk;
}



MemoryChunk* get_process_memory_chunk_from_current_top()
{
	MemoryChunk* current_top_memory_chunk = vector_get(TOP_MEM_CHUNK_VEC, TOP_MEM_CHUNK_VEC.number_of_elements-1);
	ObjectMeta* head_object_meta = current_top_memory_chunk->head_object_meta;

	
}



MemoryChunk get_new_process_memory_chunk()
{

	if (!IS_TOP_MEM_CHUNK_VEC_INITIALISED)
	{
		/**
		* The calling program is using the library for the first time. Need to initalize 
		* the root memory chunk vector and create the current root memory chunk
		*/
		TOP_MEM_CHUNK_VEC = create_vector(4);
		MemoryChunk* top_memory_chunk = get_initial_top_memory_chunk();
		vector_add(&TOP_MEM_CHUNK_VEC, &top_memory_chunk);
		IS_TOP_MEM_CHUNK_VEC_INITIALISED = true;
	}

	get_process_memory_chunk_from_current_top();


	/**
     * 
     * Need to handle case where root memory chunk size is not enough.
    */
}

void perffree(MemoryChunk mc, void* object, size_t object_size)
{

}


void* perfalloc(MemoryChunk pmc, size_t size)
{
	void* memory = pmc.memory;
	Header* pmc_header = (Header*) memory;
	size_t memory_chunk_size = pmc_header->total_size;
	size_t memory_meta_vec_size = memory_chunk_size * MEMORY_CHUNK_HEADER_SIZE_FACTOR;
	void* alloc_region_start_address = memory + memory_meta_vec_size;
	ObjectMeta* pmc_rom = memory + MC_HEADER_SIZE;

	/**
     * Need to handle the case where memory chunk is not enough
    */
	if (pmc_header->occupied_space + size > memory_chunk_size)
	{

	}


	ObjectMeta* current_object_meta = pmc_rom;
	void* previous_object_end_address = NULL;
	void* previous_object_meta_start_address = NULL;
	void* next_object_meta_start_address = NULL;
	void* previous_object_meta_end_address = NULL;
	bool is_object_meta_insertion_location_found = false;

	while(current_object_meta != NULL)
	{
		if (current_object_meta->is_head)
		{
			previous_object_end_address = alloc_region_start_address;
			previous_object_meta_end_address = current_object_meta + OBJECT_META_SIZE;
			previous_object_meta_start_address = current_object_meta;
			next_object_meta_start_address = current_object_meta->next_object_meta;
		}
		else if ((previous_object_end_address + size < current_object_meta->start_address) && (previous_object_meta_end_address + OBJECT_META_SIZE < (void *)current_object_meta))
		{
			break;
		}
		else
	{
			previous_object_end_address = current_object_meta->start_address + current_object_meta->size;
			previous_object_meta_end_address = current_object_meta + OBJECT_META_SIZE;
			previous_object_meta_start_address = current_object_meta;
			next_object_meta_start_address = current_object_meta->next_object_meta;
		}

		current_object_meta = current_object_meta->next_object_meta;
	}

	/**
     * TODO:Also need to create new object meta for the current object
    */
	void* object_meta_location = previous_object_meta_end_address;
	void* object_location = previous_object_end_address;

	ObjectMeta* object_meta = (ObjectMeta*) object_meta_location;
	object_meta->start_address = object_location;
	object_meta->is_head = false;
	object_meta->size = size;
	object_meta->next_object_meta = next_object_meta_start_address;
	object_meta->previous_object_meta = previous_object_meta_start_address;
	object_meta->previous_object_meta->next_object_meta = object_meta;

	if (object_meta->next_object_meta != NULL)
	{
		object_meta->next_object_meta->previous_object_meta = object_meta;
	}

	return object_location;
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
	ObjectMeta* pmc_root_obj_meta = (ObjectMeta*) pmc_header->parent_memory_chunk_meta_list_object;

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
