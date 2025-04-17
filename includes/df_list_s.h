#ifndef LIST_S_H
#define LIST_S_H

#include "df_common.h"
#include <stdlib.h>

typedef struct DfList_S DfList_S;

typedef struct DfList_S_Node DfList_S_Node;

DfResult dflist_s_create();

DfResult dflist_s_destroy(DfList_S *list, void (*cleanup)(void *element));

DfResult dflist_s_push_back(DfList_S *list, void *element);

DfResult dflist_s_push_front(DfList_S *list, void *element);

DfResult dflist_s_pop_front(DfList_S *list);

DfResult dflist_s_pop_back(DfList_S *list);

DfResult dflist_s_insert_at(DfList_S *list, void *element, size_t index);

DfResult dflist_s_remove_at(DfList_S *list, size_t index);

DfResult dflist_s_get(DfList_S *list, size_t index);

DfResult dflist_s_peek_front(DfList_S *list);

DfResult dflist_s_peek_back(DfList_S *list);

DfResult dflist_s_length(DfList_S *list);

#endif
