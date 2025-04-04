#include <stdlib.h>
#include "../includes/df_iterator.h"

void Iterator_Destroy(Iterator *it) {
    if (it && it->current) {
        free(it->current);
        it->current = NULL;
    }
}
