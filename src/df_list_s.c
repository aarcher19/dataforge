#include "../includes/df_list_s.h"
#include <criterion/alloc.h>
#include <stdio.h>
#include <stdlib.h>

// Singly Linked
typedef struct DfList_S
{
  DfList_S_Node *head;
  DfList_S_Node *tail;
  size_t length;
} DfList_S;

typedef struct DfList_S_Node
{
  void *element;
  struct DfList_S_Node *next;
} DfList_S_Node;

/*
typedef struct DfList_D {
  DfList_D_Node *head;
  DfList_D_Node *tail;
  size_t length;
} DfList_D;

typedef struct DfList_D_Node {
  void *element;
  struct DfList_D_Node *next;
  struct DfList_D_Node *prev;
} DfList_D_Node;
*/

DfList_S *dflist_s_create()
{
  DfList_S *list = malloc(sizeof(DfList_S));
  if (!list)
  {
    return NULL;
  }

  list->head = list->tail = NULL;
  list->length = 0;

  return list;
}

void dflist_s_destroy(DfList_S *list, void (*cleanup)(void *element))
{
  if (!list)
  {
    return;
  }

  DfList_S_Node *current = list->head;
  while (current)
  {
    DfList_S_Node *next = current->next;
    if (cleanup)
      cleanup(current->element);
    free(current);
    current = next;
  }

  free(list);
}

DfList_S_Node *dflist_s_create_node(void *element)
{
  DfList_S_Node *new_node = malloc(sizeof(DfList_S_Node));
  if (!new_node)
  {
    perror("Failed to allocate memory for new node");
    return NULL;
  }

  new_node->element = element;
  new_node->next = NULL;

  return new_node;
}

void dflist_s_push_back(DfList_S *list, void *element)
{
  if (!list)
  {
    return;
  }

  DfList_S_Node *new_node = dflist_s_create_node(element);
  if (!new_node)
  {
    return;
  }

  if (!list->head)
  {
    list->head = new_node;
    list->tail = new_node;
  }
  else
  {
    list->tail->next = new_node;
    list->tail = new_node;
  }

  list->length++;
}

void dflist_s_push_front(DfList_S *list, void *element)
{
  if (!list)
  {
    return;
  }

  DfList_S_Node *new_node = dflist_s_create_node(element);
  if (!new_node)
  {
    return;
  }

  if (!list->head)
  {
    list->head = new_node;
    list->tail = new_node;
  }
  else
  {
    new_node->next = list->head;
    list->head = new_node;
  }

  list->length++;
}

void *dflist_s_pop_front(DfList_S *list)
{
  if (!list)
  {
    return NULL;
  }

  if (!list->head)
  {
    perror("Can not pop from empty list");
    return NULL;
  }

  DfList_S_Node *old_head = list->head;
  void *dest = old_head->element;
  list->head = old_head->next;
  free(old_head);
  list->length--;

  // Ensure to document user takes control of element memory (need to free it when done)
  return dest;
}

void *dflist_s_pop_back(DfList_S *list)
{
  if (!list)
  {
    return NULL;
  }

  if (!list->tail)
  {
    perror("Can not pop from empty list");
    return NULL;
  }

  void *dest;

  if (list->head == list->tail)
  {
    dest = list->head->element;
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
  }
  else
  {
    DfList_S_Node *cur = list->head;

    while (cur->next != list->tail)
    {
      cur = cur->next;
    }

    dest = list->tail->element;
    free(list->tail);
    list->tail = cur;
    list->tail->next = NULL;
  }

  list->length--;

  // Ensure to document user takes control of element memory (need to free it when done)
  return dest;
}

void dflist_s_insert_at() {}
