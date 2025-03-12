## Overview
DataForge is a lightweight and extensible C library that provides high-level control over common data structures and utilities that are designed for efficiency, modularity and safety. DataForge aims to enhance C programming with modern, high-level abstractions while maintaining performance and flexibility.

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
  </details>  
</details>

## Contributing
Currently using this as a learning experience and not looking for contributions at this time. But in the future as this expands I will update this section.

## License
Nothing yet.

