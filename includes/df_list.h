#ifndef LIST_H
#define LIST_H

#include <stdio.h>

// Single Linked
typedef struct DfList_S DfList_S;

typedef struct DfList_S_Node DfList_S_Node;

DfList_S *DfList_S_Create(size_t elem_size);

void DfList_S_Destroy(DfList_S *list);

#endif
