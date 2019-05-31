#include "dyn_llist.h"

const uint16_t NODE_SIZE = sizeof(char *) * 3;

void llist_add_last(LList *list, char *str)
{
  if (list->empty == 1)
  {
    list->first = malloc(NODE_SIZE);
    list->last = list->first;
    list->first->str = str;
    list->empty = 0;
  }
  else
  {
    Node *new_last = malloc(NODE_SIZE);
    list->last->next = new_last;
    list->last = new_last;
    list->last->next = 0;
  }
}

void llist_remove_nth(LList *list, uint8_t n)
{
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
  free(current);
}

/**
 * removes first element of list and returns removed element,
 * allows easy iteration and memory freeing.
 * Returns 0 if list is empty.
 */
char *llist_remove_first(LList *list)
{
  if (list->empty == 1)
  {
    return 0;
  }

  if (list->last == list->first)
  {
    list->empty = 1;
  }
  Node *old_first = list->first;
  list->first = old_first->next;
  char *str = old_first->str;
  free(old_first);
  return str;
}

void llist_destroy(LList *list)
{
  while(list->empty != 0)
  {
    llist_remove_first(list);
  }
  free (list);
}