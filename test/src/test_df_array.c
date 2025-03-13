#include <criterion/alloc.h>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <dataforge/df_array.h>
#include <dataforge/df_iterator.h>

typedef struct DfArray {
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

Test(df_array_suit, shrink) {
  DfArray *array = DfArray_Create(sizeof(int), 4);
  int val1 = 1, val2 = 2;
  DfArray_Push(array, &val1);
  DfArray_Push(array, &val2);
  int dest;
  DfArray_Pop(array, &dest);
  cr_assert(array->capacity == array->length, "Expected capacity to match length");
  DfArray_Shift(array, &dest);
  cr_assert(array->length == 0, "Expected length to be 0 but it is %zu", array->length);
  cr_assert(array->items == NULL, "Expected items to be null");
  cr_assert(array->capacity == 0, "Expected capacity to be 0 but was %zu", array->capacity);
  DfArray_Destroy(array);
}

Test(df_array_suit, set) {
  DfArray *array = DfArray_Create(sizeof(int), 2);
  int val1 = 10, val2 = 20;
  DfArray_Push(array, &val1);
  DfArray_Push(array, &val2);
  int newVal = 30;
  DfArray_Set(array, 0, &newVal);
  int check;
  DfArray_Get(array, 0, &check);
  cr_assert(check == 30, "Expected check to equal 30");
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

Test(df_array_suit, shift) {
  DfArray *array = DfArray_Create(sizeof(int), 4);
  int nums[] = {10, 20, 30, 40, 50};
  for(int i = 0; i < 5; i++){
    DfArray_Push(array, &nums[i]);
  }
  int first;
  DfArray_Shift(array, &first);
  cr_assert(first == 10, "Expected first to equel 10");
  DfArray_Destroy(array);
}

Test(df_array_suit, unshift) {
  DfArray *array = DfArray_Create(sizeof(int), 2);
  int nums[] = {10, 20, 30};
  for(int i = 0; i < 3; i++) {
    DfArray_Unshift(array, &nums[i]);
  }
  int first;
  DfArray_Shift(array, &first);
  cr_assert(first == 30, "Expected first to equel 30 but is %d instead", first);
  DfArray_Destroy(array);
}

Test(df_array_suit, insertAt) {
  DfArray *array = DfArray_Create(sizeof(int), 5);
  int nums[] = {10, 20, 30, 40, 50};
  for(int i = 0; i < 5; i++){
    DfArray_Push(array, &nums[i]);
  }
  int num = 60;
  DfArray_InsertAt(array, 1, &num);
  int retrived;
  DfArray_Get(array, 1, &retrived);
  cr_assert(retrived == num, "Expected retrived to be equel to num but got %d instead", retrived);
  DfArray_InsertAt(array, 6, &retrived);
  int popped;
  DfArray_Pop(array, &popped);
  cr_assert(popped == retrived, "Expected popped to be equel to retrieved");
  DfArray_Destroy(array);
}

Test(df_array_suit, removeAt) {
  DfArray *array = DfArray_Create(sizeof(int), 5);
  int nums[] = {10, 20, 30, 40, 50};
  for(int i = 0; i < 5; i++){
    DfArray_Push(array, &nums[i]);
  }
  DfArray_RemoveAt(array, 3);
  int num;
  DfArray_Get(array, 3, &num);
  cr_assert(num == 50, "Expected 40 to be removed and 50 to be in its place");
  DfArray_RemoveAt(array, 3);
  DfArray_Pop(array, &num);
  cr_assert(num == 30, "Expected 50 to be removed and 30 to be popped off");
  DfArray_Destroy(array);
}

Test(df_array_suit, iterator) {
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int nums[] = {10, 20, 30};
  for(int i = 0; i < 3; i++){
    DfArray_Push(array, &nums[i]);
  }
  int j = 0;
  Iterator it = DfArray_Iterator_Create(array);
  while(it.has_next(&it)){
    cr_assert(*(int *)it.next(&it) == nums[j], "Expected value to be %d", nums[j]);
    j++;
  };
  Iterator_Destroy(&it);
  cr_assert(it.current == NULL, "Iterator not freed properly");
  DfArray_Destroy(array);
}

Test(df_array_suit, create_new_and_insert_new) {
    DfArray *original = DfArray_Create(sizeof(int), 5);
    
    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        DfArray_Push(original, &values[i]);
    }

    Iterator it = DfArray_Iterator_Create(original);

    DfArray *new_array = (DfArray *)it.create_new(&it);
    cr_assert_not_null(new_array, "create_new() should return a valid DfArray pointer.");
    cr_assert_eq(new_array->length, 0, "New array should start with length 0.");
    cr_assert_eq(new_array->capacity, original->capacity, "New array should have the same initial capacity.");
    cr_assert_eq(new_array->elem_size, original->elem_size, "New array should have the same element size.");

    while (it.has_next(&it)) {
      void *element = it.next(&it);
      it.insert_new(new_array, element);
    }

    cr_assert_eq(new_array->length, original->length, "New array should have the same length as the original.");

    for (size_t i = 0; i < original->length; i++) {
        int original_value, new_value;
        DfArray_Get(original, i, &original_value);
        DfArray_Get(new_array, i, &new_value);
        cr_assert_eq(original_value, new_value, "Elements at index %zu should be equal in both arrays.", i);
    }

    DfArray_Destroy(original);
    DfArray_Destroy(new_array);
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


