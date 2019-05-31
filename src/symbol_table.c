#include <stdlib.h>
#include "symbol_table.h"

void st_init(Symbol_Table *st)
{
  st->max_size = DEFAULT_SYMBOL_TABLE_SIZE;
  st->elements = 0;
  st->items = calloc(DEFAULT_SYMBOL_TABLE_SIZE, sizeof(Table_Item *));
}

void st_free(Symbol_Table *st)
{
  int i;
  for (i = 0; i < st->elements; i++)
  {
    Table_Item *item = st->items[i];
    free(item->label);
    free(item);
  }
  free(st);
}

void st_insert(Symbol_Table *st, char *label, int memory_addr)
{
}

int st_search(Symbol_Table *st, char *label)
{
  return -1;
}

void st_remove(Symbol_Table *st, char *label)
{
}
