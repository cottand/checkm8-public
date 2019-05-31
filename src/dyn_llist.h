/**
 * Dynamic linked list implementation that will allow
 * sort of a dynamic array-like sized data structure that
 * at the same time is easy to iterate through
 */

#ifndef LLIST_H_
#define LLIST_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct s_Node
{
  struct s_Node *next;
  char *str;
} Node;

typedef struct s_LList
{
  Node *first;
  Node *last;
  uint8_t empty;
} LList;

char *llist_remove_first(LList *list);
void llist_add_last(LList *list, char *str);
void llist_remove_nth(LList *list, uint8_t n);
void llist_destroy(LList *list);
void llist_init(LList *list);

#endif /* DYN_ARRAY_H_ */