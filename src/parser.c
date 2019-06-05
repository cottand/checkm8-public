#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_SIZE_ASSUMPTION 100

/** Takes an empty list
 * and to the lsit adds every line of the program in order
 * PRE: assumes the list to be empty and uninitialised
 */
void parse_src_pass1(Assembler *asse, Parsed_src *parsed)
{
  LList *list = parsed->instructions;
  Symbol_Table *table = parsed->table;
  llist_init(list);
  //INIT TABLE TODO
  uint32_t mem_addr = 0;
  char *src = asse->src;
  char *pch;
  uint8_t to_free_size = 0;
  pch = strtok(src, "\n");
  while (pch != NULL)
  {
    llist_add_last(list, pch);
    if (pch[strlen(pch) - 1] == ':')
      st_insert(table, pch, mem_addr);
    else if (pch[0] == 'b') //if line is a branch instruction
      to_free_size++;
    mem_addr += sizeof(uint32_t);
    pch = strtok(NULL, " ,.-");
  }
  parsed->to_free = malloc(sizeof(char *) * to_free_size);
  return;
  // TODO : free() ret of get_instruction_array
}

void parsed_src_delete(Parsed_src *parsed)
{
  st_delete(parsed->table);
  llist_delete(parsed->instructions);
  free(parsed->to_free);
  free(parsed);
}
/**
void parse_src_pass2(Assembler *asse, Parsed_src *parsed)
{
  LList *list = parsed->instructions;
  Symbol_Table *table = parsed->table;
  for (uint8_t i = 0; i < list->size; i++)
  {
    char *current = llist_peek(parsed->instructions, i);
    //If line is a branch instruction
    if (current[strlen(current) - 1] != ':' && current[0] == 'b')
    {
      char *to_be_replaced;
      uint32_t addr;
      if (current[1] == ' ') 
      {
        to_be_replaced = &current[2];
      }
      else
      {
        to_be_replaced = &current[4];
      }
      //GET GROM SYMBOL TABLE TODO
      addr = st_search(table,to_be_replaced);
      char* 
      char* new_line;
    }
  }
}
*/
