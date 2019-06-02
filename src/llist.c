#include "llist.h"
#include <stdio.h>

const uint16_t NODE_SIZE = sizeof(char *) * 3;

void llist_init(LList *list)
{
  list->size = 0;
}

void llist_node_delete(Node *node)
{
  free(node);
}

void llist_add_last(LList *list, char *str)
{
  if (list->size == 0)
  {
    list->first = malloc(NODE_SIZE);
    list->last = list->first;
    list->first->str = str;
  }
  else
  {
    Node *new_last = malloc(NODE_SIZE);
    new_last->str = str;
    list->last->next = new_last;
    list->last = new_last;
    list->last->next = 0;
  }
  list->size++;
}

void llist_remove(LList *list, uint8_t n)
{
  if (list->size == 0)
  {
    printf("LList: tried to remove nth element from empty list");
    return;
  }
  list->size--;
  Node *current = list->first;
  Node *prev;
  int i = 1;
  for (; i < n; i++)
  {
    prev = current;
    current = current->next;
    if (current == 0)
      perror("llist_remove_nth: out of bounds");
  }
  prev->next = current->next;
  llist_node_delete(current);
}
/**
 * removes first element of list and returns removed element,
 * allows easy iteration and memory freeing.
 * Returns 0 if list is empty.
 */
char *llist_remove_first(LList *list)
{
  if (list->size == 0)
  {
    printf("LList: Tried to remove element from empty list");
    return 0;
  }

  Node *old_first = list->first;
  list->first = old_first->next;
  char *str = old_first->str; //segfault here TODO debug
  list->size--;
  llist_node_delete(old_first);
  return str;
}

void llist_delete(LList *list)
{
  while (list->size > 0)
  {
    llist_remove_first(list);
  }
  free(list);
}

char *llist_peek(LList *list, uint8_t n)
{
  int i = 0;
  Node *current = list->first;
  for (; i < n; i++)
  {
    current = current->next;
  }
  return current->str;
}
