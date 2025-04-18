#include <criterion/alloc.h>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/logging.h>
#include <stdio.h>
#include "../../../includes/df_array.h"
#include "../../../includes/df_iterator.h"
#include "../../../includes/df_common.h"
#include "../../../internal/df_internal.h"

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

Test(df_array_suit, creates_array_successfully)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 10;

  DfResult res = dfarray_create(elem_size, capacity);
  cr_assert_eq(res.error, DF_OK, "Expected DF_OK but got error code %d", res.error);

  DfArray *arr = res.value;
  cr_assert_not_null(arr, "Expected non-null DfArray pointer");
  cr_assert_not_null(arr->items, "Expected non-null items pointer");
  cr_assert_eq(arr->length, 0, "Expected length to be 0");
  cr_assert_eq(arr->capacity, capacity, "Expected capacity to be %zu", capacity);
  cr_assert_eq(arr->elem_size, elem_size, "Expected elem_size to be %zu", elem_size);

  // Cleanup
  free(arr->items);
  free(arr);
}

Test(df_array_suit, destroys_valid_array)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK, "Array creation failed unexpectedly");

  DfArray *arr = create_res.value;

  DfResult destroy_res = dfarray_destroy(arr);
  cr_assert_eq(destroy_res.error, DF_OK, "Expected DF_OK when destroying valid array");
}

Test(df_array_suit, resizes_array_with_non_zero_capacity)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 4;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK, "Array creation failed");

  DfArray *arr = create_res.value;
  cr_assert_eq(arr->capacity, capacity, "Initial capacity mismatch");

  DfResult resize_res = dfarray_resize(arr);
  cr_assert_eq(resize_res.error, DF_OK, "Resize failed");

  cr_assert_eq(arr->capacity, capacity * 2, "Expected capacity to double");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, resizes_array_with_zero_capacity)
{
  DfArray *arr = malloc(sizeof(DfArray));
  cr_assert_not_null(arr, "Failed to allocate DfArray struct");

  arr->capacity = 0;
  arr->length = 0;
  arr->elem_size = sizeof(int);
  arr->items = NULL;

  DfResult resize_res = dfarray_resize(arr);
  cr_assert_eq(resize_res.error, DF_OK, "Resize with zero capacity failed");
  cr_assert_eq(arr->capacity, 5, "Expected default capacity to be 5");
  cr_assert_not_null(arr->items, "Expected items to be allocated");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, shrinks_array_when_length_is_zero)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 10;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;

  DfResult shrink_res = dfarray_shrink(arr);
  cr_assert_eq(shrink_res.error, DF_OK, "Shrink failed with length 0");
  cr_assert_eq(arr->capacity, 0, "Expected capacity to be 0");
  cr_assert_null(arr->items, "Expected items to be NULL");

  // Cleanup
  free(arr);
}

