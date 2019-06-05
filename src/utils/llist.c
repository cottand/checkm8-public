#include "llist.h"
#include <stdio.h>

void node_init(Node *node)
{
  node->next = 0;
  node->elem = 0;
}

void llist_init(LList *list)
{
  /* We init the list with a dummy node */

  list->first = malloc(sizeof(Node));
  node_init(list->first);
  list->last = list->first;

  list->size = 1;
}

void llist_free(LList *list)
{
  Node *node = list->first;
  while (node)
  {
    Node *next = node->next;

    free(node);

    node = next;
  }

  list->first = 0;
  list->last  = 0;
  list->size  = 0;
}

void llist_add(LList *list, void *elem)
{
  Node *new = malloc(sizeof(Node));
  node_init(new);

  new->elem = elem;

  list->last->next = new;
  list->last = new;
  list->size++;
}

void *llist_pop_first(LList *list)
{
  Node *next = list->first->next;

  void *elem = next->elem;
  list->first->next = next->next;
  free(next);

  list->size--;

  return elem;
}


void llist_remove(LList *list, uint8_t n)
{
  Node *curr = list->first;
  uint8_t i;
  for (i = 0; i < list->size; i++)
  {
    if (i == n)
    {
      Node *next = curr->next;

      curr->next = curr->next->next;
      free(next);

      if (!curr->next)
      {
        list->last = curr;
      }

      list->size--;
      return;
    }

    curr = curr->next;
  }
}

void *llist_get(LList *list, uint8_t n)
{
  Node *curr = list->first;
  uint8_t i;
  for (i = 0; i < list->size; i++)
  {
    if (i - 1 == n)
    {
      return curr->elem;
    }

    curr = curr->next;
  }

  return 0;
}

uint8_t llist_size(LList *list)
{
  return list->size - 1;
}
