#include <stdlib.h>
#include "../includes/df_iterator.h"
#include "../includes/df_common.h"

DfResult iterator_destroy(Iterator *it)
{
    DfResult res;
    res.value = NULL;
    if (!it)
    {
        res.error = DF_ERR_NULL_PTR;
        return res;
    }

    if (it->current)
    {
        free(it->current);
        it->current = NULL;
    }

    res.error = DF_OK;
    return res;
}
