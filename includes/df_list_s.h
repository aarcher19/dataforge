#ifndef LIST_S_H
#define LIST_S_H

#include "df_common.h"

// Single Linked
typedef struct DfList_S DfList_S;

typedef struct DfList_S_Node DfList_S_Node;

// typedef struct DfList_D DfList_D;

// typedef struct DfList_D_Node DfList_D_Node;

DfResult dflist_s_create();

DfResult dflist_s_destroy(DfList_S *list, void (*cleanup)(void *element));

DfResult dflist_s_push_back(DfList_S *list, void *element);

DfResult dflist_s_push_front(DfList_S *list, void *element);

DfResult dflist_s_pop_front(DfList_S *list);

DfResult dflist_s_pop_back(DfList_S *list);

#endif
