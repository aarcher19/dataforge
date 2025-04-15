## Overview
Data Forge is a lightweight and extensible C library that provides high-level control over common data structures and utilities that are designed for efficiency, modularity and safety. DataForge aims to enhance C programming with modern, high-level abstractions while maintaining performance and flexibility.

## Data Structures

<details>
  <summary><strong>DfArray - Dynamic Array</strong></summary>

  ### DfArray
  DfArray is a lightweight, dynamic array that provides high-level and memory-safe functionality to standard static C arrays. All operations return a `DfResult` type, encapsulating both the result and potential error.

  ### Features
  - **Dynamic resizing**: Automatically expands when elements are added.
  - **Bounds checking**: Prevents out-of-bounds access with detailed error reporting.
  - **Generic storage**: Supports any data type via `void *` and configurable element sizes.
  - **Push/pop & unshift/shift operations**: Similar to JavaScript arrays.
  - **Functional mapping**: Apply functions to all elements.
  - **Iteration**: Iterate sequentially through all elements.
  - **Unified error handling**: Every function returns a `DfResult`, enabling precise control and logging.

  > 💡 Use `df_error_to_string(result.error)` to convert error codes into human-readable messages.

  <details>
    <summary><strong>Usage</strong></summary>

  #### Creating and Destroying an Array
  ```c
  DfArray *array = dfarray_create(sizeof(int), 10);
  dfarray_destroy(array);
  ```

  #### Getting and Setting Elements
  ```c
  int num = 10;
  DfResult set_result = dfarray_set(array, 1, &num);
  if (set_result.error != DF_OK) {
      printf("Set error: %s\n", df_error_to_string(set_result.error));
  }

  DfResult get_result = dfarray_get(array, 1);
  if (get_result.error == DF_OK) {
      int *retrieved = (int *)get_result.value;
      printf("Retrieved value: %d\n", *retrieved);
      free(retrieved);
  } else {
      printf("Get error: %s\n", df_error_to_string(get_result.error));
  }
  ```

  #### Adding and Removing Elements
  ```c
  int value = 42;
  dfarray_push(array, &value);

  DfResult pop_result = dfarray_pop(array);
  if (pop_result.error == DF_OK) {
      int *popped = (int *)pop_result.value;
      printf("Popped value: %d\n", *popped);
      free(popped);
  }

  int value2 = 25;
  dfarray_unshift(array, &value2);

  DfResult shift_result = dfarray_shift(array);
  if (shift_result.error == DF_OK) {
      int *shifted = (int *)shift_result.value;
      printf("Shifted value: %d\n", *shifted);
      free(shifted);
  }

  int value3 = 30;
  dfarray_insert_at(array, 1, &value3);

  DfResult inserted_result = dfarray_get(array, 1);
  if (inserted_result.error == DF_OK) {
      int *inserted = (int *)inserted_result.value;
      printf("Inserted value: %d\n", *inserted);
      free(inserted);
  }

  dfarray_remove_at(array, 1);
  ```

  #### Iteration
  ```c
  DfArray *array = dfarray_create(sizeof(int), 3);
  int nums[] = {10, 20, 30};
  for (int i = 0; i < 3; i++) {
      dfarray_push(array, &nums[i]);
  }

  Iterator it = dfarray_iterator_create(array);
  while (it.has_next(&it)) {
      void *val = it.next(&it);
      printf("Value: %d\n", *(int *)val);
  }

  iterator_destroy(&it);
  dfarray_destroy(array);
  ```

  #### Applying a Function to All Elements
  ```c
  void printInt(void *item) {
      printf("%d\n", *(int *)item);
  }
  dfarray_map(array, printInt);
  ```

  </details>

  <details>
    <summary><strong>API Reference</strong></summary>

  #### `DfArray* dfarray_create(size_t elem_size, size_t initial_capacity)`
  Allocates a new dynamic array.

  #### `void dfarray_destroy(DfArray* array)`
  Frees memory associated with the array.

  #### `DfResult dfarray_push(DfArray* array, void *value)`
  Adds an element to the end. Returns `DF_OK` or error code.

  #### `DfResult dfarray_pop(DfArray* array)`
  Removes and retrieves the last element. `value` contains the element if successful.

  #### `DfResult dfarray_unshift(DfArray* array, void *value)`
  Adds an element to the front.

  #### `DfResult dfarray_shift(DfArray* array)`
  Removes and retrieves the first element.

  #### `DfResult dfarray_set(DfArray* array, size_t index, void *value)`
  Updates an element at a given index.

  #### `DfResult dfarray_get(DfArray* array, size_t index)`
  Retrieves an element with bounds checking.

  #### `DfResult dfarray_insert_at(DfArray* array, size_t index, void *value)`
  Inserts an element at a specified index and shifts elements to the right.

  #### `DfResult dfarray_remove_at(DfArray* array, size_t index)`
  Removes an element at a specified index and shifts elements to the left.

  #### `void dfarray_map(DfArray *array, void (*func)(void *))`
  Applies a function to each element.

  #### `Iterator dfarray_iterator_create(DfArray *array)`
  Creates an iterator for a dynamic array.

  #### `int dfarray_iterator_has_next(Iterator *it)`
  Checks if there is a next value to iterate over.

  #### `void *dfarray_iterator_next(Iterator *it)`
  Retrieves the next value in the array.

  </details>
