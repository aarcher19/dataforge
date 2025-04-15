#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/df_array.h"
#include "../includes/df_iterator.h"
#include "../includes/df_common.h"

// Core functionality

typedef struct DfArray
{
  void *items;
  size_t length;
  size_t elem_size;
  size_t capacity;
} DfArray;

DfResult dfarray_create(size_t elem_size, size_t initial_capacity)
{
  DfResult res;
  res.value = NULL;

  DfArray *array = malloc(sizeof(DfArray));
  if (!array)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  array->items = malloc(initial_capacity * elem_size);
  if (!array->items)
  {
    free(array);
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  array->length = 0;
  array->elem_size = elem_size;
  array->capacity = initial_capacity;

  res.value = array;
  return res;
}

DfResult dfarray_destroy(DfArray *array)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  free(array->items);
  free(array);

  res.error = DF_OK;
  return res;
}

DfResult dfarray_resize(DfArray *array)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

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
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  array->items = resized_items;
  array->capacity = new_capacity;

  res.error = DF_OK;
  return res;
}

DfResult dfarray_shrink(DfArray *array)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (array->length == 0)
  {
    free(array->items);
    array->items = NULL;
    array->capacity = 0;

    res.error = DF_OK;
    return res;
  }

  size_t new_capacity = array->length;
  void *shrunk_items = realloc(array->items, new_capacity * array->elem_size);

  if (!shrunk_items)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  array->items = shrunk_items;
  array->capacity = new_capacity;

  res.error = DF_OK;
  return res;
}

DfResult dfarray_get(DfArray *array, size_t index)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (index >= array->length)
  {
    res.error = DF_ERR_INDEX_OUT_OF_BOUNDS;
    return res;
  }

  void *dest = malloc(array->elem_size);
  if (!dest)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  memcpy(dest, (char *)array->items + index * array->elem_size, array->elem_size);

  res.value = dest;
  res.error = DF_OK;
  return res;
}

DfResult dfarray_set(DfArray *array, size_t index, void *value)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (index >= array->length)
  {
    res.error = DF_ERR_INDEX_OUT_OF_BOUNDS;
    return res;
  }

  memcpy((char *)array->items + index * array->elem_size, value, array->elem_size);

  res.error = DF_OK;
  return res;
}

DfResult dfarray_push(DfArray *array, void *value)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (array->length >= array->capacity)
  {
    DfResult resize_res = dfarray_resize(array);
    if (resize_res.error != DF_OK)
    {
      return resize_res;
    }
  }

  memcpy((char *)array->items + array->length * array->elem_size, value, array->elem_size);
  array->length++;

  res.error = DF_OK;
  return res;
}

DfResult dfarray_pop(DfArray *array)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (array->length < 1)
  {
    res.error = DF_ERR_EMPTY;
    return res;
  }

  void *dest = malloc(array->elem_size);
  if (!dest)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  memcpy(dest, (char *)array->items + (array->length - 1) * array->elem_size, array->elem_size);
  array->length--;

  if (array->length <= array->capacity / 2 || array->length == 0)
  {
    DfResult shrink_res = dfarray_shrink(array);
    if (shrink_res.error != DF_OK)
    {
      free(dest);
      return shrink_res;
    }
  }

  res.error = DF_OK;
  res.value = dest;
  return res;
}

DfResult dfarray_shift(DfArray *array)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (array->length == 0)
  {
    res.error = DF_ERR_EMPTY;
    return res;
  }

  void *dest = malloc(array->elem_size);
  if (!dest)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  memcpy(dest, array->items, array->elem_size);
  memmove(array->items, (char *)array->items + array->elem_size, (array->length - 1) * array->elem_size);
  array->length--;

  if (array->length <= array->capacity / 2 || array->length == 0)
  {
    DfResult shrink_res = dfarray_shrink(array);
    if (shrink_res.error != DF_OK)
    {
      free(dest);
      return shrink_res;
    }
  }

  res.error = DF_OK;
  res.value = dest;
  return res;
}

DfResult dfarray_unshift(DfArray *array, void *value)
{
  DfResult res;
  res.value = NULL;

  if (!array || !value)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (array->length >= array->capacity)
  {
    DfResult resize_res = dfarray_resize(array);
    if (resize_res.error != DF_OK)
    {
      return resize_res;
    }
  }

  memmove((char *)array->items + array->elem_size, array->items, array->length * array->elem_size);
  memcpy(array->items, value, array->elem_size);

  array->length++;

  res.error = DF_OK;
  return res;
}

