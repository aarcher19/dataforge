#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include "df_iterator.h"

/*🔹 Insertion & Deletion
dfarray_insert_at(DfArray *array, size_t index, void *value)
Inserts a value at a specific index, shifting elements right.

dfarray_remove_at(DfArray *array, size_t index)
Removes an element at a given index, shifting elements left.
*/

typedef struct DfArray DfArray;

DfArray *dfarray_create(size_t elem_size, size_t initial_capcity);

void dfarray_destroy(DfArray *array);

void *dfarray_get(DfArray *array, size_t index);

void dfarray_push(DfArray *array, void *value);

void *dfarray_pop(DfArray *array);

void *dfarray_shift(DfArray *array);

void dfarray_unshift(DfArray *array, void *value);

void dfarray_set(DfArray *array, size_t index, void *value);

void dfarray_insert_at(DfArray *array, size_t index, void *value);

void dfarray_remove_at(DfArray *array, size_t index);

// Will be removed after iterators and utils are implemented!
void dfarray_map(DfArray *array, void (*func)(void *));

// Iterator
typedef struct DfArray_Iterator DfArray_Iterator;

Iterator dfarray_iterator_create(DfArray *array);

int dfarray_iterator_has_next(Iterator *it);

void *dfarray_iterator_next(Iterator *it);

#endif
