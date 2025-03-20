#ifndef DF_ITERATOR_H
#define DF_ITERATOR_H

#include <stdlib.h>

typedef struct Iterator {
    void *structure;                   // Pointer to the data structure
    void *current;                      // Current element (varies by structure)
    void *(*next)(struct Iterator *);   // Function pointer to get the next element
    int (*has_next)(struct Iterator *); // Function to check if more elements exist
    void *(*create_new)(struct Iterator *);          // Function to create new empty instance of ds
    void (*insert_new)(void *new_ds, void *element); // Function to insert new element into datastructure
    size_t (*elem_size)(struct Iterator *);
    void (*free_all)(struct Iterator *);
} Iterator;

void Iterator_Destroy(Iterator *it);

#endif
