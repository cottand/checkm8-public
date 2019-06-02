#include <stdlib.h>
#include <string.h>


#include "symbol_table.h"

//FNV-1a 32 bit constants
static uint32_t offset_basis = 2166136261;
static uint32_t FNV_prime = 16777619;

static uint32_t hash_func(char *string)
{
  // FNV-1a hashing algorithm (in public domain)
  uint32_t hash = offset_basis;
  int i;
  for (i = 0; i < strlen(string); i++)
  {
    hash = hash ^ string[i];
    hash = hash * FNV_prime;
  }
  return hash;
}


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

int main()
{}