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
  void *elem;
} Node;

typedef struct s_LList
{
  Node *first;
  Node *last;
  uint8_t size;
} LList;

void llist_add(LList *list, void *elem);
void *llist_pop_first(LList *list);
void llist_remove(LList *list, uint8_t n);
void *llist_get(LList *list, uint8_t n);

void llist_init(LList *list);
void llist_free(LList *list);

uint8_t llist_size(LList *list);

void node_init(Node *node);

#endif /* LLIST_H_ */
