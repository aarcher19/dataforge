#include <criterion/alloc.h>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/logging.h>
#include <stdio.h>
#include "../../../includes/df_array.h"
#include "../../../includes/df_iterator.h"

typedef struct DfArray
{
  void *items;
  size_t length;
  size_t elem_size;
  size_t capacity;
} DfArray;

// Helper functions
void double_value(void *element)
{
  *(int *)element *= 2;
}

Test(df_array_suit, create)
{
  DfArray *array = dfarray_create(sizeof(int), 10);
  cr_assert(array != NULL, "Expected array to not be NULL");
  cr_assert(array->items != NULL, "Expected items to not be NULL");
  cr_assert(array->length == 0, "Expected length to be 0");
  cr_assert(array->capacity == 10, "Expected capacity to be 10");
  cr_assert(array->elem_size == sizeof(int), "Expected elem_size to be sizeof(int)");
  dfarray_destroy(array);
}

Test(df_array_suit, resize)
{
  DfArray *array = dfarray_create(sizeof(int), 2);
  int val1 = 1, val2 = 2, val3 = 3, val4 = 4;
  dfarray_push(array, &val1);
  dfarray_push(array, &val2);
  size_t old_capacity = array->capacity;
  dfarray_push(array, &val3);
  dfarray_push(array, &val4);
  cr_assert(array->capacity > old_capacity, "Expected capacity to increase");
  cr_assert(array->length == 4, "Expected length to increase");
  dfarray_destroy(array);
}

Test(df_array_suit, shrink)
{
  DfArray *array = dfarray_create(sizeof(int), 4);
  int val1 = 1, val2 = 2;
  dfarray_push(array, &val1);
  dfarray_push(array, &val2);
  int *dest = (int *)dfarray_pop(array);
  free(dest);
  cr_assert(array->capacity == array->length, "Expected capacity to match length");
  dest = (int *)dfarray_shift(array);
  free(dest);
  cr_assert(array->length == 0, "Expected length to be 0 but it is %zu", array->length);
  cr_assert(array->items == NULL, "Expected items to be null");
  cr_assert(array->capacity == 0, "Expected capacity to be 0 but was %zu", array->capacity);
  dfarray_destroy(array);
}

Test(df_array_suit, set)
{
  DfArray *array = dfarray_create(sizeof(int), 2);
  int val1 = 10, val2 = 20;
  dfarray_push(array, &val1);
  dfarray_push(array, &val2);
  int newVal = 30;
  dfarray_set(array, 0, &newVal);
  int *check = (int *)dfarray_get(array, 0);
  cr_assert(*check == 30, "Expected check to equal 30");
  free(check);
  dfarray_destroy(array);
}

Test(df_array_suit, push)
{
  DfArray *array = dfarray_create(sizeof(int), 2);
  int val1 = 10, val2 = 20, val3 = 30;
  dfarray_push(array, &val1);
  dfarray_push(array, &val2);
  cr_assert(array->length == 2, "Expected length to be 2");
  dfarray_push(array, &val3);
  cr_assert(array->length == 3, "Expected length to be 3");
  cr_assert(array->capacity >= 3, "Expected capacity to be 3");
  dfarray_destroy(array);
}

Test(df_array_suit, get)
{
  DfArray *array = dfarray_create(sizeof(int), 3);
  int val1 = 5, val2 = 15, val3 = 25;
  int *retrieved;
  dfarray_push(array, &val1);
  dfarray_push(array, &val2);
  dfarray_push(array, &val3);
  retrieved = (int *)dfarray_get(array, 0);
  cr_assert(*retrieved == val1, "Expected retrieved to be equal to val1");
  free(retrieved);
  retrieved = (int *)dfarray_get(array, 1);
  cr_assert(*retrieved == val2, "Expected retrieved to be equal to val2");
  free(retrieved);
  retrieved = (int *)dfarray_get(array, 2);
  cr_assert(*retrieved == val3, "Expected retrieved to be equal to val3");
  free(retrieved);
  dfarray_destroy(array);
}

Test(df_array_suit, pop)
{
  DfArray *array = dfarray_create(sizeof(int), 3);
  int val1 = 100, val2 = 200, val3 = 300;
  int *retrieved;

  dfarray_push(array, &val1);
  dfarray_push(array, &val2);
  dfarray_push(array, &val3);

  retrieved = (int *)dfarray_pop(array);
  cr_assert(*retrieved == val3, "Expected retrived to be equal to val3");
  free(retrieved);
  cr_assert(array->length == 2, "Expected length to have decreased");

  retrieved = (int *)dfarray_pop(array);
  cr_assert(*retrieved == val2, "Expected retrieved to be equal to val2");
  free(retrieved);
  cr_assert(array->length == 1, "Expected length to have decreased");

  retrieved = (int *)dfarray_pop(array);
  cr_assert(*retrieved == val1, "Expected retrieved to be equal to val1");
  free(retrieved);
  cr_assert(array->length == 0, "Expected length to have decreased");
  dfarray_destroy(array);
}

