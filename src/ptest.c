#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "execute.h"
#include "decode.h"
#include "llist.h"
#include <assert.h>

int main(int argc, char **argv)
{
  LList list;
  llist_init(&list);
  llist_add_last(&list, "hello");
  llist_add_last(&list,"there");
  llist_add_last(&list, "beaut");
  char * str = llist_peek(&list, 0);
  printf("String: %s\n", str);
  str = llist_peek(&list, 2);
  printf("String: %s\n", str);
  return EXIT_SUCCESS;
}
