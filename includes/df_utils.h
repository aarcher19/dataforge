#ifndef DF_UTILS_H
#define DF_UTILS_H

#include "df_iterator.h"
#include <stdbool.h>

void *df_map(Iterator *it, void *(*func)(void *element));

void *df_filter(Iterator *it, bool (*func)(void *element));

void *df_find(Iterator *it, bool (*func)(void *element));

void df_for_each(Iterator *it, void (*func)(void *element));

size_t df_count(Iterator *it, bool (*func)(void *element));

void *df_reduce(Iterator *it, void *initial, void (*func)(void *accumulator, void *element));

void df_free_all(Iterator *it);

#endif
