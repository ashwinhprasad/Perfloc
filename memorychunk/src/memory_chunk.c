#include "memory_chunk.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Vector ROOT_MEM_CHUNK_VEC;
bool IS_ROOT_MEM_CHUNK_VEC_INITIALISED = false;


MemoryChunk get_new_process_memory_chunk()
{

    /**
     * The calling program is using the library for the first time. Need to initalize 
     * the root memory chunk vector and create the current root memory chunk
    */
    if (!IS_ROOT_MEM_CHUNK_VEC_INITIALISED)
    {
        ROOT_MEM_CHUNK_VEC = create_vector(4);
        MemoryChunk* root_memory_chunk = (MemoryChunk*) malloc(sizeof(MemoryChunk));
        root_memory_chunk->memory = (void*)malloc(INITIAL_ROOT_MEMORY_CHUNK_SIZE);
        
        Header rmc_header = {
            true,
            INITIAL_ROOT_MEMORY_CHUNK_SIZE,
            0,
            NULL
        };

        ObjectMeta object_meta_root = {
            NULL,
            0,
            true,
            NULL,
            NULL
        };

        memcpy(root_memory_chunk->memory,&rmc_header, MC_HEADER_SIZE);
        memcpy(root_memory_chunk->memory + MC_HEADER_SIZE, &object_meta_root, OBJECT_META_SIZE);
        
        vector_add(&ROOT_MEM_CHUNK_VEC, &root_memory_chunk);
        IS_ROOT_MEM_CHUNK_VEC_INITIALISED = true;

        /**
         * Create process memory chunk from root memory chunk
        */
        ObjectMeta* rmc_rom = &object_meta_root;
        ObjectMeta* rmc_first_object_meta = &object_meta_root + OBJECT_META_SIZE;
        rmc_rom->next_object_meta = rmc_first_object_meta;
        rmc_first_object_meta->previous_object_meta = rmc_rom;
        void* ptr_to_first_pmc = root_memory_chunk->memory + (int)(INITIAL_ROOT_MEMORY_CHUNK_SIZE * MEMORY_CHUNK_HEADER_SIZE_FACTOR);
        rmc_first_object_meta->start_address = ptr_to_first_pmc;
        
        
        Header pmc_header = {
            false,
            INITIAL_PROCESS_MEMORY_CHUNK_SIZE,
            0,
            rmc_first_object_meta
        };

        ObjectMeta pmc_root_object_meta = {
            NULL,
            0,
            true,
            NULL
        };

        memcpy(ptr_to_first_pmc,&pmc_header,MC_HEADER_SIZE);
        memcpy(ptr_to_first_pmc + MC_HEADER_SIZE, &pmc_root_object_meta, OBJECT_META_SIZE);

        MemoryChunk process_mem_chunk = {
            ptr_to_first_pmc
        };
        return process_mem_chunk;
    }

    /**
     * 
     * Need to handle case where root memory chunk size is not enough.
    */
}



void* perfalloc(MemoryChunk pmc, size_t size)
{
    void* memory = pmc.memory;
    Header* pmc_header = (Header*) memory;
    size_t memory_chunk_size = pmc_header->mc_size;
    size_t memory_meta_vec_size = memory_chunk_size * MEMORY_CHUNK_HEADER_SIZE_FACTOR;
    void* alloc_region_start_address = memory + memory_meta_vec_size;
    ObjectMeta* root_object_meta = memory + MC_HEADER_SIZE;

    /**
     * Need to handle the case where memory chunk is not enough
    */
    if (pmc_header->occupied_space + size > memory_chunk_size)
    {

    }

    // find a space where the memory chunk of the given size can be properly declared
    ObjectMeta* current_object_meta = root_object_meta;
    void* previous_object_end_address = NULL;
    bool is_object_meta_insertion_location_found = false;
    
    while(current_object_meta != NULL)
    {
        if (current_object_meta->is_root)
        {
            previous_object_end_address = alloc_region_start_address;
        }
        else if (previous_object_end_address + size < current_object_meta->start_address)
        {
            break;
        }
        else
        {
            previous_object_end_address = current_object_meta->start_address + current_object_meta->size;
        }
    
        current_object_meta = current_object_meta->next_object_meta;
    }

    void* location_to_insert = previous_object_end_address;
    return location_to_insert;
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
    ObjectMeta* pmc_root_obj_meta = (ObjectMeta*) pmc_header->root_obj_meta_ptr;

    pmc_root_obj_meta->previous_object_meta->next_object_meta = pmc_root_obj_meta->next_object_meta;
    pmc_root_obj_meta->next_object_meta->previous_object_meta = pmc_root_obj_meta->previous_object_meta;
    


}