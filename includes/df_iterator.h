#ifndef DF_ITERATOR_H
#define DF_ITERATOR_H

typedef struct Iterator {
    void *structure;                   // Pointer to the data structure
    void *current;                      // Current element (varies by structure)
    void *(*next)(struct Iterator *);   // Function pointer to get the next element
    int (*has_next)(struct Iterator *); // Function to check if more elements exist
} Iterator;

void Iterator_Destroy(Iterator *it);

#endif
