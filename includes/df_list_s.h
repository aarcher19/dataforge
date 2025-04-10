#ifndef LIST_S_H
#define LIST_S_H

// Single Linked
typedef struct DfList_S DfList_S;

typedef struct DfList_S_Node DfList_S_Node;

// typedef struct DfList_D DfList_D;

// typedef struct DfList_D_Node DfList_D_Node;

DfList_S *dflist_s_create();

void dflist_s_destroy(DfList_S *list, void (*cleanup)(void *element));

void dflist_s_push_back(DfList_S *list, void *element);

void dflist_s_push_front(DfList_S *list, void *element);

void *dflist_s_pop_front(DfList_S *list);

void *dflist_s_pop_back(DfList_S *list);

#endif
