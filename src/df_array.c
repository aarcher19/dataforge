#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/df_array.h"

typedef struct df_array {
  void *items;
  size_t length;
  size_t elem_size;
  size_t capacity;
} DfArray;

// Implements bounds checking when accessing index
void DfArray_Get(DfArray *array, size_t index, void *dest) {
  if (index < array->length) {
    memcpy(dest, (char *)array->items + index * array->elem_size, array->elem_size);
  } else {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }
}

// Initialize dynamic int array
DfArray* DfArray_Create(size_t elem_size, size_t initial_capacity) { 
  // Allocate memory for struct on heap
  DfArray *array = malloc(sizeof(DfArray));
  if (!array) {
    perror("Struct memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  // Allocate memory for items array on heap
  array->items = malloc(initial_capacity * elem_size);
  if (!array->items){
    free(array);
    perror("Items memory allocation failed.");
    exit(EXIT_FAILURE);  
  }

  // Initialize length and capacity values
  array->length = 0;
  array->elem_size = elem_size;
  array->capacity = initial_capacity;

  return array;
}

// Remove struct and items memory from heap
void DfArray_Destroy(DfArray* array) {
  // Prevent unnecessary dealocation with conditional
  if (array) {
    free(array->items);
    free(array);
  }
}

void DfArray_Resize(DfArray* array) {
  size_t new_capacity = array->capacity * 2;
  void *resized_items = realloc(array->items, new_capacity * array->elem_size);
  if(!resized_items) {
    perror("Reallocation of IntArray->capacity failed");
    return;
  }

  array->items = resized_items;
  array->capacity = new_capacity;
}

void DfArray_Push(DfArray* array, void *value) {
  // Handle array resizing if neccesary
  if (array->length >= array->capacity) {
    DfArray_Resize(array);
  }

  memcpy((char *)array->items + array->length * array->elem_size, value, array->elem_size);
  array->length++;
}

void DfArray_Pop(DfArray *array, void *dest) {
  if (array->length < 1) {
    fprintf(stderr, "Error: Array is empty, can not pop\n");
    exit(1);
  } else {
    memcpy(dest, (char *)array->items + (array->length - 1) * array->elem_size, array->elem_size);
    array->length--;

  }
}

void DfArray_Map(DfArray *array, void (*func)(void *)) {
  for (size_t i = 0; i < array->length; i++) {
    func((char *)array->items + (i * array->elem_size));
  }
};
