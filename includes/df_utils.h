#ifndef DF_UTILS_H
#define DF_UTILS_H

#include "df_iterator.h"
#include <stdbool.h>

void *DfMap(Iterator *it, void *(*func)(void *element));

void *DfFilter(Iterator *it, bool (*func)(void *element));

void *DfFind(Iterator *it, bool (*func)(void *element));

void DfForEach(Iterator *it, void (*func)(void *element));

size_t DfCount(Iterator *it, bool (*func)(void *element));

void *DfReduce(Iterator *it, void *initial, void (*func)(void *accumulator, void *element));

#endif
