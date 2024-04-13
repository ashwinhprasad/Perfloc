#include "memory_chunk.h"
#include "../collections/vector.h"

Vector ROOT_MEM_CHUNK_VEC;
bool IS_ROOT_MEM_CHUNK_VEC_INITIALISED = false;




MemoryChunk get_New_Process_Memory_Chunk()
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
            INITIAL_ROOT_MEMORY_CHUNK_SIZE,
            0,
            true
        };

        ObjectMeta object_meta = {
            NULL,
            0,
            true,
            NULL
        };

        memcpy(root_memory_chunk->memory,&rmc_header, MC_HEADER_SIZE);
        memcpy(root_memory_chunk->memory + MC_HEADER_SIZE, &object_meta, OBJECT_META_SIZE);
        vector_add(ROOT_MEM_CHUNK_VEC, &root_memory_chunk);
    }
}



