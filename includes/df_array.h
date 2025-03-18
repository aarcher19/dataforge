#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include "df_iterator.h"

/*🔹 Insertion & Deletion
DfArray_InsertAt(DfArray *array, size_t index, void *value)
Inserts a value at a specific index, shifting elements right.

DfArray_RemoveAt(DfArray *array, size_t index)
Removes an element at a given index, shifting elements left.
*/

typedef struct DfArray DfArray;

DfArray* DfArray_Create(size_t elem_size, size_t initial_capcity);

void DfArray_Destroy(DfArray* array);

void *DfArray_Get(DfArray* array, size_t index);

void DfArray_Push(DfArray* array, void *value);

void *DfArray_Pop(DfArray* array);

void *DfArray_Shift(DfArray* array);

void DfArray_Unshift(DfArray* array, void *value);

void DfArray_Set(DfArray* array, size_t index, void *value);

void DfArray_InsertAt(DfArray *array, size_t index, void *value);

void DfArray_RemoveAt(DfArray *array, size_t index);

// Will be removed after iterators and utils are implemented!
void DfArray_Map(DfArray *array, void (*func)(void *));

// Iterator
typedef struct DfArray_Iterator DfArray_Iterator;

Iterator DfArray_Iterator_Create(DfArray *array);

int DfArray_Iterator_Has_Next(Iterator *it);

void *DfArray_Iterator_Next(Iterator *it);

#endif
