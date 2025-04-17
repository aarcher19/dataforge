#ifndef DF_INTERNAL_H
#define DF_INTERNAL_H

#include "../includes/df_common.h"
#include <stdlib.h>

static inline DfResult df_result_init()
{
    return (DfResult){.error = DF_OK, .value = NULL};
}

void df_null_ptr_check(void *ptr, DfResult *res);

void df_index_check_access(size_t index, size_t length, DfResult *res);

void df_index_check_insert(size_t index, size_t length, DfResult *res);

#endif
