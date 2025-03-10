#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>

/*🔹 Insertion & Deletion
DfArray_Insert(DfArray *array, size_t index, void *value)

Inserts a value at a specific index, shifting elements right.
DfArray_RemoveAt(DfArray *array, size_t index)

Removes an element at a given index, shifting elements left.
🔹 Searching & Finding
DfArray_Find(DfArray *array, bool (*cmp)(void *, void *), void *target)

Returns the index of the first element matching target, or -1 if not found.
Example comparator: bool int_compare(void *a, void *b) { return *(int *)a == *(int *)b; }
DfArray_Contains(DfArray *array, void *target, bool (*cmp)(void *, void *))

Returns true if target exists in the array.
🔹 Sorting & Modifications
DfArray_Sort(DfArray *array, int (*cmp)(const void *, const void *))

Sorts the array using qsort(), allowing custom comparison functions.
DfArray_Reverse(DfArray *array)

Reverses the order of elements in place.
DfArray_Filter(DfArray *array, bool (*predicate)(void *))

Removes elements that do not match the predicate function.
🔹 Metadata & Info
DfArray_IsEmpty(DfArray *array)

Returns true if the array is empty.
DfArray_Clear(DfArray *array)

Resets length to 0 without freeing the memory.
DfArray_Copy(DfArray *array)

Creates a deep copy of an array.*/

typedef struct df_array DfArray;

void DfArray_Get(DfArray* array, size_t index, void *dest) ;

DfArray* DfArray_Create(size_t elem_size, size_t initial_capcity);

void DfArray_Destroy(DfArray* array);

void DfArray_Push(DfArray* array, void *value);

void DfArray_Pop(DfArray* array, void *dest);

void DfArray_Shift(DfArray* array, void *dest);

void DfArray_Unshift(DfArray* array, void *value);

void DfArray_Map(DfArray *array, void (*func)(void *));

#endif
