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

<details>
  <summary><strong>Creating and Destroying an Array</strong></summary>

```c
DfResult res = dfarray_create(sizeof(int), 10);
DfArray *array = (DfArray *)res.value;
if (res.error != DF_OK) {
    printf("Create error: %s\n", df_error_to_string(res.error));
    return;
}

DfResult destroy_result = dfarray_destroy(array);
if (destroy_result.error != DF_OK) {
    printf("Destroy error: %s\n", df_error_to_string(destroy_result.error));
}
```
</details>

<details>
  <summary><strong>Getting and Setting Elements</strong></summary>

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
</details>

<details>
  <summary><strong>Adding and Removing Elements</strong></summary>

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
</details>

<details>
  <summary><strong>Iteration</strong></summary>

```c
DfResult create_result = dfarray_create(sizeof(int), 3);
DfArray *array = (DfArray *)create_result.value;
int nums[] = {10, 20, 30};
for (int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
}

DfResult it_result = dfarray_iterator_create(array);
if (it_result.error != DF_OK) {
    printf("Iterator create error: %s\n", df_error_to_string(it_result.error));
    dfarray_destroy(array);
    return;
}

Iterator *it = (Iterator *)it_result.value;

while (it->has_next(it)) {
    DfResult next_res = it->next(it);
    if (next_res.error == DF_OK) {
        int *val = (int *)next_res.value;
        printf("Value: %d\n", *val);
        free(val);
    }
}

// Clean up
it->free_all(it);
dfarray_destroy(array);
```
</details>
  </details>

  <details>
    <summary><strong>API Reference</strong></summary>

### `DfResult dfarray_create(size_t elem_size, size_t initial_capacity)`
Creates a new dynamic array with a specific element size and initial capacity.  
✅ **Returns:**  
- `value`: `(DfArray *)` — pointer to the newly allocated dynamic array.  
- `error`: `DF_OK` on success, or an error code on failure.

---

### `DfResult dfarray_destroy(DfArray *array)`
Frees memory associated with the dynamic array.  
✅ **Returns:**  
- `value`: `NULL`.  
- `error`: `DF_OK` on success, or `DF_ERR_NULL_PTR` if `array` is `NULL`.

---

### `DfResult dfarray_push(DfArray *array, void *value)`
Appends an element to the end of the array.  
✅ **Returns:**  
- `value`: `NULL`.  
- `error`: `DF_OK` on success, or error if memory reallocation fails.

---

### `DfResult dfarray_pop(DfArray *array)`
Removes the last element from the array.  
✅ **Returns:**  
- `value`: `(void *)` — pointer to a **heap-allocated copy** of the removed element.  
- Caller is responsible for freeing the value.  
- `error`: `DF_OK` on success, or `DF_ERR_EMPTY` if the array is empty.

---

### `DfResult dfarray_unshift(DfArray *array, void *value)`
Inserts an element at the beginning of the array.  
✅ **Returns:**  
- `value`: `NULL`.  
- `error`: `DF_OK` on success, or an error code if memory reallocation fails.

---

### `DfResult dfarray_shift(DfArray *array)`
Removes the first element from the array.  
✅ **Returns:**  
- `value`: `(void *)` — pointer to a **heap-allocated copy** of the removed element.  
- Caller must `free()` the returned pointer.  
- `error`: `DF_OK` on success, or `DF_ERR_EMPTY` if the array is empty.

---

### `DfResult dfarray_set(DfArray *array, size_t index, void *value)`
Overwrites the value at the specified index.  
✅ **Returns:**  
- `value`: `NULL`.  
- `error`: `DF_OK` on success, or `DF_ERR_INDEX_OUT_OF_BOUNDS`.

---

### `DfResult dfarray_get(DfArray *array, size_t index)`
Retrieves the element at the specified index.  
✅ **Returns:**  
- `value`: `(void *)` — pointer to a **heap-allocated copy** of the element.  
- Caller must `free()` the returned pointer.  
- `error`: `DF_OK` on success, or `DF_ERR_INDEX_OUT_OF_BOUNDS`.

---

### `DfResult dfarray_insert_at(DfArray *array, size_t index, void *value)`
Inserts an element at the specified index, shifting subsequent elements right.  
✅ **Returns:**  
- `value`: `NULL`.  
- `error`: `DF_OK` on success, or an error code if index is invalid or reallocation fails.

---

### `DfResult dfarray_remove_at(DfArray *array, size_t index)`
Removes the element at the specified index, shifting remaining elements left.  
✅ **Returns:**  
- `value`: `(void *)` — pointer to a **heap-allocated copy** of the removed element.  
- Caller is responsible for freeing the memory.  
- `error`: `DF_OK` on success, or `DF_ERR_INDEX_OUT_OF_BOUNDS`.

---

### `DfResult dfarray_iterator_create(DfArray *array)`
Initializes a generic `Iterator` for the given array.  
✅ **Returns:**  
- `value`: `(Iterator *)` — pointer to a heap-allocated iterator.  
- `error`: `DF_OK` on success, or error if memory allocation fails.

---

### `int dfarray_iterator_has_next(Iterator *it)`
Checks if there are more elements in the iteration.  
✅ **Returns:**  
- `1` if more elements exist, `0` otherwise.

---

### `DfResult dfarray_iterator_next(Iterator *it)`
Retrieves the next element from the iterator.  
✅ **Returns:**  
- `value`: `(void *)` — pointer to a **heap-allocated copy** of the current element.  
- Caller must `free()` the returned pointer.  
- `error`: `DF_OK` if successful, or `DF_ERR_ITER_END` if no more elements.

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
- **Iteration**: Iterate sequentially through all elements.
---

<details>
<summary><strong>Usage</strong></summary>

<details>
  <summary><strong>Creating and Destroying a List</strong></summary>

```c
DfResult res_create = dflist_s_create();
if (res_create.error != DF_OK) {
    printf("Create error: %s\n", df_error_to_string(res_create.error));
    return;
}
DfList_S *list = (DfList_S *)res_create.value;


DfResult destroy_result = dfarray_destroy(array);
if (destroy_result.error != DF_OK) {
    printf("Destroy error: %s\n", df_error_to_string(destroy_result.error));
}
```
</details>

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

    DfResult it_des_res = iterator_destroy(&it);
    if (it_des_res.error){
      // Handle error
    }
    DfResult arr_des_res = dfarray_destroy(array);
    if (arr_des_res.error){
      // Handle error
    }
  }
}
```

</details>

## Contributing
Currently using this as a learning experience and not looking for contributions at this time. But in the future as this expands I will update this section.

## License
Nothing yet.