</details>

<details>
  <summary><strong>DfList_S - Singly Linked List</strong></summary>

### DfList_S

`DfList_S` is a lightweight, dynamic singly linked list that provides high-level and memory-safe functionality with generic type storage.

---

### Features

- **Dynamic & Generic** – Stores any data type using `void *`.
- **Insertion** – Add elements at the front, back, or a specific index.
- **Deletion** – Remove elements from the front or back.
- **Safe Memory Management** – Custom cleanup function for freeing stored data.
- **Robust Error Handling** – Returns `DfResult` with error codes for safer programming.

---

<details>
<summary><strong>Usage</strong></summary>

```c
DfResult res = dflist_s_create();
DfList_S *list = (DfList_S *)res.value;

dflist_s_push_back(list, my_data);
dflist_s_push_front(list, other_data);

DfResult popped = dflist_s_pop_back(list);
// Remember to free the popped element if necessary

dflist_s_destroy(list, free); // free each element using user-defined cleanup
```

</details>

---

<details>
<summary><strong>API Reference</strong></summary>

#### `DfResult dflist_s_create()`
Creates a new singly linked list.  
Returns a `DfResult` with `value` pointing to the new `DfList_S`.

#### `DfResult dflist_s_destroy(DfList_S *list, void (*cleanup)(void *element))`
Destroys the list and all of its nodes.  
Calls `cleanup` on each element if provided.

#### `DfResult dflist_s_push_back(DfList_S *list, void *element)`
Appends an element to the end of the list.

#### `DfResult dflist_s_push_front(DfList_S *list, void *element)`
Prepends an element to the front of the list.

#### `DfResult dflist_s_pop_back(DfList_S *list)`
Removes and returns the last element in the list.  
⚠️ User is responsible for freeing the returned element if necessary.

#### `DfResult dflist_s_pop_front(DfList_S *list)`
Removes and returns the first element in the list.  
⚠️ User is responsible for freeing the returned element if necessary.

#### `DfResult dflist_s_insert_at(DfList_S *list, void *element, size_t index)`
Inserts an element at the specified index.  
Returns an error if index is out of bounds.

</details>

</details>


## Utils
Data Forge uses a monolithic utils header for ease of use. To use any utility function include df_utils.h in your file as shown below.
```c
#include <dataforge/df_utils.h>
```



<details>
  <summary><strong>Generic - Can be used with any data structure</strong></summary>

### `DfResult df_map(Iterator *it, void *(*func)(void *element))`

`df_map` takes an iterator and a function pointer as arguments. It iterates over any data structure, applies the provided function to each element, and returns a new data structure containing the modified elements.

#### Usage
```c
DfResult res = dfarray_create(sizeof(int), 3);
if (res.error) {
  // Handle error
} else {
  DfArray *array = (DfArray *)res.value;
  int nums[] = {10, 20, 30};
  for (int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }

  void *double_element(void *element) {
    int *value = (int *)element;
    int *modified = malloc(sizeof(int));
    *modified = (*value) * 2;
    return modified;
  }

  DfResult it_res = dfarray_iterator_create(array);
  if (it_res.error) {
    // Handle error
  } else {
    Iterator it = *(Iterator *)it_res.value;

    // Cast returned data structure to proper type
    DfResult map_res = df_map(&it, double_element);
    if (map_res.error) {
      // Handle error
    } else {
      DfArray *new_array = (DfArray *)map_res.value;
      // Use new_array
    }
  }
}
```

---

### `DfResult df_filter(Iterator *it, bool (*func)(void *element))`

`df_filter` takes an iterator and a boolean function pointer. It returns a new data structure containing only the elements that satisfy the condition in the provided function.

#### Usage
```c
DfResult res = dfarray_create(sizeof(int), 3);
if (res.error) {
  // Handle error
} else {
  DfArray *array = (DfArray *)res.value;
  int nums[] = {10, 23, 30};
  for (int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }

  bool is_even(void *element) {
    return *(int *)element % 2 == 0;
  }

  DfResult it_res = dfarray_iterator_create(array);
  if (it_res.error) {
    // Handle error
  } else {
    Iterator it = *(Iterator *)it_res.value;

    // Cast returned data structure to proper type
    DfResult filter_res = df_filter(&it, is_even);
    if (filter_res.error) {
      // Handle error
    } else {
      DfArray *filtered = (DfArray *)filter_res.value;
      // Use filtered
    }
  }
}
```

