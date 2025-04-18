// #include <criterion/alloc.h>
// #include <criterion/criterion.h>
// #include <criterion/internal/assert.h>
// #include <criterion/internal/test.h>
// #include <criterion/logging.h>
// #include <stdio.h>
// #include "../../../includes/df_array.h"
// #include "../../../includes/df_iterator.h"
// #include "../../../includes/df_utils.h"

// // Helper funcs
// void *double_value2(void *element)
// {
//   *(int *)element *= 2;
//   return element;
// }

// bool isEven(void *element)
// {
//   return *(int *)element % 2 == 0;
// }

// bool greater_than_10(void *element)
// {
//   return *(int *)element > 10;
// }

// bool greater_than_30(void *element)
// {
//   return *(int *)element > 30;
// }

// void print_num_plus_2(void *element)
// {
//   printf("%d\n", *(int *)element + 2);
// }

// void sum_int(void *acc, void *elem)
// {
//   *(int *)acc += *(int *)elem;
// }

// typedef struct DfArray
// {
//   void *items;
//   size_t length;
//   size_t elem_size;
//   size_t capacity;
// } DfArray;

// // map tests

// Test(generic_utils_suit, map_df_array)
// {
//   DfArray *array = dfarray_create(sizeof(int), 3);
//   int nums[] = {10, 20, 30};
//   for (int i = 0; i < 3; i++)
//   {
//     dfarray_push(array, &nums[i]);
//   }
//   Iterator it = dfarray_iterator_create(array);
//   DfArray *modified_array = (DfArray *)df_map(&it, double_value2);
//   Iterator modified_it = dfarray_iterator_create(modified_array);
//   while (it.has_next(&it))
//   {
//     cr_assert(*(int *)it.next(&it) * 2 == *(int *)modified_it.next(&modified_it), "Expected modified value to be double original value");
//   }
//   iterator_destroy(&it);
//   iterator_destroy(&modified_it);
//   dfarray_destroy(array);
//   dfarray_destroy(modified_array);
// }

// // filter tests

// Test(generic_utils_suit, filter_df_array)
// {
//   DfArray *array = dfarray_create(sizeof(int), 3);
//   int nums[] = {10, 23, 30};
//   for (int i = 0; i < 3; i++)
//   {
//     dfarray_push(array, &nums[i]);
//   }
//   Iterator it = dfarray_iterator_create(array);
//   DfArray *filtered = (DfArray *)df_filter(&it, isEven);
//   int *num;
//   num = (int *)dfarray_get(filtered, 0);
//   cr_assert(*num == 10, "Expected num to equal 10");
//   free(num);
//   num = (int *)dfarray_get(filtered, 1);
//   cr_assert(*num == 30, "Expected num to equal 10");
//   free(num);
//   iterator_destroy(&it);
//   dfarray_destroy(array);
//   dfarray_destroy(filtered);
// }

// // find tests

// Test(generic_utils_suit, find_df_array)
// {
//   DfArray *array = dfarray_create(sizeof(int), 3);
//   int nums[] = {10, 23, 30};
//   for (int i = 0; i < 3; i++)
//   {
//     dfarray_push(array, &nums[i]);
//   }
//   Iterator it = dfarray_iterator_create(array);
//   void *found = df_find(&it, greater_than_10);
//   cr_assert(*(int *)found == 23, "Expected first element to meet condition to be 23");
//   found = df_find(&it, greater_than_30);
//   cr_assert(found == NULL, "Expected find to be NULL");
//   iterator_destroy(&it);
//   dfarray_destroy(array);
//   free(found);
// }

// // forEach tests

// Test(generic_utils_suit, for_each_df_array)
// {
//   DfArray *array = dfarray_create(sizeof(int), 3);
//   int nums[] = {10, 23, 30};
//   for (int i = 0; i < 3; i++)
//   {
//     dfarray_push(array, &nums[i]);
//   }
//   Iterator it = dfarray_iterator_create(array);
//   int *num;
//   num = (int *)dfarray_get(array, 0);
//   cr_assert(*num == 10, "Expected num to equal 10");
//   free(num);
//   df_for_each(&it, print_num_plus_2);
//   num = (int *)dfarray_get(array, 0);
//   cr_assert(*num == 10, "Expected num to still equal 10");
//   free(num);
//   iterator_destroy(&it);
//   dfarray_destroy(array);
// }

// // count tests

// Test(generic_utils_suit, count_df_array)
// {
//   DfArray *array = dfarray_create(sizeof(int), 3);
//   int nums[] = {10, 23, 30};
//   for (int i = 0; i < 3; i++)
//   {
//     dfarray_push(array, &nums[i]);
//   }
//   Iterator it = dfarray_iterator_create(array);
//   size_t count = df_count(&it, isEven);
//   cr_assert(count == 2, "Expected count to be equal to 2");
//   iterator_destroy(&it);
//   dfarray_destroy(array);
// }

// Test(generic_utils_suit, reduce_df_array)
// {
//   DfArray *array = dfarray_create(sizeof(int), 3);
//   int nums[] = {10, 23, 30};
//   for (int i = 0; i < 3; i++)
//   {
//     dfarray_push(array, &nums[i]);
//   }
//   Iterator it = dfarray_iterator_create(array);
//   int initial = 0;
//   int *reduced = (int *)df_reduce(&it, &initial, sum_int);
//   cr_assert(*reduced == 63, "Expected reduced sum to be 63");
//   free(reduced);
//   iterator_destroy(&it);
//   dfarray_destroy(array);
// }

// Test(generic_utils_suit, free_all_df_array)
// {
//   DfArray *array = dfarray_create(sizeof(int), 3);
//   int nums[] = {10, 23, 30};
//   for (int i = 0; i < 3; i++)
//   {
//     dfarray_push(array, &nums[i]);
//   }
//   Iterator it = dfarray_iterator_create(array);
//   df_free_all(&it);
//   DfArray *test = (DfArray *)it.structure;
//   cr_assert(test->items == NULL);
//   iterator_destroy(&it);
//   dfarray_destroy(array);
// }
