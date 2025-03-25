#include "../includes/df_list.h"
#include <stdio.h>
#include <stdlib.h>

// Singly Linked
typedef struct DfList_S {
  DfList_S_Node *head;
  DfList_S_Node *tail;
  size_t length;
} DfList_S;

typedef struct DfList_S_Node {
  void *element;
  struct DfList_S_Node *next;
  size_t elem_size;
} DfList_S_Node;

DfList_S *DfList_S_Create() {
  DfList_S *list = malloc(sizeof(DfList_S));
  if(!list) return NULL;

  list->head = NULL;
  list->tail = NULL;
  list->length = 0;

  return list;
}

void DfList_S_Destroy(DfList_S *list, void (*cleanup)(void *element)) {
  if(!list) return;

  DfList_S_Node *current = list->head;
  while(current) {
    DfList_S_Node *next = current->next;
    if(cleanup) cleanup(current->element);
    free(current);
    current = next;
  }

  free(list);
}