---

### `DfResult df_find(Iterator *it, bool (*func)(void *element))`

`df_find` searches through a data structure and returns the first element that satisfies the condition specified in the provided function.

#### Usage
```c
DfResult res = dfarray_create(sizeof(int), 3);
if (res.error) {
  // Handle error
} else {
  DfArray *array = (DfArray *)res.value;
  int nums[] = {10, 23, 30};
  for (int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }

  bool greater_than_10(void *element) {
    return *(int *)element > 10;
  }

  DfResult it_res = dfarray_iterator_create(array);
  if (it_res.error) {
    // Handle error
  } else {
    Iterator it = *(Iterator *)it_res.value;
    DfResult find_res = df_find(&it, greater_than_10);

    if (find_res.error) {
      // Handle error
    } else {
      int *found = (int *)find_res.value;
      printf("Found element: %d", *found);
    }
  }
}
```

---

### `DfResult df_for_each(Iterator *it, void (*func)(void *element))`

`df_for_each` applies a function to every element in the data structure without modifying the structure or returning a value.

#### Usage
```c
DfResult res = dfarray_create(sizeof(int), 3);
if (res.error) {
  // Handle error
} else {
  DfArray *array = (DfArray *)res.value;
  int nums[] = {10, 23, 30};
  for (int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }

  void print_plus_two(void *element) {
    printf("%d
", *(int *)element + 2);
  }

  DfResult it_res = dfarray_iterator_create(array);
  if (it_res.error) {
    // Handle error
  } else {
    Iterator it = *(Iterator *)it_res.value;
    DfResult for_each_res = df_for_each(&it, print_plus_two);

    if (for_each_res.error) {
      // Handle error
    }
  }
}
```

---

### `DfResult df_count(Iterator *it, bool (*func)(void *element))`

`df_count` returns the number of elements in the data structure that satisfy the given condition function.

#### Usage
```c
DfResult res = dfarray_create(sizeof(int), 3);
if (res.error) {
  // Handle error
} else {
  DfArray *array = (DfArray *)res.value;
  int nums[] = {10, 23, 30};
  for (int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }

  bool is_even(void *element) {
    return *(int *)element % 2 == 0;
  }

  DfResult it_res = dfarray_iterator_create(array);
  if (it_res.error) {
    // Handle error
  } else {
    Iterator it = *(Iterator *)it_res.value;
    DfResult count_res = df_count(&it, is_even);

    if (count_res.error) {
      // Handle error
    } else {
      size_t count = *(size_t *)count_res.value;
      printf("Count: %zu", count);
    }
  }
}
```

---

### `DfResult df_reduce(Iterator *it, void *initial, void (*func)(void *accumulator, void *element))`

`df_reduce` takes an iterator, an initial value, and a reducer function. It combines all elements into a single result based on the reducer logic.

#### Usage
```c
DfResult res = dfarray_create(sizeof(int), 3);
if (res.error) {
  // Handle error
} else {
  DfArray *array = (DfArray *)res.value;
  int nums[] = {10, 23, 30};
  for (int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }

  void sum_int(void *acc, void *elem) {
    *(int *)acc += *(int *)elem;
  }

  DfResult it_res = dfarray_iterator_create(array);
  if (it_res.error) {
    // Handle error
  } else {
    Iterator it = *(Iterator *)it_res.value;
    int initial = 0;

    DfResult reduce_res = df_reduce(&it, &initial, sum_int);
    if (reduce_res.error) {
      // Handle error
    } else {
      int *reduced = (int *)reduce_res.value;
      printf("Reduced value: %d", *reduced);

      free(reduced);
    }
  }
}
```

---

### `DfResult df_free_all(Iterator *it)`

`df_free_all` frees the memory of all elements inside the data structure but leaves the structure itself intact so it can be reused.

#### Usage
```c
DfResult res = dfarray_create(sizeof(int), 3);
if (res.error) {
  // Handle error
} else {
  DfArray *array = (DfArray *)res.value;
  int nums[] = {10, 23, 30};
  for (int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }

  DfResult it_res = dfarray_iterator_create(array);
  if (it_res.error) {
    // Handle error
  } else {
    Iterator it = *(Iterator *)it_res.value;
    DfResult free_all_res = df_free_all(&it);

    if (free_all_res.error) {
      // Handle error
    }

    // Safe to reuse the structure
    int new_num = 5;
    dfarray_push(array, &new_num);

    iterator_destroy(&it);
    dfarray_destroy(array);
  }
}
```

</details>

## Contributing
Currently using this as a learning experience and not looking for contributions at this time. But in the future as this expands I will update this section.

## License
Nothing yet.

