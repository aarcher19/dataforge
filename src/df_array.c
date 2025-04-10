#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/df_array.h"
#include "../includes/df_iterator.h"

// Core functionality

typedef struct DfArray
{
  void *items;
  size_t length;
  size_t elem_size;
  size_t capacity;
} DfArray;

DfArray *dfarray_create(size_t elem_size, size_t initial_capacity)
{
  DfArray *array = malloc(sizeof(DfArray));
  if (!array)
  {
    perror("Struct memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  array->items = malloc(initial_capacity * elem_size);
  if (!array->items)
  {
    free(array);
    perror("Items memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  array->length = 0;
  array->elem_size = elem_size;
  array->capacity = initial_capacity;

  return array;
}

void dfarray_destroy(DfArray *array)
{
  if (array)
  {
    free(array->items);
    free(array);
  }
}

void dfarray_resize(DfArray *array)
{
  size_t new_capacity;
  void *resized_items;
  if (array->capacity == 0)
  {
    new_capacity = 5;
    resized_items = malloc(new_capacity * array->elem_size);
  }
  else
  {
    new_capacity = array->capacity * 2;
    resized_items = realloc(array->items, new_capacity * array->elem_size);
  }

  if (!resized_items)
  {
    perror("Reallocation of IntArray->capacity failed");
    return;
  }

  array->items = resized_items;
  array->capacity = new_capacity;
}

void dfarray_shrink(DfArray *array)
{
  if (array->length == 0)
  {
    free(array->items);
    array->items = NULL;
    array->capacity = 0;
    return;
  }

  size_t new_capacity = array->length;
  void *shrunk_items = realloc(array->items, new_capacity * array->elem_size);

  if (!shrunk_items)
  {
    perror("Realloc failed in dfArray_Shrink.");
  }

  array->items = shrunk_items;
  array->capacity = new_capacity;
}

void *dfarray_get(DfArray *array, size_t index)
{
  if (index >= array->length)
  {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }
  void *dest = malloc(array->elem_size);
  memcpy(dest, (char *)array->items + index * array->elem_size, array->elem_size);
  return dest;
}

void dfarray_set(DfArray *array, size_t index, void *value)
{
  if (index >= array->length)
  {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }
  memcpy((char *)array->items + index * array->elem_size, value, array->elem_size);
}

void dfarray_push(DfArray *array, void *value)
{
  if (array->length >= array->capacity)
  {
    dfarray_resize(array);
  }

  memcpy((char *)array->items + array->length * array->elem_size, value, array->elem_size);
  array->length++;
}

void *dfarray_pop(DfArray *array)
{
  if (array->length < 1)
  {
    fprintf(stderr, "Error: Array is empty, can not pop\n");
    exit(1);
  }
  void *dest = malloc(array->elem_size);
  memcpy(dest, (char *)array->items + (array->length - 1) * array->elem_size, array->elem_size);
  array->length--;

  if (array->length <= array->capacity / 2 || array->length == 0)
  {
    dfarray_shrink(array);
  }

  return dest;
}

void *dfarray_shift(DfArray *array)
{
  if (array->length == 0)
  {
    fprintf(stderr, "Error: Cannot shift from an empty array\n");
    exit(1);
  }
  void *dest = malloc(array->elem_size);
  memcpy(dest, array->items, array->elem_size);
  memmove(array->items, (char *)array->items + array->elem_size, (array->length - 1) * array->elem_size);
  array->length--;

  if (array->length <= array->capacity / 2 || array->length == 0)
  {
    dfarray_shrink(array);
  }

  return dest;
}

void dfarray_unshift(DfArray *array, void *value)
{
  if (array->length >= array->capacity)
  {
    dfarray_resize(array);
  }

  memmove((char *)array->items + array->elem_size, array->items, array->length * array->elem_size);
  memcpy(array->items, value, array->elem_size);

  array->length++;
}

void dfarray_insert_at(DfArray *array, size_t index, void *value)
{
  if (index < array->length)
  {
    if (array->length >= array->capacity)
    {
      dfarray_resize(array);
    }

    memmove(
        (char *)array->items + (index + 1) * array->elem_size,
        (char *)array->items + index * array->elem_size,
        (array->length - index) * array->elem_size);

    memcpy((char *)array->items + index * array->elem_size, value, array->elem_size);

    array->length++;
  }
  else if (index == array->length)
  {
    dfarray_push(array, value);
  }
  else
  {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }
}

void dfarray_remove_at(DfArray *array, size_t index)
{
  if (index < array->length)
  {
    memmove(
        (char *)array->items + index * array->elem_size,
        (char *)array->items + (index + 1) * array->elem_size,
        (array->length - index - 1) * array->elem_size);

    array->length--;
  }
  else
  {
    fprintf(stderr, "Error: Index %zu out of bounds (length: %zu)\n", index, array->length);
    exit(1);
  }

  if (array->length < array->capacity / 2)
  {
    dfarray_shrink(array);
  }
}

void dfarray_map(DfArray *array, void (*func)(void *))
{
  for (size_t i = 0; i < array->length; i++)
  {
    func((char *)array->items + (i * array->elem_size));
  }
};

// Iterator

typedef struct DfArray_Iterator
{
  DfArray *array;
  size_t index;
} DfArray_Iterator;

int dfarray_iterator_has_next(Iterator *it)
{
  DfArray_Iterator *arr_it = (DfArray_Iterator *)it->current;
  return arr_it->index < arr_it->array->length;
}

void *dfarray_iterator_next(Iterator *it)
{
  DfArray_Iterator *arr_it = (DfArray_Iterator *)it->current;
  if (!dfarray_iterator_has_next(it))
    return NULL;
  return (char *)arr_it->array->items + (arr_it->index++ * arr_it->array->elem_size);
}

void *dfarray_create_new(Iterator *it)
{
  DfArray_Iterator *arr_it = (DfArray_Iterator *)it->current;

  DfArray *new_array = dfarray_create(arr_it->array->elem_size, arr_it->array->capacity);

  return new_array;
}

void dfarray_insert_new(void *new_ds, void *element)
{
  DfArray *arr = (DfArray *)new_ds;

  dfarray_push(arr, element);
}

size_t dfarray_elem_size(Iterator *it)
{
  DfArray *array = (DfArray *)it->structure;
  return array->elem_size;
}

void dfarray_free_all(Iterator *it)
{
  DfArray *array = (DfArray *)it->structure;
  free(array->items);
  array->items = NULL;
  array->capacity = 0;
  array->length = 0;
}

Iterator dfarray_iterator_create(DfArray *array)
{
  DfArray_Iterator *it = malloc(sizeof(DfArray_Iterator));
  it->array = array;
  it->index = 0;

  return (Iterator){
      .structure = array,
      .current = it,
      .next = dfarray_iterator_next,
      .has_next = dfarray_iterator_has_next,
      .create_new = dfarray_create_new,
      .insert_new = dfarray_insert_new,
      .elem_size = dfarray_elem_size,
      .free_all = dfarray_free_all};
}
