#include "../../includes/df_iterator.h"
#include "../../includes/df_utils.h"
#include "../../includes/df_common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

DfResult df_map(Iterator *it, void *(*func)(void *element))
{
  DfResult res;
  res.value = NULL;

  if (!it)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfResult new_ds_res = it->create_new(it);
  if (new_ds_res.error)
  {
    return new_ds_res;
  }

  void *new_ds = new_ds_res.value;

  while (it->has_next(it))
  {
    DfResult element_res = it->next(it);
    if (element_res.error)
    {
      return element_res;
    }

    void *element = element_res.value;

    void *modified_element = func(element);

    it->insert_new(new_ds, modified_element);
  }

  res.error = DF_OK;
  res.value = new_ds;
  return res;
}

DfResult df_filter(Iterator *it, bool (*func)(void *element))
{
  DfResult res;
  res.value = NULL;
  if (!it)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  DfResult filtered_ds_res = it->create_new(it);
  if (filtered_ds_res.error)
  {
    return filtered_ds_res;
  }

  void *filtered_ds = filtered_ds_res.value;

  while (it->has_next(it))
  {
    DfResult element_res = it->next(it);
    if (element_res.error)
    {
      return element_res;
    }
    void *element = element_res.value;

    if (func(element))
      it->insert_new(filtered_ds, element);
  }

  res.error = DF_OK;
  res.value = filtered_ds;
  return res;
}

DfResult df_find(Iterator *it, bool (*func)(void *element))
{
  if (!it)
  {
    return (DfResult){error : DF_ERR_NULL_PTR, value : NULL};
  }

  DfResult element_res;
  while (it->has_next(it))
  {
    element_res = it->next(it);
    if (element_res.error)
    {
      return element_res;
    }

    if (func(element_res.value))
    {
      return element_res;
    }
  }

  return (DfResult){error : DF_ERR_ELEMENT_NOT_FOUND, value : NULL};
}

DfResult df_for_each(Iterator *it, void (*func)(void *element))
{
  DfResult res;
  res.value = NULL;

  if (!it)
  {
    res.error = DF_ERR_NULL_PTR;
  }

  size_t size = it->elem_size(it);
  void *copy = malloc(size);
  if (!copy)
  {
    res.error = DF_ERR_ALLOC_FAILED;
  }

  while (it->has_next(it))
  {
    DfResult element_res = it->next(it);
    if (element_res.error)
    {
      return element_res;
    }
    void *element = element_res.value;
    memcpy(copy, element, size);
    func(copy);
  }

  free(copy);
  res.error = DF_OK;
  return res;
}

DfResult df_count(Iterator *it, bool (*func)(void *element))
{
  DfResult res;
  res.value = 0;

  if (!it)
  {
    res.error = DF_ERR_NULL_PTR;
  }

  while (it->has_next(it))
  {
    DfResult element_res = it->next(it);
    if (element_res.error)
    {
      return element_res;
    }
    void *element = element_res.value;

    if (func(element))
      (size_t *)res.value++;
  }

  res.error = DF_OK;
  return res;
}

DfResult df_reduce(Iterator *it, void *initial, void (*func)(void *accumulator, void *element))
{
  size_t size = it->elem_size(it);
  DfResult res;
  res.value = NULL;

  if (!it || !initial)
  {
    res.error = DF_ERR_NULL_PTR;
    return res;
  }

  res.value = malloc(size);
  if (!res.value)
  {
    res.value = NULL;
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  memcpy(res.value, initial, size);
  if (!initial)
  {
    res.value = NULL;
    res.error = DF_ERR_ALLOC_FAILED;
    return res;
  }

  while (it->has_next(it))
  {
    DfResult element_res = it->next(it);
    if (element_res.error)
    {
      free(res.value);
      res.value = NULL;
      free(initial);
      initial = NULL;
      return element_res;
    }
    void *element = element_res.value;
    func(res.value, element);
  }

  res.error = DF_OK;
  return res;
}

DfResult df_free_all(Iterator *it)
{
  DfResult res;
  res.value = NULL;

  if (!it)
  {
    res.error = DF_ERR_NULL_PTR;
  }

  it->free_all(it);

  res.error = DF_OK;
  return res;
}