Test(df_array_suit, shrinks_array_to_match_length)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 10;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 4; // Simulate data
  cr_assert_lt(arr->length, arr->capacity);

  DfResult shrink_res = dfarray_shrink(arr);
  cr_assert_eq(shrink_res.error, DF_OK, "Shrink failed");
  cr_assert_eq(arr->capacity, arr->length, "Capacity should match length after shrink");
  cr_assert_not_null(arr->items, "Items should not be NULL after shrinking to non-zero length");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, sets_element_successfully)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 3; // Simulate data

  // Fill the array with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Example values: 1, 2, 3
  }

  int new_value = 42;
  DfResult set_res = dfarray_set(arr, 1, &new_value);
  cr_assert_eq(set_res.error, DF_OK, "Expected DF_OK when setting a valid index");

  cr_assert_eq(((int *)arr->items)[1], 42, "Expected value at index 1 to be updated to 42");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, returns_error_for_invalid_index_on_set)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 3; // Simulate data

  int new_value = 42;
  DfResult set_res = dfarray_set(arr, 5, &new_value); // Invalid index
  cr_assert_eq(set_res.error, DF_ERR_INDEX_OUT_OF_BOUNDS, "Expected DF_ERR_INDEX_OUT_OF_BOUNDS for invalid index");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, pushes_element_to_full_array_and_resizes)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 3; // Small capacity to trigger resize

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 3; // Fill the array

  int new_value = 42;
  DfResult push_res = dfarray_push(arr, &new_value);
  cr_assert_eq(push_res.error, DF_OK, "Push failed with valid array");

  cr_assert_eq(arr->length, 4, "Expected length to be 4 after push");
  cr_assert_eq(arr->capacity, 6, "Expected capacity to be resized (doubled)");

  // Verify the new value was added
  cr_assert_eq(((int *)arr->items)[3], 42, "Expected value at index 3 to be 42");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, pushes_element_to_non_full_array)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 2; // Fill the array with 2 elements

  int new_value = 42;
  DfResult push_res = dfarray_push(arr, &new_value);
  cr_assert_eq(push_res.error, DF_OK, "Push failed with valid array");

  cr_assert_eq(arr->length, 3, "Expected length to be 3 after push");
  cr_assert_eq(arr->capacity, capacity, "Expected capacity to remain the same");

  // Verify the new value was added
  cr_assert_eq(((int *)arr->items)[2], 42, "Expected value at index 2 to be 42");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, gets_element_successfully)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 3; // Simulate data

  // Fill the array with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Example values: 1, 2, 3
  }

  DfResult get_res = dfarray_get(arr, 1);
  cr_assert_eq(get_res.error, DF_OK, "Expected DF_OK when accessing a valid index");

  int *value = (int *)get_res.value;
  cr_assert_eq(*value, 2, "Expected value to be 2 at index 1");

  free(get_res.value); // Don't forget to free the memory allocated by dfarray_get

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, returns_error_for_invalid_index)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 3; // Simulate data

  DfResult get_res = dfarray_get(arr, 5); // Invalid index
  cr_assert_eq(get_res.error, DF_ERR_INDEX_OUT_OF_BOUNDS, "Expected DF_ERR_INDEX_OUT_OF_BOUNDS for invalid index");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, pops_element_from_array_and_shrinks)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 6;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 4; // Fill the array with 4 elements

  // Fill with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Values: 1, 2, 3, 4
  }

  DfResult pop_res = dfarray_pop(arr);
  cr_assert_eq(pop_res.error, DF_OK, "Pop failed on non-empty array");

  int *value = (int *)pop_res.value;
  cr_assert_eq(*value, 4, "Expected popped value to be 4");

  free(pop_res.value);

  cr_assert_eq(arr->length, 3, "Expected length to be 3 after pop");
  cr_assert_eq(arr->capacity, 3, "Expected capacity to shrink after pop");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, returns_error_for_empty_array_on_pop)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 0; // Empty array

  DfResult pop_res = dfarray_pop(arr);
  cr_assert_eq(pop_res.error, DF_ERR_EMPTY, "Expected DF_ERR_EMPTY when popping from an empty array");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, shifts_element_from_array_and_shrinks)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 6;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 4; // Fill the array with 4 elements

  // Fill with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Values: 1, 2, 3, 4
  }

  DfResult shift_res = dfarray_shift(arr);
  cr_assert_eq(shift_res.error, DF_OK, "Shift failed on non-empty array");

  int *value = (int *)shift_res.value;
  cr_assert_eq(*value, 1, "Expected shifted value to be 1");

  free(shift_res.value);

  cr_assert_eq(arr->length, 3, "Expected length to be 3 after shift");
  cr_assert_eq(arr->capacity, 3, "Expected capacity to shrink after shift");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, returns_error_for_empty_array_on_shift)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 0; // Empty array

  DfResult shift_res = dfarray_shift(arr);
  cr_assert_eq(shift_res.error, DF_ERR_EMPTY, "Expected DF_ERR_EMPTY when shifting from an empty array");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, unshifts_element_to_array_and_resizes)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 3; // Small capacity to trigger resize

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 3; // Fill the array

  // Fill with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Values: 1, 2, 3
  }

  int new_value = 42;
  DfResult unshift_res = dfarray_unshift(arr, &new_value);
  cr_assert_eq(unshift_res.error, DF_OK, "Unshift failed with valid array");

  cr_assert_eq(arr->length, 4, "Expected length to be 4 after unshift");
  cr_assert_eq(arr->capacity, 6, "Expected capacity to be resized (doubled)");

  // Verify the new value was added at the front
  cr_assert_eq(((int *)arr->items)[0], 42, "Expected value at index 0 to be 42");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, unshifts_element_to_non_full_array)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 2; // Fill the array with 2 elements

  // Fill with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Values: 1, 2
  }

  int new_value = 42;
  DfResult unshift_res = dfarray_unshift(arr, &new_value);
  cr_assert_eq(unshift_res.error, DF_OK, "Unshift failed with valid array");

  cr_assert_eq(arr->length, 3, "Expected length to be 3 after unshift");
  cr_assert_eq(arr->capacity, capacity, "Expected capacity to remain the same");

  // Verify the new value was added at the front
  cr_assert_eq(((int *)arr->items)[0], 42, "Expected value at index 0 to be 42");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, inserts_element_at_specified_index_and_resizes)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;

  // Fill with some values
  for (size_t i = 0; i < 5; i++)
  {
    dfarray_push(arr, &i);
  }

  int new_value = 42;
  DfResult insert_res = dfarray_insert_at(arr, 5, &new_value);
  cr_assert_eq(insert_res.error, DF_OK, "Insert failed on valid index");

  cr_assert_eq(arr->length, 6, "Expected length to be 6 after insert but it is %zu", arr->length);
  cr_assert_eq(arr->capacity, 10, "Expected capacity to be resized (doubled) but it is %zu", arr->capacity);

  // Verify the new value was inserted at the correct index
  cr_assert_eq(((int *)arr->items)[5], 42, "Expected value at index 5 to be 42");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, inserts_element_at_end_when_index_equals_length)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 3; // Fill the array

  // Fill with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Values: 1, 2, 3
  }

  int new_value = 42;
  DfResult insert_res = dfarray_insert_at(arr, arr->length, &new_value); // Insert at the end
  cr_assert_eq(insert_res.error, DF_OK, "Insert failed at the end of the array");

  cr_assert_eq(arr->length, 4, "Expected length to be 4 after insert at the end");
  cr_assert_eq(arr->capacity, 5, "Expected capacity to remain the same (no resize)");

  // Verify the new value was added at the end
  cr_assert_eq(((int *)arr->items)[3], 42, "Expected value at index 3 to be 42");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, removes_element_at_specified_index_and_shrinks)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 4; // Fill the array

  // Fill with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Values: 1, 2, 3, 4
  }

  DfResult remove_res = dfarray_remove_at(arr, 1); // Remove element at index 1 (value 2)
  cr_assert_eq(remove_res.error, DF_OK, "Remove failed on valid index");

  cr_assert_eq(arr->length, 3, "Expected length to be 3 after removal");
  cr_assert_eq(arr->capacity, 5, "Expected capacity to remain the same (no resize)");

  // Verify the elements are shifted correctly
  cr_assert_eq(((int *)arr->items)[0], 1, "Expected value at index 0 to be 1");
  cr_assert_eq(((int *)arr->items)[1], 3, "Expected value at index 1 to be 3");
  cr_assert_eq(((int *)arr->items)[2], 4, "Expected value at index 2 to be 4");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, removes_element_at_end_and_shrinks_capacity)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 4; // Fill the array

  // Fill with some values
  for (size_t i = 0; i < arr->length; i++)
  {
    ((int *)arr->items)[i] = (int)(i + 1); // Values: 1, 2, 3, 4
  }

  DfResult remove_res = dfarray_remove_at(arr, arr->length - 1); // Remove last element (value 4)
  cr_assert_eq(remove_res.error, DF_OK, "Remove failed at the end of the array");

  cr_assert_eq(arr->length, 3, "Expected length to be 3 after removal");

  // Verify the remaining elements
  cr_assert_eq(((int *)arr->items)[0], 1, "Expected value at index 0 to be 1");
  cr_assert_eq(((int *)arr->items)[1], 2, "Expected value at index 1 to be 2");
  cr_assert_eq(((int *)arr->items)[2], 3, "Expected value at index 2 to be 3");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, returns_correct_length_of_array)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 4; // Set array length to 4

  DfResult length_res = dfarray_length(arr);
  cr_assert_eq(length_res.error, DF_OK, "Length retrieval failed");
  cr_assert_eq((size_t)length_res.value, 4, "Expected length to be 4");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, returns_zero_when_array_is_empty)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 0; // Set array length to 0 (empty array)

  DfResult length_res = dfarray_length(arr);
  cr_assert_eq(length_res.error, DF_OK, "Length retrieval failed");
  cr_assert_eq((size_t)length_res.value, 0, "Expected length to be 0 for an empty array");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_suit, returns_correct_length_after_operations)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 2; // Set array length to 2

  // Add a new element
  int value = 5;
  DfResult push_res = dfarray_push(arr, &value);
  cr_assert_eq(push_res.error, DF_OK, "Failed to push an element");
  cr_assert_eq(arr->length, 3, "Expected length to be 3 after pushing an element");

  // Remove an element
  DfResult pop_res = dfarray_pop(arr);
  cr_assert_eq(pop_res.error, DF_OK, "Failed to pop an element");
  cr_assert_eq(arr->length, 2, "Expected length to be 2 after popping an element");

  // Verify length
  DfResult length_res = dfarray_length(arr);
  cr_assert_eq(length_res.error, DF_OK, "Length retrieval failed");
  cr_assert_eq((size_t)length_res.value, 2, "Expected length to be 2 after operations");

  // Cleanup
  dfarray_destroy(arr);
}

