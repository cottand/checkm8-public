#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symbol_table.h"

//FNV-1a 32 bit constants
static uint32_t offset_basis = 2166136261;
static uint32_t FNV_prime = 16777619;

//Empty Item
static Table_Item EMPTY_ITEM = {"", 0x0};

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
  int i;
  for (i = 0; i < st->max_size; i++)
  {
    st->items[i] = &EMPTY_ITEM;
  }
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

void st_insert(Symbol_Table *st, char *label, uint8_t memory_addr)
{
  if (st->elements == st->max_size)
  {
    //RESIZE?
    printf("Symbol Table is full");
    return;
  }

  uint32_t index = hash_func(label) % st->max_size;
  while (st->items[index] != &EMPTY_ITEM)
  {
    //We use linear probing
    index = index + 1 % st->max_size;
  }
  Table_Item new_item = {label, memory_addr};
  st->items[index] = &new_item;
}

uint8_t st_search(Symbol_Table *st, char *label)
{
  uint32_t index = hash_func(label) % st->max_size;
  while(strcmp(st->items[index]->label, label) != 0 && st->items[index] != &EMPTY_ITEM)
  {
    index = index + 1 % st->max_size;
  }
  if(st->items[index] != &EMPTY_ITEM)
  {
    return st->items[index]->memory_addr;
  }
  else
  {
    return -1;
  }
  
}

void st_remove(Symbol_Table *st, char *label)
{
}
