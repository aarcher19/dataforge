#include <stdlib.h>
#include "../includes/df_iterator.h"

void iterator_destroy(Iterator *it)
{
    if (it && it->current)
    {
        free(it->current);
        it->current = NULL;
    }
}
