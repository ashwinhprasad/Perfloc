#ifndef VECTOR_H

#define VECTOR_H

#define DEFAULT_VEC_SIZE 16

typedef struct Pointer {
  void* element;
} Pointer;

typedef struct Vector {
  Pointer* pointers;
  int number_of_elements;
  int total_capacity;  
} Vector;

Vector create_vector_with_default_cap();
Vector create_vector(int);
void* vector_get(Vector, int);
void vector_add(Vector*, void*);
void free_vec(Vector);

#endif