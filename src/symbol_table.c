#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symbol_table.h"

//FNV-1a 32 bit constants
static uint32_t offset_basis = 2166136261;
static uint32_t FNV_prime = 16777619;

//Empty Item
static Table_Item EMPTY_ITEM = {"", 0x0};

// FNV-1a hashing algorithm (in public domain)
static uint32_t hash_func(char *string)
{
  uint32_t hash = offset_basis;
  int i;
  for (i = 0; i < strlen(string); i++)
  {
    hash = hash ^ string[i];
    hash = hash * FNV_prime;
  }
  return hash;
}

//Very simplistic primality test
static int is_prime(int n)
{
  if (n <= 1)
  {
    return 0;
  }
  if (n % 2 == 0 && n > 2)
  {
    return 0;
  }
  int i;
  for (i = 3; i < n / 2; i += 2)
  {
    if (n % i == 0)
    {
      return 0;
    }
  }
  return 1;
}

static int nearest_prime(int minimum)
{
  int i = minimum + 1;
  while (!is_prime(i))
  {
    i++;
  }
  return i;
}

static void st_init_sized(Symbol_Table *st, int size)
{
  st->max_size = size;
  st->elements = 0;
  st->items = malloc(size * sizeof(Table_Item *));
  int i;
  for (i = 0; i < st->max_size; i++)
  {
    st->items[i] = &EMPTY_ITEM;
  }
}

static void resize(Symbol_Table *st)
{
  printf("RESIZING\n");
  int new_size = nearest_prime(st->max_size * 2);
  Symbol_Table *new_st = malloc(sizeof(Symbol_Table *));
  st_init_sized(new_st, new_size);
  int i;
  for (i = 0; i < st->max_size; i++)
  {
    Table_Item *item = st->items[i];
    if (item != &EMPTY_ITEM)
    {
      st_insert(new_st, item->label, item->memory_addr);
    }
  }

  st->max_size = new_st->max_size;
  st->elements = new_st->elements;
  st->items = new_st->items;

  st_free(new_st);
}

void st_init(Symbol_Table *st)
{
  st_init_sized(st, DEFAULT_SYMBOL_TABLE_SIZE);
}

void st_free(Symbol_Table *st)
{
  int i;
  for (i = 0; i < st->elements; i++)
  {
    Table_Item *item = st->items[i];
    if (item != &EMPTY_ITEM)
    {
      free(item->label);
      free(item);
    }
  }
  free(st);
}

void st_insert(Symbol_Table *st, char *label, uint8_t memory_addr)
{
  if (st->elements + 1 == st->max_size)
  {
    resize(st);
  }

  uint32_t index = hash_func(label) % st->max_size;
  int h = 1;
  while (st->items[index] != &EMPTY_ITEM)
  {
    //We use quadratic probing
    index = (index + h * h) % st->max_size;
    h++;
  }
  Table_Item *new_item = malloc(sizeof(Table_Item *));
  new_item->label = strdup(label);
  new_item->memory_addr = memory_addr;
  st->items[index] = new_item;

  st->elements++;
  printf("Added [%d] : %s - %d\n", index, st->items[index]->label, st->items[index]->memory_addr);
}

uint8_t st_search(Symbol_Table *st, char *label)
{
  printf("Searching for %s\n", label);
  uint32_t index = hash_func(label) % st->max_size;
  int h = 1;
  printf("Trying index %d with array_size %d\n", index, st->max_size);
  while (strcmp(st->items[index]->label, label) != 0 && st->items[index] != &EMPTY_ITEM)
  {
    //We use quadratic probing
    index = (index + h * h) % st->max_size;
    h++;
    printf("Trying index %d\n", index);
  }

  printf("Found index %d\n", index);
  if (st->items[index] != &EMPTY_ITEM)
  {
    return st->items[index]->memory_addr;
  }
  else
  {
    return -1;
  }
}

int st_remove(Symbol_Table *st, char *label)
{
  uint32_t index = hash_func(label) % st->max_size;
  int h = 1;
  while (strcmp(st->items[index]->label, label) != 0 && st->items[index] != &EMPTY_ITEM)
  {
    index = (index + h * h) % st->max_size;
    h++;
  }
  if (st->items[index] != &EMPTY_ITEM)
  {
    st->items[index] = &EMPTY_ITEM;
    st->elements--;
    return 1;
  }
  else
  {
    return 0;
  }
}

int main()
{
  printf("Test !\n");
  Symbol_Table st;
  st_init(&st);
  printf("Initialised !\n");
  char current_label[3];
  for (int i = 0; i < 13; i++)
  {
    snprintf(current_label, 3, "%02d", i);
    st_insert(&st, current_label, i);
  }
  printf("\n");
  printf("Index 2 : %s\n", st.items[2]->label);
  uint8_t addr = st_search(&st, "03");
  printf("%d\n", addr);
}