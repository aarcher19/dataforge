#ifndef DF_UTILS_H
#define DF_UTILS_H

#include "df_iterator.h"
void *DfMap(Iterator *it, void *(*func)(void *element));

#endif
