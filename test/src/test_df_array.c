#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/logging.h>
#include "../../includes/df_array.h"

typedef struct df_array {
  void *items;
  size_t length;
  size_t elem_size;
  size_t capacity;
} DfArray;

// Helper functions
void double_value(void *element) {
    *(int *)element *= 2;
}

Test(df_array_suit, create) {
  DfArray *array = DfArray_Create(sizeof(int), 10);
  cr_assert(array != NULL, "Expected array to not be NULL");
  cr_assert(array->items != NULL, "Expected items to not be NULL");
  cr_assert(array->length == 0, "Expected length to be 0");
  cr_assert(array->capacity == 10, "Expected capacity to be 10");
  cr_assert(array->elem_size == sizeof(int), "Expected elem_size to be sizeof(int)");
  DfArray_Destroy(array);
}

Test(df_array_suit, resize) {
  DfArray *array = DfArray_Create(sizeof(int), 2);
  int val1 = 1, val2 = 2, val3 = 3, val4 = 4;
  DfArray_Push(array, &val1);
  DfArray_Push(array, &val2);
  size_t old_capacity = array->capacity;
  DfArray_Push(array, &val3);
  DfArray_Push(array, &val4);
  cr_assert(array->capacity > old_capacity, "Expected capacity to increase");
  cr_assert(array->length == 4, "Expected length to increase");
  DfArray_Destroy(array);
}

Test(df_array_suit, push) {
  DfArray *array = DfArray_Create(sizeof(int), 2);
  int val1 = 10, val2 = 20, val3 = 30;
  DfArray_Push(array, &val1);
  DfArray_Push(array, &val2);
  cr_assert(array->length == 2, "Expected length to be 2");
  DfArray_Push(array, &val3);
  cr_assert(array->length == 3, "Expected length to be 3");
  cr_assert(array->capacity >= 3, "Expected capacity to be 3");
  DfArray_Destroy(array);
}

Test(df_array_suit, get) {
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int val1 = 5, val2 = 15, val3 = 25, retrieved;
  DfArray_Push(array, &val1);
  DfArray_Push(array, &val2);
  DfArray_Push(array, &val3);
  DfArray_Get(array, 0, &retrieved);
  cr_assert(retrieved == val1, "Expected retrieved to be equal to val1");
  DfArray_Get(array, 1, &retrieved);
  cr_assert(retrieved == val2, "Expected retrieved to be equal to val2");
  DfArray_Get(array, 2, &retrieved);
  cr_assert(retrieved == val3, "Expected retrieved to be equal to val3");
  DfArray_Destroy(array);
}

Test(df_array_suit, pop) {
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int val1 = 100, val2 = 200, val3 = 300, retrieved;

  DfArray_Push(array, &val1);
  DfArray_Push(array, &val2);
  DfArray_Push(array, &val3);

  DfArray_Pop(array, &retrieved);
  cr_assert(retrieved == val3, "Expected retrived to be equal to val3");
  cr_assert(array->length == 2, "Expected length to have decreased");

  DfArray_Pop(array, &retrieved);
  cr_assert(retrieved == val2, "Expected retrieved to be equal to val2");
  cr_assert(array->length == 1, "Expected length to have decreased");

  DfArray_Pop(array, &retrieved);
  cr_assert(retrieved == val1, "Expected retrieved to be equal to val1");
  cr_assert(array->length == 0, "Expected length to have decreased");
  DfArray_Destroy(array);
}

Test(df_array_suit, map) {
  DfArray *array = DfArray_Create(sizeof(int), 5);
  for (int i = 1; i <= 5; i++) {
      DfArray_Push(array, &i);
  }
  DfArray_Map(array, double_value);
  for (size_t i = 0; i < array->length; i++) {
      int value;
      DfArray_Get(array, i, &value);
      cr_assert(value == (int)((i + 1) * 2), "Expected value to be doubled");  
  }
  DfArray_Destroy(array);
}