Test(df_array_iterator_suit, iterator_has_next)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = (DfArray *)create_res.value;
  arr->length = 3; // Set array length to 3

  DfResult iter_res = dfarray_iterator_create(arr);
  cr_assert_eq(iter_res.error, DF_OK);

  Iterator *it = (Iterator *)iter_res.value;

  cr_assert(dfarray_iterator_has_next(it), "Expected has_next to return true");
  dfarray_iterator_next(it);
  dfarray_iterator_next(it);
  dfarray_iterator_next(it);
  cr_assert_not(dfarray_iterator_has_next(it), "Expected has_next to return false after reaching the end");

  // Cleanup
  dfarray_destroy(arr);
  iterator_destroy(it);
}

Test(df_array_iterator_suit, iterator_next)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;

  int values[] = {1, 2, 3};
  for (size_t i = 0; i < arr->length; i++)
  {
    dfarray_push(arr, &values[i]);
  }

  DfResult iter_res = dfarray_iterator_create(arr);
  cr_assert_eq(iter_res.error, DF_OK);

  Iterator *it = iter_res.value;

  // Test next() to retrieve elements
  for (size_t i = 0; i < arr->length; i++)
  {
    DfResult next_res = dfarray_iterator_next(it);
    cr_assert_eq(next_res.error, DF_OK);
    cr_assert_eq(*(int *)next_res.value, values[i], "Next value mismatch");
    free(next_res.value); // Free copied value
  }

  // Cleanup
  dfarray_destroy(arr);
  iterator_destroy(it);
}

