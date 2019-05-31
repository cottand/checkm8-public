#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "execute.h"
#include "decode.h"
#include "dyn_llist.h"
#include <assert.h>

int main(int argc, char **argv)
{
  LList list;
  llist_init(&list);
  llist_add_last(&list, "hello");
  llist_add_last(&list,"there");
  char * str = llist_remove_first(&list);
  printf("String: %s\n", str);
  str = llist_remove_first(&list);
  printf("String: %s\n", str);
  return EXIT_SUCCESS;
}
