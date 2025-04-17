#include "../includes/df_list_s.h"
#include "../includes/df_common.h"
#include "../internal/df_internal.h"
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
  DfResult res = df_result_init();

  DfList_S *list = malloc(sizeof(DfList_S));
  if (!list)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  list->head = list->tail = NULL;
  list->length = 0;

  res.value = list;
  return res;
}

DfResult dflist_s_destroy(DfList_S *list, void (*cleanup)(void *element))
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
    return res;
  }

  DfList_S_Node *current = list->head;
  while (current)
  {
    DfList_S_Node *next = current->next;
    if (cleanup)
    {
      cleanup(current->element);
    }
    free(current);
    current = next;
  }

  free(list);

  return res;
}

DfResult dflist_s_create_node(void *element)
{
  DfResult res = df_result_init();

  df_null_ptr_check(element, &res);
  if (res.error)
  {
    return res;
  }

  DfList_S_Node *new_node = malloc(sizeof(DfList_S_Node));
  if (!new_node)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  new_node->element = element;
  new_node->next = NULL;

  res.value = new_node;
  return res;
}

DfResult dflist_s_push_back(DfList_S *list, void *element)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  df_null_ptr_check(element, &res);
  if (res.error)
  {
    return res;
  }

  DfResult new_node_res = dflist_s_create_node(element);
  if (new_node_res.error)
  {
    return new_node_res;
  }

  DfList_S_Node *new_node = (DfList_S_Node *)new_node_res.value;

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

  return res;
}

DfResult dflist_s_push_front(DfList_S *list, void *element)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  df_null_ptr_check(element, &res);
  if (res.error)
  {
    return res;
  }

  DfResult new_node_res = dflist_s_create_node(element);
  if (new_node_res.error)
  {
    return new_node_res;
  }

  DfList_S_Node *new_node = (DfList_S_Node *)new_node_res.value;

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

  return res;
}

DfResult dflist_s_pop_front(DfList_S *list)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
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

  res.value = dest;
  // Ensure to document user takes control of element memory (need to free it when done)
  return res;
}

DfResult dflist_s_pop_back(DfList_S *list)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
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

  res.value = dest;
  // Ensure to document user takes control of element memory (need to free it when done)
  return res;
}

DfResult dflist_s_insert_at(DfList_S *list, void *element, size_t index)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  df_null_ptr_check(element, &res);
  if (res.error)
  {
    return res;
  }

  df_index_check_insert(index, list->length, &res);
  if (res.error)
  {
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
  if (new_node_res.error != DF_OK)
  {
    return new_node_res;
  }

  DfList_S_Node *new_node = (DfList_S_Node *)new_node_res.value;
  DfList_S_Node *cur = list->head;

  for (size_t i = 0; i < index - 1; i++)
  {
    cur = cur->next;
  }

  new_node->next = cur->next;
  cur->next = new_node;
  list->length++;

  return res;
}

DfResult dflist_s_remove_at(DfList_S *list, size_t index)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
    return res;
  }

  df_index_check_access(index, list->length, &res);
  if (res.error)
  {
    return res;
  }

  if (index == 0)
  {
    DfResult pop_front_res = dflist_s_pop_front(list);
    return pop_front_res;
  }

  if (index == list->length - 1)
  {
    DfResult pop_back_res = dflist_s_pop_back(list);
    return pop_back_res;
  }

  DfList_S_Node *cur = list->head;
  for (size_t i = 0; i < index - 1; i++)
  {
    cur = cur->next;
  }

  DfList_S_Node *old = cur->next;
  cur->next = old->next;
  res.value = old->element;
  free(old);
  list->length--;

  return res;
}

DfResult dflist_s_get(DfList_S *list, size_t index)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
    return res;
  }

  df_index_check_access(index, list->length, &res);
  if (res.error)
  {
    return res;
  }

  DfList_S_Node *target = list->head;
  for (size_t i = 0; i < index; i++)
  {
    target = target->next;
  }

  res.value = target->element;
  return res;
}

DfResult dflist_s_peek_front(DfList_S *list)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
    return res;
  }

  if (!list->head)
  {
    res.error = DF_ERR_EMPTY;
    return res;
  }

  res.value = list->head->element;
  return res;
}

DfResult dflist_s_peek_back(DfList_S *list)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
    return res;
  }

  if (!list->tail)
  {
    res.error = DF_ERR_EMPTY;
    return res;
  }

  res.value = list->tail->element;
  return res;
}

DfResult dflist_s_length(DfList_S *list)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
    return res;
  }

  res.value = list->length;
  return res;
}

// Iterator

typedef struct DfList_S_Iterator
{
  DfList_S *list;
  DfList_S_Node *cur;
} DfList_S_Iterator;

int dflist_s_iterator_has_next(Iterator *it)
{
  DfList_S_Iterator *list_it = (DfList_S_Iterator *)it->current;
  return list_it->cur != NULL && list_it->cur->next != NULL;
}

DfResult dflist_s_iterator_next(Iterator *it)
{
  DfResult res = df_result_init();

  df_null_ptr_check(it, &res);
  df_null_ptr_check(it->current, &res);
  if (res.error)
  {
    return res;
  }

  DfList_S_Iterator *list_it = (DfList_S_Iterator *)it->current;

  if (!list_it->cur || !list_it->cur->next)
  {
    res.error = DF_ERR_END_OF_LIST;
    return res;
  }

  list_it->cur = list_it->cur->next;
  res.value = list_it->cur->element;
  return res;
}

DfResult dflist_s_free_all(Iterator *it)
{
  DfResult res = df_result_init();

  df_null_ptr_check(it, &res);
  df_null_ptr_check(it->structure, &res);
  if (res.error)
  {
    return res;
  }

  DfList_S *list = (DfList_S *)it->structure;

  if (!list->head)
  {
    res.error = DF_ERR_ALREADY_FREED;
    return res;
  }

  DfList_S_Node *cur = list->head;
  while (cur)
  {
    DfList_S_Node *temp = cur->next;
    free(cur);
    cur = temp;
  }

  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return res;
}

DfResult dflist_s_iterator_create(DfList_S *list)
{
  DfResult res = df_result_init();

  df_null_ptr_check(list, &res);
  if (res.error)
  {
    return res;
  }

  DfList_S_Iterator *list_it = malloc(sizeof(DfList_S_Iterator));
  if (!list_it)
  {
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  list_it->list = list;
  list_it->cur = list->head;

  DfResult it_res = iterator_create();
  if (it_res.error)
  {
    free(list_it);
    return it_res;
  }

  Iterator *it = (Iterator *)it_res.value;

  it->structure = list;
  it->current = list_it;
  it->next = dflist_s_iterator_next;
  it->has_next = dflist_s_iterator_has_next;
  it->create_new = dflist_s_create;
  it->insert_new = dflist_s_push_back;
  it->free_all = dflist_s_free_all;

  res.value = it;
  return res;
}