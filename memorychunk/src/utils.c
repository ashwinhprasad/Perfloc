// #include "memory_chunk.h"
// #include "utils.h"

// void print_memory_chunk_layout(MemoryChunk mc)
// {
//     Header* mc_header = (Header*)mc.memory;
//     size_t mc_size = mc_header->total_size;

//     ObjectMeta* pmc_rom = (ObjectMeta*)((void*)mc_header + MC_HEADER_SIZE);

//     printf("Memory Chunk Size : %zd\n", mc_size);

//     ObjectMeta* current_object_meta = pmc_rom->next_object_meta;
//     while (current_object_meta!= NULL)
//     {
//         void* object_allocated_address = current_object_meta->start_address;
//         size_t object_size = current_object_meta->size;
//         printf("Object Allocated Address: %p\nObject Size: %zd\n\n", object_allocated_address, object_size);
//         current_object_meta = current_object_meta->next_object_meta;
//     }
// }