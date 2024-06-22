#include "perfloc.h"

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