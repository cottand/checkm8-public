#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#define DEFAULT_SYMBOL_TABLE_SIZE 13

typedef struct s_Symbol_Item
{
    char *label;
    int memory_addr;
} Symbol_Item;

typedef struct s_Symbol_Table
{
    int size;
    int count;
    Symbol_Item **items;
} Symbol_Table;

Symbol_Table *st_create(void);
void st_delete(Symbol_Table *st);
void st_insert(Symbol_Table *st, char *label, int memory_addr);
int st_search(Symbol_Table *st, char *label);
void st_remove(Symbol_Table *st, char *label);

#endif /* SYMBOL_TABLE_H_ */
