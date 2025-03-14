#include <criterion/alloc.h>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/logging.h>
#include "../includes/df_array.h"
#include "../includes/df_iterator.h"
#include "../includes/df_utils.h"

// Helper funcs
void *double_value2(void *element) {
    *(int *)element *= 2;
    return element;
}

// map tests

Test(generic_utils, map_df_array) {
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int nums[] = {10, 20, 30};
  for(int i = 0; i < 3; i++) {
    DfArray_Push(array, &nums[i]);
  }
  Iterator it = DfArray_Iterator_Create(array);
  DfArray *modified_array = (DfArray *)DfMap(&it, double_value2);
  Iterator modified_it = DfArray_Iterator_Create(modified_array);
  while(it.has_next(&it)){
    cr_assert(*(int *)it.next(&it) * 2 == *(int *)modified_it.next(&modified_it), "Expected modified value to be double original value");
  }
  Iterator_Destroy(&it);
  Iterator_Destroy(&modified_it);
  DfArray_Destroy(array);
  DfArray_Destroy(modified_array);
}


