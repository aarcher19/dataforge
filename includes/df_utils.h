#ifndef DF_UTILS_H
#define DF_UTILS_H

#include "df_iterator.h"
#include "df_common.h"
#include <stdbool.h>

DfResult df_map(Iterator *it, void *(*func)(void *element));

DfResult df_filter(Iterator *it, bool (*func)(void *element));

DfResult df_find(Iterator *it, bool (*func)(void *element));

DfResult df_for_each(Iterator *it, void (*func)(void *element));

DfResult df_count(Iterator *it, bool (*func)(void *element));

DfResult df_reduce(Iterator *it, void *initial, void (*func)(void *accumulator, void *element));

DfResult df_free_all(Iterator *it);

#endif
