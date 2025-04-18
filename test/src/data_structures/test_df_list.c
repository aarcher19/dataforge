// #include <criterion/alloc.h>
// #include <criterion/criterion.h>
// #include <criterion/internal/assert.h>
// #include <criterion/internal/test.h>
// #include <criterion/logging.h>
// #include <stdio.h>
// #include "../../../includes/df_list_s.h"

// typedef struct DfList_S
// {
//   DfList_S_Node *head;
//   DfList_S_Node *tail;
//   size_t length;
// } DfList_S;

// typedef struct DfList_S_Node
// {
//   void *element;
//   struct DfList_S_Node *next;
//   size_t elem_size;
// } DfList_S_Node;

// void cleanUp(void *element)
// {
//   // conditional freeing logic would go here for list nodes
//   if (*(int *)element == 10)
//     free(element);
// }

// Test(df_list_suit, create)
// {
//   DfList_S *list = dflist_s_create();
//   cr_assert(list->head == NULL, "Expected head to be NULL");
//   cr_assert(list->tail == NULL, "Expected tail to be NULL");
//   cr_assert(list->length == 0, "Expected length to be 0");

//   free(list);
// }

// Test(df_list_suit, destroy)
// {
//   DfList_S *list = dflist_s_create();
//   dflist_s_destroy(list, cleanUp);
//   // Need to implement function to add nodes to list before effective testing can be done
// }