Test(df_array_suit, shift)
{
  DfArray *array = dfarray_create(sizeof(int), 4);
  int nums[] = {10, 20, 30, 40, 50};
  for (int i = 0; i < 5; i++)
  {
    dfarray_push(array, &nums[i]);
  }
  int *first = (int *)dfarray_shift(array);
  cr_assert(*first == 10, "Expected first to equel 10");
  free(first);
  dfarray_destroy(array);
}

Test(df_array_suit, unshift)
{
  DfArray *array = dfarray_create(sizeof(int), 2);
  int nums[] = {10, 20, 30};
  for (int i = 0; i < 3; i++)
  {
    dfarray_unshift(array, &nums[i]);
  }
  int *first = (int *)dfarray_shift(array);
  cr_assert(*first == 30, "Expected first to equel 30 but is %d instead", *first);
  dfarray_destroy(array);
}

Test(df_array_suit, insertAt)
{
  DfArray *array = dfarray_create(sizeof(int), 5);
  int nums[] = {10, 20, 30, 40, 50};
  for (int i = 0; i < 5; i++)
  {
    dfarray_push(array, &nums[i]);
  }
  int num = 60;
  dfarray_insert_at(array, 1, &num);
  int *retrieved = (int *)dfarray_get(array, 1);
  cr_assert(*retrieved == num, "Expected retrived to be equel to num but got %d instead", *retrieved);
  dfarray_insert_at(array, 6, retrieved);
  int *popped = (int *)dfarray_pop(array);
  cr_assert(*popped == *retrieved, "Expected popped to be equel to retrieved");
  dfarray_destroy(array);
  free(popped);
  free(retrieved);
}

Test(df_array_suit, removeAt)
{
  DfArray *array = dfarray_create(sizeof(int), 5);
  int nums[] = {10, 20, 30, 40, 50};
  for (int i = 0; i < 5; i++)
  {
    dfarray_push(array, &nums[i]);
  }
  dfarray_remove_at(array, 3);
  int *num = (int *)dfarray_get(array, 3);
  cr_assert(*num == 50, "Expected 40 to be removed and 50 to be in its place");
  free(num);
  dfarray_remove_at(array, 3);
  num = (int *)dfarray_pop(array);
  cr_assert(*num == 30, "Expected 50 to be removed and 30 to be popped off");
  free(num);
  dfarray_destroy(array);
}

Test(df_array_suit, iterator)
{
  DfArray *array = dfarray_create(sizeof(int), 3);
  int nums[] = {10, 20, 30};
  for (int i = 0; i < 3; i++)
  {
    dfarray_push(array, &nums[i]);
  }
  int j = 0;
  Iterator it = dfarray_iterator_create(array);
  while (it.has_next(&it))
  {
    cr_assert(*(int *)it.next(&it) == nums[j], "Expected value to be %d", nums[j]);
    j++;
  };
  iterator_destroy(&it);
  cr_assert(it.current == NULL, "Iterator not freed properly");
  dfarray_destroy(array);
}

Test(df_array_suit, create_new_and_insert_new)
{
  DfArray *original = dfarray_create(sizeof(int), 5);

  int values[] = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; i++)
  {
    dfarray_push(original, &values[i]);
  }

  Iterator it = dfarray_iterator_create(original);

  DfArray *new_array = (DfArray *)it.create_new(&it);
  cr_assert_not_null(new_array, "create_new() should return a valid DfArray pointer.");
  cr_assert_eq(new_array->length, 0, "New array should start with length 0.");
  cr_assert_eq(new_array->capacity, original->capacity, "New array should have the same initial capacity.");
  cr_assert_eq(new_array->elem_size, original->elem_size, "New array should have the same element size.");

  while (it.has_next(&it))
  {
    void *element = it.next(&it);
    it.insert_new(new_array, element);
  }

  cr_assert_eq(new_array->length, original->length, "New array should have the same length as the original.");

  for (size_t i = 0; i < original->length; i++)
  {
    int *original_value, *new_value;
    original_value = (int *)dfarray_get(original, i);
    new_value = (int *)dfarray_get(new_array, i);
    cr_assert_eq(*original_value, *new_value, "Elements at index %zu should be equal in both arrays.", i);
    free(original_value);
    free(new_value);
  }

  dfarray_destroy(original);
  dfarray_destroy(new_array);
}

Test(df_array_suit, map)
{
  DfArray *array = dfarray_create(sizeof(int), 5);
  for (int i = 1; i <= 5; i++)
  {
    dfarray_push(array, &i);
  }
  dfarray_map(array, double_value);
  for (size_t i = 0; i < array->length; i++)
  {
    int *value = (int *)dfarray_get(array, i);
    cr_assert(*value == (int)((i + 1) * 2), "Expected value to be doubled");
    free(value);
  }
  dfarray_destroy(array);
}
