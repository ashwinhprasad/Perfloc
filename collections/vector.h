#ifndef VECTOR_H

#define VECTOR_H

#include<stdio.h>

#define DEFAULT_VEC_SIZE 16

typedef struct Vector {
  void* ptrs;
  int number_of_elements;
  int total_capacity;  
} Vector;

Vector create_vector_with_default_cap();
Vector create_vector(int number_of_elements);
void* vector_get(Vector vec, int elem_idx);
void vector_add(Vector vec, void* value);
void free_vec(Vector vec);

#endif