#ifndef DF_ITERATOR_H
#define DF_ITERATOR_H

#include <stdlib.h>
#include "df_common.h"

typedef struct Iterator
{
    void *structure; // Pointer to the data structure
    void *current;   // Current element (varies by structure)

    DfResult (*next)(struct Iterator *);                 // Get the next element and wrap in DfResult
    int (*has_next)(struct Iterator *);                  // Return DfResult with bool (int) payload
    DfResult (*create_new)(struct Iterator *);           // Return new empty instance of ds
    DfResult (*insert_new)(void *new_ds, void *element); // Insert an element into the new ds
    size_t (*elem_size)(struct Iterator *);              // Return size_t for elements
    DfResult (*free_all)(struct Iterator *);             // Free the iterator and all resources
} Iterator;

DfResult iterator_destroy(Iterator *it);

#endif
