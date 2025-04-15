#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include "df_iterator.h"
#include "df_common.h"

typedef struct DfArray DfArray;

DfResult dfarray_create(size_t elem_size, size_t initial_capacity);

DfResult dfarray_destroy(DfArray *array);

DfResult dfarray_get(DfArray *array, size_t index);

DfResult dfarray_push(DfArray *array, void *value);

DfResult dfarray_pop(DfArray *array);

DfResult dfarray_shift(DfArray *array);

DfResult dfarray_unshift(DfArray *array, void *value);

DfResult dfarray_set(DfArray *array, size_t index, void *value);

DfResult dfarray_insert_at(DfArray *array, size_t index, void *value);

DfResult dfarray_remove_at(DfArray *array, size_t index);

// Iterator
typedef struct DfArray_Iterator DfArray_Iterator;

DfResult dfarray_iterator_create(DfArray *array);

int dfarray_iterator_has_next(Iterator *it);

DfResult dfarray_iterator_next(Iterator *it);

#endif
