## Overview
Data Forge is a lightweight and extensible C library that provides high-level control over common data structures and utilities that are designed for efficiency, modularity and safety. DataForge aims to enhance C programming with modern, high-level abstractions while maintaining performance and flexibility.

## Data Structures

<details>
  <summary><strong>DfArray - Dynamic Array</strong></summary>

  ### DfArray
  DfArray is a lightweight, dynamic array that provides high-level and memory safe functionality to standard static C array's.
  
  ### Features
  - **Dynamic resizing**: Automatically expands when elements are added.
  - **Bounds checking**: Prevents out-of-bounds access with safe error handling.
  - **Generic storage**: Supports any data type via `void *` and configurable element sizes.
  - **Push/pop & unshift/shift operations**: Similar to JavaScript arrays.
  - **Functional mapping**: Apply functions to all elements.
  - **Iteration**: Iterate sequentially through all elements.
  
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
  dfarray_set(array, 1, &num);
  int *retrieved = (int *)dfarray_get(array, 1);
  printf("Retrieved value: %d\n", *retrieved);
  free(retrieved);
  ```
  
  #### Adding and Removing Elements
  ```c
  int value = 42;
  dfarray_push(array, &value);
  int *popped = (int *)dfarray_pop(array);
  printf("Popped value: %d\n", *popped);
  free(popped);
  
  int value2 = 25;
  dfarray_unshift(array, &value2);
  int *shifted = (int *)dfarray_shift(array);
  printf("Shifted value: %d\n", *shifted);
  free(shifted);
  
  int value3 = 30;
  dfarray_insert_at(array, 1, &value3);
  int *inserted = (int *)dfarray_get(array, 1);
  printf("Inserted value: %d\n", *inserted);
  free(inserted);
  dfarray_remove_at(array, 1);
  ```
  #### Iteration
  ```c
  DfArray *array = dfarray_create(sizeof(int), 3);
  int nums[] = {10, 20, 30};
  for(int i = 0; i < 3; i++){
    dfarray_push(array, &nums[i]);
  }

  Iterator it = dfarray_iterator_create(array);
  while(it.has_next(&it)){
    printf("Value: %d", *(int *)it.next(&it));
  };

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
  
  #### `void dfarray_push(DfArray* array, void *value)`
  Adds an element to the end, resizing if needed.
  
  #### `void *dfarray_pop(DfArray* array)`
  Removes and retrieves the last element.
  
  #### `void dfarray_unshift(DfArray* array, void *value)`
  Adds an element to the front, resizing if needed.
  
  #### `void *dfarray_shift(DfArray* array)`
  Removes and retrieves the first element.
  
  #### `void dfarray_set(DfArray* array, size_t index, void *value)`
  Updates a given element at a specified index.
  
  #### `void *dfarray_get(DfArray* array, size_t index)`
  Retrieves an element with bounds checking.
  
  #### `void dfarray_insert_at(DfArray* array, size_t index, void *value)`
  Inserts an element at a specified index and shifts following elements to the right.
  
  #### `void dfarray_remove_at(DfArray* array, size_t index)`
  Removes an element at a specified index and shifts following elements to the left.
  
  #### `void dfarray_map(DfArray *array, void (*func)(void *))`
  Applies a function to each element.

  #### `Iterator dfarray_iterator_create(DfArray *array)`
  Creates an iterator for a dynamic array.
  
  #### `int dfarray_iterator_has_next(Iterator *it)`
  Checks if there is a value to iterate over.
  
  #### `void *dfarray_iterator_next(Iterator *it)`
  Iterates over the next value in the array.
  
  </details>  
</details>
<details>
  <summary><strong>DfList_S - Singly Linked List</strong></summary>

  ### DfList_S
  DfList_S is a lightweight, dynamic singly linked list that provides high-level and memory safe functionality with generic type storage.

  ### Features
  - **Dynamic & Generic** – Stores any data type using void *
  - **Insertion** – Add elements at the front, back, or a specific index.
  - **Deletion** – Remove elements from the front, back, or a specific index.
  - **Access & Lookup** – Retrieve elements by index, find values with a comparator.
  - **Iteration** - Iterate sequentially through all elements.
  - **Safe Memory Management** – Custom cleanup function for freeing stored data.

<details>
    <summary><strong>Usage</strong></summary>
</details>

<details>
    <summary><strong>API Reference</strong></summary>

  #### `void DfList_S_Destroy(DfList_S *list, void (*cleanup)(void *element))`
  Free's a DfList_S struct and nodes. Allows user to pass cleanup function that determines how the elements stored in the linked list are free'd. 
</details>
</details>

## Utils
Data Forge uses a monolithic utils header for ease of use. To use any utility function include df_utils.h in your file as shown below.
```c
#include <dataforge/df_utils.h>
```