Test(df_array_iterator_suit, iterator_create_new)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;

  int values[] = {1, 2};
  for (size_t i = 0; i < arr->length; i++)
  {
    dfarray_push(arr, &values[i]);
  }

  DfResult iter_res = dfarray_iterator_create(arr);
  cr_assert_eq(iter_res.error, DF_OK, "iter_res error: %s\n", df_error_to_string(iter_res.error));

  Iterator *it = iter_res.value;

  DfResult new_array_res = dfarray_create_new(it);
  cr_assert_eq(new_array_res.error, DF_OK, "new_array_res error: %s\n", df_error_to_string(new_array_res.error));
  DfArray *new_array = new_array_res.value;

  cr_assert_eq(new_array->capacity, arr->capacity, "New array capacity mismatch");
  cr_assert_eq(new_array->length, 0, "New array should be empty");

  // Cleanup
  dfarray_destroy(arr);
  dfarray_destroy(new_array);
  iterator_destroy(it);
}

Test(df_array_iterator_suit, iterator_insert_new)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;

  int values[] = {1, 2};
  for (size_t i = 0; i < 2; i++)
  {
    dfarray_push(arr, &values[i]);
  }

  DfResult iter_res = dfarray_iterator_create(arr);
  cr_assert_eq(iter_res.error, DF_OK);

  Iterator *it = iter_res.value;

  int new_value = 3;
  DfResult insert_res = dfarray_insert_new(arr, &new_value);
  cr_assert_eq(insert_res.error, DF_OK);

  cr_assert_eq(arr->length, 3, "Array length mismatch after insert");

  // Cleanup
  dfarray_destroy(arr);
  iterator_destroy(it);
}

Test(df_array_iterator_suit, iterator_elem_size)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = create_res.value;
  arr->length = 3; // Set array length to 3

  DfResult iter_res = dfarray_iterator_create(arr);
  cr_assert_eq(iter_res.error, DF_OK);

  Iterator *it = (Iterator *)iter_res.value;

  size_t size = dfarray_elem_size(it);
  cr_assert_eq(size, elem_size, "Element size mismatch");

  // Cleanup
  dfarray_destroy(arr);
  iterator_destroy(it);
}

Test(df_array_iterator_suit, iterator_free_all)
{
  size_t elem_size = sizeof(int);
  size_t capacity = 5;

  DfResult create_res = dfarray_create(elem_size, capacity);
  cr_assert_eq(create_res.error, DF_OK);

  DfArray *arr = (DfArray *)create_res.value;

  int values[] = {1, 2};
  for (size_t i = 0; i < 2; i++)
  {
    dfarray_push(arr, &values[i]);
  }

  DfResult iter_res = dfarray_iterator_create(arr);
  cr_assert_eq(iter_res.error, DF_OK);

  Iterator *it = (Iterator *)iter_res.value;

  DfResult free_res = dfarray_free_all(it);
  cr_assert_eq(free_res.error, DF_OK, "Failed to free all elements");
  cr_assert_eq(arr->length, 0, "Array length mismatch after free");

  // Cleanup
  dfarray_destroy(arr);
  iterator_destroy(it);
}