DfResult dfarray_insert_at(DfArray *array, size_t index, void *value)
{
  DfResult res;
  res.value = NULL;

  if (!array || !value)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (index > array->length)
  {
    res.error = DF_ERR_INDEX_OUT_OF_BOUNDS;
    return res;
  }

  if (index < array->length)
  {
    if (array->length >= array->capacity)
    {
      DfResult resize_res = dfarray_resize(array);
      if (resize_res.error != DF_OK)
      {
        return resize_res;
      }
    }

    memmove(
        (char *)array->items + (index + 1) * array->elem_size,
        (char *)array->items + index * array->elem_size,
        (array->length - index) * array->elem_size);

    memcpy((char *)array->items + index * array->elem_size, value, array->elem_size);
    array->length++;
  }
  else // index == array->length
  {
    DfResult push_res = dfarray_push(array, value);
    if (push_res.error != DF_OK)
    {
      return push_res;
    }
  }

  res.error = DF_OK;
  return res;
}

DfResult dfarray_remove_at(DfArray *array, size_t index)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (index >= array->length)
  {
    res.error = DF_ERR_INDEX_OUT_OF_BOUNDS;
    return res;
  }

  memmove(
      (char *)array->items + index * array->elem_size,
      (char *)array->items + (index + 1) * array->elem_size,
      (array->length - index - 1) * array->elem_size);

  array->length--;

  if (array->length < array->capacity / 2)
  {
    DfResult shrink_res = dfarray_shrink(array);
    if (shrink_res.error != DF_OK)
    {
      return shrink_res;
    }
  }

  res.error = DF_OK;
  return res;
}

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

DfResult dfarray_iterator_next(Iterator *it)
{
  DfResult res;
  res.value = NULL;

  if (!it || !it->current)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfArray_Iterator *arr_it = (DfArray_Iterator *)it->current;

  if (arr_it->index >= arr_it->array->length)
  {
    res.error = DF_ERR_INDEX_OUT_OF_BOUNDS;
    return res;
  }

  void *elem_ptr = (char *)arr_it->array->items + (arr_it->index++ * arr_it->array->elem_size);
  void *copied_elem = malloc(arr_it->array->elem_size);
  if (!copied_elem)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  memcpy(copied_elem, elem_ptr, arr_it->array->elem_size);

  res.error = DF_OK;
  res.value = copied_elem;
  return res;
}

DfResult dfarray_create_new(Iterator *it)
{
  DfResult res;
  res.value = NULL;

  if (!it)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfArray_Iterator *arr_it = (DfArray_Iterator *)it->current;

  DfResult new_array_res = dfarray_create(arr_it->array->elem_size, arr_it->array->capacity);
  DfArray *new_array = (DfArray *)new_array_res.value;

  res.error = DF_OK;
  res.value = new_array;
  return res;
}

DfResult dfarray_insert_new(void *new_ds, void *element)
{
  DfResult res;
  res.value = NULL;

  if (!new_ds || !element)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfArray *arr = (DfArray *)new_ds;

  DfResult push_res = dfarray_push(arr, element);
  if (push_res.error)
  {
    return push_res;
  }

  res.error = DF_OK;
  return res;
}

size_t dfarray_elem_size(Iterator *it)
{
  DfArray *array = (DfArray *)it->structure;
  return array->elem_size;
}

DfResult dfarray_free_all(Iterator *it)
{
  DfResult res;
  res.value = NULL;

  if (!it || !it->structure)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfArray *array = (DfArray *)it->structure;

  if (!array->items)
  {
    res.error = DF_ERR_ALREADY_FREED;
    return res;
  }

  free(array->items);
  array->items = NULL;
  array->capacity = 0;
  array->length = 0;

  res.error = DF_OK;
  return res;
}

DfResult dfarray_iterator_create(DfArray *array)
{
  DfResult res;
  res.value = NULL;

  if (!array)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfArray_Iterator *array_it = malloc(sizeof(DfArray_Iterator));
  if (!array_it)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  array_it->array = array;
  array_it->index = 0;

  Iterator *it = malloc(sizeof(Iterator));
  if (!it)
  {
    free(array_it);
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  it->structure = array;
  it->current = array_it;
  it->next = dfarray_iterator_next;
  it->has_next = dfarray_iterator_has_next;
  it->create_new = dfarray_create_new;
  it->insert_new = dfarray_insert_new;
  it->elem_size = dfarray_elem_size;
  it->free_all = dfarray_free_all;

  res.error = DF_OK;
  res.value = it;
  return res;
}
