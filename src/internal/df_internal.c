#include "../../internal/df_internal.h"
#include "../../includes/df_common.h"

void df_null_ptr_check(void *ptr, DfResult *res)
{
    if (!ptr)
    {
        res->error = DF_ERR_NULL_PTR;
    }
}

void df_index_check_access(size_t index, size_t length, DfResult *res)
{
    if (index >= length)
    {
        res->error = DF_ERR_INDEX_OUT_OF_BOUNDS;
    }
}

void df_index_check_insert(size_t index, size_t length, DfResult *res)
{
    if (index > length)
    {
        res->error = DF_ERR_INDEX_OUT_OF_BOUNDS;
    }
}