<details>
  <summary><strong>Generic - Can be used with any data structure</strong></summary>

  ### `void *df_map(Iterator *it, void *(*func)(void *element))`
  df_map takes an iterator and a function pointer as arguments. It iterates over any data structure and apply's a function to each element, then returns a new data structure      containing the modified elements.

  ### Usage
  ```c
  DfArray *array = dfarray_create(sizeof(int), 3);
  int nums[] = {10, 20, 30};
  for(int i = 0; i < 3; i++){
    dfarray_push(array, &nums[i]);
  }

  void *double_element(void *element) {
    int *value = (int *)element;
    int *modified_value = malloc(sizeof(int));
    *modified_value = (*value) * 2;
    return modified_value;
  }

  Iterator it = dfarray_iterator_create(array);

  // cast returned data structure to proper type
  DfArray *new_array = (DfArray *)df_map(&it, double_element);
  ```
  ### `void *df_filter(Iterator *it, bool (*func)(void *element))`
  df_filter takes an iterator and function pointer that returns a bool as parameters. It iterates over any data structure and applies the comparison function to each element. It then returns a new data structure containing the filtered elements that met the condition in the comparison function.

  ### Usage
  ```c
  DfArray *array = dfarray_create(sizeof(int), 3);
  int nums[] = {10, 23, 30};
  for(int i = 0; i < 3; i++){
    dfarray_push(array, &nums[i]);
  }

  bool isEven(void *element) {
    return *(int *)element % 2 == 0;
  }

  Iterator it = dfarray_iterator_create(array);

  // Cast returned data structure to propper type
  DfArray *filtered = (DfArray *)df_filter(&it, isEven);
  ```

### `void *df_find(Iterator *it, bool (*func)(void *element))`
df_find takes an iterator and a function pointer that returns a bool as parameters. It iterates over any data structure and applies the comparison function to each elemenet. It then returns the first element that meets the condition in the comparison function.

### Usage
```c
DfArray *array = dfarray_create(sizeof(int), 3);
int nums[] = {10, 23, 30};
for(int i = 0; i < 3; i++){
    dfarray_push(array, &nums[i]);
}

bool greater_than_10(void *element) {
  return *(int *)element > 10;
}

Iterator it = dfarray_iterator_create(array);
void *found = df_find(&it, greater_than_10);

// Best practice to check for null before casting to propper type
if (found != NULL) {
  printf("Found element: %d", *(int *)found);
} else {
  printf("No element found");
}
```

### `void df_for_each(Iterator *it, void (*func)(void *element))`
df_for_each takes an iterator and a function pointer as parameters. It iterates through any data structure and applies the provided function to each element. It does not return anything nor does it modify the original data structure.

### Usage
```c
DfArray *array = dfarray_create(sizeof(int), 3);
int nums[] = {10, 23, 30};
for(int i = 0; i < 3; i++){
    dfarray_push(array, &nums[i]);
}

void print_num_plus_2(void *element) {
  printf("%d\n", *(int *)element + 2);
}

Iterator it = dfarray_iterator_create(array);
df_for_each(&it, print_num_plus_2);
```

### `size_t df_count(Iterator *it, bool (*func)(void *element))`
df_count takes an iterator and a function pointer as parameters. It iterates through any data structure and applies the passed in comparison function to each element to see if the element satisfies a condition, if the function returns true a count is incremented. The final count is then returned.

### Usage
```c
DfArray *array = dfarray_create(sizeof(int), 3);
int nums[] = {10, 23, 30};
for(int i = 0; i < 3; i++){
    dfarray_push(array, &nums[i]);
}

bool isEven(void *element) {
    return *(int *)element % 2 == 0;
}

Iterator it = dfarray_iterator_create(array);
size_t count = df_count(&it, isEven);
```

### `void *df_reduce(Iterator *it, void *initial, void (*func)(void *accumulator, void *element))`
df_reduce takes a pointer to an interator, pointer to an initial value and a function pointer as parameters. It iterates through any data structure and applies the function to every element in the data structure and returns the reduced value.

### Usage
```c
  DfArray *array = dfarray_create(sizeof(int), 3);
  int nums[] = {10, 23, 30};
  for(int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }
  Iterator it = dfarray_iterator_create(array);

  void sum_int(void *acc, void *elem) {
    *(int *)acc += *(int *)elem;
  }

  int initial = 0;
  int *reduced = (int *)df_reduce(&it, &initial, sum_int);
  printf("Reduced: %d", *reduced);

  free(reduced);
  iterator_destroy(&it);
  dfarray_destroy(array);
```

### `void df_free_all(Iterator *it)`
df_free_all takes an iterator pointer as a parameter. It frees all elements in the data structure from memory without freeing the data structure struct itself allowing you to continue using the now empty data structure.

### Usage
```c
  DfArray *array = dfarray_create(sizeof(int), 3);
  int nums[] = {10, 23, 30};
  for(int i = 0; i < 3; i++) {
    dfarray_push(array, &nums[i]);
  }

  Iterator it = dfarray_iterator_create(array);
  df_free_all(&it);

  int newNum = 5;
  dfarray_push(array, &newNum);
  
  iterator_destroy(&it);
  dfarray_destroy(array);
```
</details>

## Contributing
Currently using this as a learning experience and not looking for contributions at this time. But in the future as this expands I will update this section.

## License
Nothing yet.

