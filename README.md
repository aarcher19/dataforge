## Data Structures

<details>
  <summary><strong>DfArray - Dynamic Array</strong></summary>

## Overview
DfArray is a lightweight, dynamic array that provides high-level control over memory management and data structures.

## Features
- **Dynamic resizing**: Automatically expands when elements are added.
- **Bounds checking**: Prevents out-of-bounds access with safe error handling.
- **Generic storage**: Supports any data type via `void *` and configurable element sizes.
- **Push/pop & unshift/shift operations**: Similar to JavaScript arrays.
- **Functional mapping**: Apply functions to all elements.

## Installation
To use DfArray in your project, include `df_array.h` and link with the corresponding source files. Example:
```sh
# Example compilation
gcc -o my_program my_program.c src/df_array.c -Iincludes
```

## Usage
### Creating and Destroying an Array
```c
DfArray *array = DfArray_Create(sizeof(int), 10);
DfArray_Destroy(array);
```

### Adding and Removing Elements
```c
int num = 10;
DfArray_Set(array, 1, &num);
int retrieved;
DfArray_Get(array, 1, &retrieved);
printf("Retrieved value: %d\n", retrieved);

int value = 42;
DfArray_Push(array, &value);
int popped;
DfArray_Pop(array, &popped);
printf("Popped value: %d\n", popped);

int value2 = 25;
DfArray_Unshift(array, &value2);
int shifted;
DfArray_Shift(array, &shifted);
printf("Shifted value: %d/n", shifted);
```

### Accessing Elements Safely
```c
int retrieved;
DfArray_Get(array, 0, &retrieved);
printf("Retrieved value: %d\n", retrieved);
```

### Applying a Function to All Elements
```c
void printInt(void *item) {
    printf("%d\n", *(int *)item);
}
DfArray_Map(array, printInt);
```

## API Reference
### `DfArray* DfArray_Create(size_t elem_size, size_t initial_capacity)`
Allocates a new dynamic array.

### `void DfArray_Destroy(DfArray* array)`
Frees memory associated with the array.

### `void DfArray_Push(DfArray* array, void *value)`
Adds an element to the end, resizing if needed.

### `void DfArray_Pop(DfArray* array, void *dest)`
Removes and retrieves the last element.

### `void DfArray_Unshift(DfArray* array, void *value)`
Adds an element to the front, resizing if needed.

### `void DfArray_Shift(DfArray* array, void *dest)`
Removes and retrieves the first element.

### `void DfArray_Set(DfArray* array, size_t index, void *value)`
Updates a given element at a specified index.

### `void DfArray_Get(DfArray* array, size_t index, void *dest)`
Retrieves an element with bounds checking.

### `void DfArray_Map(DfArray *array, void (*func)(void *))`
Applies a function to each element.
</details>

## Contributing
Currently using this as a learning experience and not looking for contributions at this time. But in the future as this expands I will update this section.

## License
Nothing yet.

