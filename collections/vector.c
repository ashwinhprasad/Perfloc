#include "vector.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

Vector create_vector_With_Default_Cap()
{
    return create_vector(DEFAULT_VEC_SIZE);
}


Vector create_vector(int number_of_elements) 
{
    Vector vec = {
        (void *)malloc(sizeof(void*) * number_of_elements),
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

    void** elem_address = vec.ptrs + elem_idx * sizeof(void*);
    return *elem_address;
}



void vector_add(Vector vec, void* element)
{
    if(vec.number_of_elements >= vec.total_capacity)
    {
        vec.ptrs = realloc(vec.ptrs, sizeof(void*) * vec.total_capacity * 2);
        if (vec.ptrs != NULL)
        {
            vec.total_capacity *= 2;
        }
    }

    void* ptr = vec.ptrs + sizeof(void*) * vec.number_of_elements;
    mempcpy(ptr, &element, sizeof(void*));
    vec.number_of_elements++;
}


void free_vec(Vector vec) 
{
    free(vec.ptrs);
}