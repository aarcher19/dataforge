#ifndef DF_INTERNAL_H
#define DF_INTERNAL_H

#include "../includes/df_common.h"
#include "../includes/df_array.h"
#include "../includes/df_iterator.h"
#include <stdlib.h>

DfResult df_result_init();

void df_null_ptr_check(void *ptr, DfResult *res);

void df_index_check_access(size_t index, size_t length, DfResult *res);

void df_index_check_insert(size_t index, size_t length, DfResult *res);

DfResult dfarray_shrink(DfArray *array);

DfResult dfarray_resize(DfArray *array);

DfResult dfarray_free_all(Iterator *it);

DfResult dfarray_insert_new(void *new_ds, void *element);

DfResult dfarray_create_new(Iterator *it);

size_t dfarray_elem_size(Iterator *it);

#endif
