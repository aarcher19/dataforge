#include "../../includes/df_iterator.h"
#include "../../includes/df_utils.h"

void *DfMap(Iterator *it, void *(*func)(void *element)) {
  void *new_ds = it->create_new(it);

  while(it->has_next(it)) {
    void *element = it->next(it);
    void *modified_element = func(element);

    it->insert_new(new_ds, modified_element);
  }

  return new_ds;
}

