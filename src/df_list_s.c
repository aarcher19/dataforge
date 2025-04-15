#include "../includes/df_list_s.h"
#include "../includes/df_common.h"
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

DfResult dflist_s_create()
{
  DfResult res;
  res.value = NULL;

  DfList_S *list = malloc(sizeof(DfList_S));
  if (!list)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  list->head = list->tail = NULL;
  list->length = 0;

  res.error = DF_OK;
  res.value = list;
  return res;
}

DfResult dflist_s_destroy(DfList_S *list, void (*cleanup)(void *element))
{
  DfResult res;
  res.value = NULL;

  if (!list)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
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

  res.error = DF_OK;
  return res;
}

DfResult dflist_s_create_node(void *element)
{
  DfResult res;
  res.value = NULL;

  DfList_S_Node *new_node = malloc(sizeof(DfList_S_Node));
  if (!new_node)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  new_node->element = element;
  new_node->next = NULL;

  res.error = DF_OK;
  res.value = new_node;
  return res;
}

DfResult dflist_s_push_back(DfList_S *list, void *element)
{
  DfResult res;
  res.value = NULL;

  if (!list)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfResult new_node_res = dflist_s_create_node(element);
  DfList_S_Node *new_node = (DfList_S_Node *)new_node_res.value;
  if (new_node_res.error != DF_OK)
  {
    return new_node_res;
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

  res.error = DF_OK;
  return res;
}

DfResult dflist_s_push_front(DfList_S *list, void *element)
{
  DfResult res;
  res.value = NULL;

  if (!list)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfResult new_node_res = dflist_s_create_node(element);
  DfList_S_Node *new_node = (DfList_S_Node *)new_node_res.value;
  if (new_node_res.error != DF_OK)
  {
    return new_node_res;
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

  res.error = DF_OK;
  return res;
}

DfResult dflist_s_pop_front(DfList_S *list)
{
  DfResult res;
  res.value = NULL;

  if (!list)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (!list->head)
  {
    res.error = DF_ERR_EMPTY;
    return res;
  }

  DfList_S_Node *old_head = list->head;
  void *dest = old_head->element;
  list->head = old_head->next;
  free(old_head);
  list->length--;

  res.error = DF_OK;
  res.value = dest;
  // Ensure to document user takes control of element memory (need to free it when done)
  return res;
}

DfResult dflist_s_pop_back(DfList_S *list)
{
  DfResult res;
  res.value = NULL;

  if (!list)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (!list->tail)
  {
    res.error = DF_ERR_EMPTY;
    return res;
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

  res.error = DF_OK;
  res.value = dest;
  // Ensure to document user takes control of element memory (need to free it when done)
  return res;
}

DfResult dflist_s_insert_at(DfList_S *list, void *element, size_t index)
{
  DfResult res;
  res.value = NULL;

  if (!list)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  if (index > list->length)
  {
    res.error = DF_ERR_INDEX_OUT_OF_BOUNDS;
    return res;
  }

  if (index == 0)
  {
    DfResult push_front_res = dflist_s_push_front(list, element);
    return push_front_res;
  }

  if (index == list->length)
  {
    DfResult push_back_res = dflist_s_push_back(list, element);
    return push_back_res;
  }

  DfResult new_node_res = dflist_s_create_node(element);
  DfList_S_Node *new_node = (DfList_S_Node *)new_node_res.value;
  if (new_node_res.error != DF_OK)
  {
    return new_node_res;
  }

  size_t i = 0;
  DfList_S_Node *cur = list->head;

  for (size_t i = 0; i < index - 1; i++)
  {
    cur = cur->next;
  }

  new_node->next = cur->next;
  cur->next = new_node;
  list->length++;

  res.error = DF_OK;
  return res;
}
