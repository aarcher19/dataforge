#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/df_array.h"
#include "../includes/df_iterator.h"

// Core functionality

typedef struct DfArray {
  void *items;
  size_t length;
  size_t elem_size;
  size_t capacity;
} DfArray;

DfArray* DfArray_Create(size_t elem_size, size_t initial_capacity) { 
  DfArray *array = malloc(sizeof(DfArray));
  if (!array) {
    perror("Struct memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  array->items = malloc(initial_capacity * elem_size);
  if (!array->items){
    free(array);
    perror("Items memory allocation failed.");
    exit(EXIT_FAILURE);  
  }

  array->length = 0;
  array->elem_size = elem_size;
  array->capacity = initial_capacity;

  return array;
}

void DfArray_Destroy(DfArray* array) {
  if (array) {
    free(array->items);
    free(array);
  }
}

void DfArray_Resize(DfArray* array) {
  size_t new_capacity;
  void *resized_items;
  if (array->capacity == 0) {
    new_capacity = 5;
    resized_items = malloc(new_capacity * array->elem_size);
  } else {
    new_capacity = array->capacity * 2;
    resized_items = realloc(array->items, new_capacity * array->elem_size);
  }
  
  if(!resized_items) {
    perror("Reallocation of IntArray->capacity failed");
    return;
  }

  array->items = resized_items;
  array->capacity = new_capacity;
}

void DfArray_Shrink(DfArray* array) {
  if (array->length == 0) {
    free(array->items);
    array->items = NULL;
    array->capacity = 0;
    return;
  }

  size_t new_capacity = array->length;
  void *shrunk_items = realloc(array->items, new_capacity * array->elem_size);

  if (!shrunk_items) {
    perror("Realloc failed in dfArray_Shrink.");
  }

  array->items = shrunk_items;
  array->capacity = new_capacity;
}

void *DfArray_Get(DfArray *array, size_t index) {
  if (index >= array->length) {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }
  void *dest = malloc(array->elem_size);
  memcpy(dest, (char *)array->items + index * array->elem_size, array->elem_size);
  return dest;
}

void DfArray_Set(DfArray *array, size_t index, void *value) {
  if (index >= array->length) {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }
  memcpy((char *)array->items + index * array->elem_size, value, array->elem_size);
}

void DfArray_Push(DfArray* array, void *value) {
  if (array->length >= array->capacity) {
    DfArray_Resize(array);
  }

  memcpy((char *)array->items + array->length * array->elem_size, value, array->elem_size);
  array->length++;
}

void *DfArray_Pop(DfArray *array) {
  if (array->length < 1) {
    fprintf(stderr, "Error: Array is empty, can not pop\n");
    exit(1);
  } 
  void *dest = malloc(array->elem_size);
  memcpy(dest, (char *)array->items + (array->length - 1) * array->elem_size, array->elem_size);
  array->length--;

  if (array->length <= array->capacity/2 || array->length == 0) {
    DfArray_Shrink(array);
  }

  return dest;
}

void *DfArray_Shift(DfArray* array) {
  if (array->length == 0) {
    fprintf(stderr, "Error: Cannot shift from an empty array\n");
    exit(1);
  }
  void *dest = malloc(array->elem_size);
  memcpy(dest, array->items, array->elem_size);
  memmove(array->items, (char *)array->items + array->elem_size, (array->length -1) * array->elem_size);
  array->length--;

  if (array->length <= array->capacity/2 || array->length == 0) {
    DfArray_Shrink(array);
  }

  return dest;
}

void DfArray_Unshift(DfArray* array, void *value) {
  if (array->length >= array->capacity) {
    DfArray_Resize(array);
  }
  
  memmove((char *)array->items + array->elem_size, array->items, array->length * array->elem_size);
  memcpy(array->items, value, array->elem_size);

  array->length++;
}

void DfArray_InsertAt(DfArray *array, size_t index, void *value) {
  if (index < array->length) {
    if (array->length >= array->capacity) {
      DfArray_Resize(array);
    }

    memmove(
      (char *)array->items + (index + 1) * array->elem_size, 
      (char *)array->items + index * array->elem_size, 
      (array->length - index) * array->elem_size
    );

    memcpy((char *)array->items + index * array->elem_size, value, array->elem_size);

    array->length++;
  } else if (index == array->length) {
    DfArray_Push(array, value);
  } else {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }
}

void DfArray_RemoveAt(DfArray *array, size_t index) {
  if (index < array->length) {
    memmove(
      (char *)array->items + index * array->elem_size,
      (char *)array->items + (index + 1) * array->elem_size,
      (array->length - index - 1) * array->elem_size
    );

    array->length--;

  } else {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }

  if (array->length < array->capacity/2) {
      DfArray_Shrink(array);
  }
}

void DfArray_Map(DfArray *array, void (*func)(void *)) {
  for (size_t i = 0; i < array->length; i++) {
    func((char *)array->items + (i * array->elem_size));
  }
};

// Iterator

typedef struct DfArray_Iterator {
  DfArray *array;
  size_t index;
} DfArray_Iterator;

int DfArray_Iterator_Has_Next(Iterator *it) {
  DfArray_Iterator *arr_it = (DfArray_Iterator *)it->current;
  return arr_it->index < arr_it->array->length;
}

void *DfArray_Iterator_Next(Iterator *it) {
  DfArray_Iterator *arr_it = (DfArray_Iterator *)it->current;
  if (!DfArray_Iterator_Has_Next(it)) return NULL;
  return (char *)arr_it->array->items + (arr_it->index++ * arr_it->array->elem_size);
}
    
void *DfArray_Create_New(Iterator *it) {
  DfArray_Iterator *arr_it = (DfArray_Iterator *)it->current;

  DfArray *new_array = DfArray_Create(arr_it->array->elem_size, arr_it->array->capacity);

  return new_array;
}

void DfArray_Insert_New(void *new_ds, void *element) {
  DfArray *arr = (DfArray *)new_ds;
  
  DfArray_Push(arr, element);
}

size_t DfArray_Elem_Size(Iterator *it) {
  DfArray *array =(DfArray *)it->structure;
  return array->elem_size;
}

void DfArray_Free_All(Iterator *it) {
  DfArray *array = (DfArray *)it->structure;
  free(array->items);
  array->items = NULL;
  array->capacity = 0;
  array->length = 0;
}

Iterator DfArray_Iterator_Create(DfArray *array) {
  DfArray_Iterator *it = malloc(sizeof(DfArray_Iterator));
  it->array = array;
  it->index = 0;

  return (Iterator){
    .structure = array,
    .current = it,
    .next = DfArray_Iterator_Next,
    .has_next = DfArray_Iterator_Has_Next,
    .create_new = DfArray_Create_New,
    .insert_new = DfArray_Insert_New,
    .elem_size = DfArray_Elem_Size,
    .free_all = DfArray_Free_All
  };
}

