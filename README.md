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
  DfArray *array = DfArray_Create(sizeof(int), 10);
  DfArray_Destroy(array);
  ```
  
  #### Getting and Setting Elements
  ```c
  int num = 10;
  DfArray_Set(array, 1, &num);
  int retrieved;
  DfArray_Get(array, 1, &retrieved);
  printf("Retrieved value: %d\n", retrieved);
  ```
  
  #### Adding and Removing Elements
  ```c
  int value = 42;
  DfArray_Push(array, &value);
  int popped;
  DfArray_Pop(array, &popped);
  printf("Popped value: %d\n", popped);
  
  int value2 = 25;
  DfArray_Unshift(array, &value2);
  int shifted;
  DfArray_Shift(array, &shifted);
  printf("Shifted value: %d\n", shifted);
  
  int value3 = 30;
  DfArray_InsertAt(array, 1, &value3);
  int inserted;
  DfArray_Get(array, 1, &inserted);
  printf("Inserted value: %d\n", inserted);
  DfArray_RemoveAt(array, 1);
  ```
  #### Iteration
  ```c
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int nums[] = {10, 20, 30};
  for(int i = 0; i < 3; i++){
    DfArray_Push(array, &nums[i]);
  }

  Iterator it = DfArray_Iterator_Create(array);
  while(it.has_next(&it)){
    printf("Value: %d", *(int *)it.next(&it));
  };

  Iterator_Destroy(&it);
  DfArray_Destroy(array);
  ```
  
  #### Applying a Function to All Elements
  ```c
  void printInt(void *item) {
      printf("%d\n", *(int *)item);
  }
  DfArray_Map(array, printInt);
  ```
  </details>

  <details>
    <summary><strong>API Reference</strong></summary>
    
  #### `DfArray* DfArray_Create(size_t elem_size, size_t initial_capacity)`
  Allocates a new dynamic array.
  
  #### `void DfArray_Destroy(DfArray* array)`
  Frees memory associated with the array.
  
  #### `void DfArray_Push(DfArray* array, void *value)`
  Adds an element to the end, resizing if needed.
  
  #### `void DfArray_Pop(DfArray* array, void *dest)`
  Removes and retrieves the last element.
  
  #### `void DfArray_Unshift(DfArray* array, void *value)`
  Adds an element to the front, resizing if needed.
  
  #### `void DfArray_Shift(DfArray* array, void *dest)`
  Removes and retrieves the first element.
  
  #### `void DfArray_Set(DfArray* array, size_t index, void *value)`
  Updates a given element at a specified index.
  
  #### `void DfArray_Get(DfArray* array, size_t index, void *dest)`
  Retrieves an element with bounds checking.
  
  #### `void DfArray_InsertAt(DfArray* array, size_t index, void *value)`
  Inserts an element at a specified index and shifts following elements to the right.
  
  #### `void DfArray_RemoveAt(DfArray* array, size_t index)`
  Removes an element at a specified index and shifts following elements to the left.
  
  #### `void DfArray_Map(DfArray *array, void (*func)(void *))`
  Applies a function to each element.

  #### `Iterator DfArray_Iterator_Create(DfArray *array)`
  Creates an iterator for a dynamic array.
  
  #### `int DfArray_Iterator_Has_Next(Iterator *it)`
  Checks if there is a value to iterate over.
  
  #### `void *DfArray_Iterator_Next(Iterator *it)`
  Iterates over the next value in the array.
  
  </details>  
</details>

## Utils
Data Forge uses a monolithic utils header for ease of use. To use any utility function include df_utils.h in your file as shown below.
```c
#include <dataforge/df_utils.h>
```

<details>
  <summary><strong>Generic - Can be used with any data structure</strong></summary>

  ### `void *DfMap(Iterator *it, void *(*func)(void *element))`
  DfMap takes an iterator and a function pointer as arguments. It iterates over any data structure and apply's a function to each element, then returns a new data structure      containing the modified elements.

  ### Usage
  ```c
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int nums[] = {10, 20, 30};
  for(int i = 0; i < 3; i++){
    DfArray_Push(array, &nums[i]);
  }

  void *double_element(void *element) {
    int *value = (int *)element;
    int *modified_value = malloc(sizeof(int));
    *modified_value = (*value) * 2;
    return modified_value;
  }

  Iterator it = DfArray_Iterator_Create(array);

  // cast returned data structure to proper type
  DfArray *new_array = (DfArray *)DfMap(&it, double_element);
  ```
  ### `void *DfFilter(Iterator *it, bool (*func)(void *element))`
  DfFilter takes an iterator and function pointer that returns a bool as parameters. It iterates over any data structure and applies the comparison function to each element. It then returns a new data structure containing the filtered elements that met the condition in the comparison function.

  ### Usage
  ```c
  DfArray *array = DfArray_Create(sizeof(int), 3);
  int nums[] = {10, 23, 30};
  for(int i = 0; i < 3; i++){
    DfArray_Push(array, &nums[i]);
  }

  bool isEven(void *element) {
    return *(int *)element % 2 == 0;
  }

  Iterator it = DfArray_Iterator_Create(array);

  // Cast returned data structure to propper type
  DfArray *filtered = (DfArray *)DfFilter(&it, isEven);
  ```

### `void *DfFind(Iterator *it, bool (*func)(void *element))`
DfFind takes an iterator and a function pointer that returns a bool as parameters. It iterates over any data structure and applies the comparison function to each elemenet. It then returns the first element that meets the condition in the comparison function.

### Usage
```c
DfArray *array = DfArray_Create(sizeof(int), 3);
int nums[] = {10, 23, 30};
for(int i = 0; i < 3; i++){
    DfArray_Push(array, &nums[i]);
}

bool greater_than_10(void *element) {
  return *(int *)element > 10;
}

Iterator it = DfArray_Iterator_Create(array);
void *found = DfFind(&it, greater_than_10);

// Best practice to check for null before casting to propper type
if (found != NULL) {
  printf("Found element: %d", *(int *)found);
} else {
  printf("No element found");
}
```

### `void DfForEach(Iterator *it, void (*func)(void *element))`
DfForEach takes an iterator and a function pointer as parameters. It iterates through any data structure and applies the provided function to each element. It does not return anything nor does it modify the original data structure.

### Usage
```c
DfArray *array = DfArray_Create(sizeof(int), 3);
int nums[] = {10, 23, 30};
for(int i = 0; i < 3; i++){
    DfArray_Push(array, &nums[i]);
}

void print_num_plus_2(void *element) {
  printf("%d\n", *(int *)element + 2);
}

Iterator it = DfArray_Iterator_Create(array);
DfForEach(&it, print_num_plus_2);
```

### `size_t DfCount(Iterator *it, bool (*func)(void *element))`
DfCount takes an iterator and a function pointer as parameters. It iterates through any data structure and applies the passed in comparison function to each element to see if the element satisfies a condition, if the function returns true a count is incremented. The final count is then returned.

### Usage
```c
DfArray *array = DfArray_Create(sizeof(int), 3);
int nums[] = {10, 23, 30};
for(int i = 0; i < 3; i++){
    DfArray_Push(array, &nums[i]);
}

bool isEven(void *element) {
    return *(int *)element % 2 == 0;
}

Iterator it = DfArray_Iterator_Create(array);
size_t count = DfCount(&it, isEven);
```
</details>

## Contributing
Currently using this as a learning experience and not looking for contributions at this time. But in the future as this expands I will update this section.

## License
Nothing yet.

