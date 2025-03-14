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

bool isEven(void *element) {
  return *(int *)element % 2 == 0;
}

bool greater_than_10(void *element) {
  return *(int *)element > 10;
}

bool greater_than_30(void *element) {
  return *(int *)element > 30;
}


// map tests

Test(generic_utils_suit, map_df_array) {
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

// filter tests

Test(generic_utils_suit, filter_df_array) {
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int nums[] = {10, 23, 30};
  for(int i = 0; i < 3; i++) {
    DfArray_Push(array, &nums[i]);
  }
  Iterator it = DfArray_Iterator_Create(array);
  DfArray *filtered = (DfArray *)DfFilter(&it, isEven);
  int num;
  DfArray_Get(filtered, 0, &num); 
  cr_assert(num == 10, "Expected num to equal 10");
  DfArray_Get(filtered, 1, &num); 
  cr_assert(num == 30, "Expected num to equal 10");
  Iterator_Destroy(&it);
  DfArray_Destroy(array);
  DfArray_Destroy(filtered);
}

// find tests

Test(generic_utils_suit, find_df_array) {
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int nums[] = {10, 23, 30};
  for(int i = 0; i < 3; i++) {
    DfArray_Push(array, &nums[i]);
  }
  Iterator it = DfArray_Iterator_Create(array);
  void *found = DfFind(&it, greater_than_10);
  cr_assert(*(int *)found == 23, "Expected first element to meet condition to be 23");
  found = DfFind(&it, greater_than_30);
  cr_assert(found == NULL, "Expected find to be NULL");
  Iterator_Destroy(&it);
  DfArray_Destroy(array);
  free(found);
}
