#ifndef LIST_H
#define LIST_H

// Single Linked
typedef struct DfList_S DfList_S;

typedef struct DfList_S_Node DfList_S_Node;

DfList_S *DfList_S_Create();

void DfList_S_Destroy(DfList_S *list, void (*cleanup)(void *element));

#endif
