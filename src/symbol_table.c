#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "symbol_table.h"

static Symbol_Item ST_DELETED_ITEM = {NULL, -1};

static Symbol_Item *st_new_item(char *label, int memory_addr)
{
    Symbol_Item *item = malloc(sizeof(Symbol_Item *));
    item->label = strdup(label);
    item->memory_addr = memory_addr;
    return item;
}

static void st_delete_item(Symbol_Item *item)
{
    free(item->label);
    free(item);
}

static int st_hash(char *s, int a, int m)
{
    long hash = 0;
   // const int len_s = strlen(s);
   /* for (int i = 0; i < len_s; i++)
    {
        hash += (long)pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    } */ //TODO NOT USE long :) or pow really
    return (int)hash;
}

static int st_get_hash(char *s, int num_buckets, int attempt)
{
    int hash_a = st_hash(s, 131, num_buckets);
    int hash_b = st_hash(s, 163, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

Symbol_Table *st_create(void)
{
    Symbol_Table *st = malloc(sizeof(Symbol_Item *));

    st->size = DEFAULT_SYMBOL_TABLE_SIZE;
    st->count = 0;
    st->items = calloc(st->size, sizeof(Symbol_Item *));
    return st;
}

void st_delete(Symbol_Table *st)
{
    for (int i = 0; i < st->size; i++)
    {
        Symbol_Item *item = st->items[i];
        if (item != NULL)
        {
            st_delete_item(item);
        }
    }
    free(st->items);
    free(st);
}

void st_insert(Symbol_Table *st, char *label, int memory_addr)
{
    Symbol_Item *item = st_new_item(label, memory_addr);
    int index = st_get_hash(item->label, st->size, 0);
    Symbol_Item *cur_item = st->items[index];
    int i = 1;
    while (cur_item != NULL)
    {
        if (cur_item != &ST_DELETED_ITEM)
        {
            if (strcmp(cur_item->label, label) == 0)
            {
                st_delete_item(cur_item);
                st->items[index] = item;
                return;
            }

            index = st_get_hash(item->label, st->size, i);
            cur_item = st->items[index];
            i++;
        }
    }
    st->items[index] = item;
    st->count++;
}

int st_search(Symbol_Table *st, char *label)
{
    int index = st_get_hash(label, st->size, 0);
    Symbol_Item *item = st->items[index];
    int i = 1;
    while (item != NULL)
    {
        if (item != &ST_DELETED_ITEM)
        {
            if (strcmp(item->label, label) == 0)
            {
                return item->memory_addr;
            }
        }
        index = st_get_hash(label, st->size, i);
        item = st->items[index];
        i++;
    }
    return -1;
}

void st_remove(Symbol_Table *st, char *label)
{
    int index = st_get_hash(label, st->size, 0);
    Symbol_Item *item = st->items[index];
    int i = 1;
    while (item != NULL)
    {
        if (item != &ST_DELETED_ITEM)
        {
            if (strcmp(item->label, label) == 0)
            {
                st_delete_item(item);
                st->items[index] = &ST_DELETED_ITEM;
            }
        }
        index = st_get_hash(label, st->size, i);
        item = st->items[index];
        i++;
    }
    st->count--;
}
