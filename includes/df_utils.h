#ifndef DF_UTILS_H
#define DF_UTILS_H

#include "df_iterator.h"
#include <stdbool.h>

void *DfMap(Iterator *it, void *(*func)(void *element));

void *DfFilter(Iterator *it, bool (*func)(void *element));

#endif
