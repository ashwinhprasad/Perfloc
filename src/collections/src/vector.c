#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include "vector.h"



Vector create_vector_with_default_cap()
{
    return create_vector(DEFAULT_VEC_SIZE);
}


Vector create_vector(int number_of_elements) 
{
    Vector vec = {
        malloc( sizeof(Pointer) * number_of_elements),
        0,
        number_of_elements
    };
    return vec;
}


void* vector_get(Vector vec, int elem_idx) 
{
    if (elem_idx >= vec.number_of_elements && elem_idx < 0)
    {
        return NULL;
    }

    Pointer* pointer = vec.pointers + elem_idx * sizeof(Pointer);
    return pointer->element;
}



void vector_add(Vector* vec, void* element)
{
    if(vec->number_of_elements >= vec->total_capacity)
    {
        vec->pointers = realloc(vec->pointers, sizeof(Pointer) * vec->total_capacity * 2);
        if (vec->pointers != NULL)
        {
            vec->total_capacity *= 2;
        }
    }

    Pointer* ptr = vec->pointers + sizeof(Pointer) * vec->number_of_elements;
    ptr->element = element;
    vec->number_of_elements++;
}


void free_vec(Vector vec) 
{
    free(vec.pointers);
}