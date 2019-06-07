#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "execute.h"
#include "decode.h"
#include "token_stream.h"
#include "symbol_table.h"

int main(int argc, char **argv)
{
 // char *str = "mov r0,#1";
  Symbol_Table table;
  st_init(&table);
  st_insert(&table, "Hello", (uint8_t) 8);
  uint8_t ret = st_search(&table, "Hello");
  printf("\n the num is: %x \n", ret);
  st_free(&table);


  return EXIT_SUCCESS;
}
