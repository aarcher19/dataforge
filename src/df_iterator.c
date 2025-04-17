#include <stdlib.h>
#include "../includes/df_iterator.h"
#include "../includes/df_common.h"
#include "../internal/df_internal.h"

DfResult iterator_create()
{
    DfResult res = df_result_init();

    Iterator *it = malloc(sizeof(Iterator));
    if (!it)
    {
        res.error = DF_ERR_ALLOC_FAILED;
        return res;
    }

    res.value = it;
    return res;
}

DfResult iterator_destroy(Iterator *it)
{
    DfResult res = df_result_init();

    df_null_ptr_check(it, &res);
    if (res.error)
    {
        return res;
    }

    if (it->current)
    {
        free(it->current);
        it->current = NULL;
    }

    return res;
}
