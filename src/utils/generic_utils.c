#include "../../includes/df_iterator.h"
#include "../../includes/df_utils.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void *df_map(Iterator *it, void *(*func)(void *element))
{
  void *new_ds = it->create_new(it);

  while (it->has_next(it))
  {
    void *element = it->next(it);
    void *modified_element = func(element);

    it->insert_new(new_ds, modified_element);
  }

  return new_ds;
}

void *df_filter(Iterator *it, bool (*func)(void *element))
{
  void *filtered_ds = it->create_new(it);

  while (it->has_next(it))
  {
    void *element = it->next(it);

    if (func(element))
      it->insert_new(filtered_ds, element);
  }
  return filtered_ds;
}

void *df_find(Iterator *it, bool (*func)(void *element))
{
  void *element;
  while (it->has_next(it))
  {
    element = it->next(it);
    if (func(element))
      return element;
  }
  return NULL;
}

void df_for_each(Iterator *it, void (*func)(void *element))
{
  size_t size = it->elem_size(it);
  void *copy = malloc(size);

  while (it->has_next(it))
  {
    void *element = it->next(it);
    memcpy(copy, element, size);
    func(copy);
  }

  free(copy);
}

size_t df_count(Iterator *it, bool (*func)(void *element))
{
  size_t count = 0;

  while (it->has_next(it))
  {
    void *element = it->next(it);
    if (func(element))
      count++;
  }

  return count;
}

void *df_reduce(Iterator *it, void *initial, void (*func)(void *accumulator, void *element))
{
  size_t size = it->elem_size(it);
  void *result = malloc(size);
  memcpy(result, initial, size);

  while (it->has_next(it))
  {
    void *element = it->next(it);
    func(result, element);
  }

  return result;
}

void df_free_all(Iterator *it)
{
  it->free_all(it);
}
