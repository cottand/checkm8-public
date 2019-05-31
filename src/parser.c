#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_SIZE_ASSUMPTION 100

/**
 * Returns an array of strings where each string is a line of the
 * program, and the first element of the array encodes the number of
 * of lines in the program
 */
// TODO : test
char **get_instruction_array(Assembler *asse)
{
  char *src = asse->src;
  char **array_of_strings = malloc(BIN_SIZE_ASSUMPTION * sizeof(char *));
  char *pch;
  pch = strtok(src, "\n");
  register int i = 1;
  while (pch != NULL)
  {
    array_of_strings[i++] = pch;
    pch = strtok(NULL, " ,.-");
  }
  array_of_strings[0] = (char *)i - 1;
  return array_of_strings;
  // TODO : free() ret of get_instruction_array
}
//TODO parse replace and add $ before 0x0....
Symbol_Table get_table(Assembler *asse, char **instr_array)
{
  Symbol_Table table;
  uint16_t prog_size = (uint16_t)instr_array[0];
  uint32_t i = 1;
  uint32_t mem_addr = 0;
  for (i = 1; i <= prog_size; i++)
  {
    char *current = instr_array[i];
    //If current line is a label
    if (current[strlen(current) -1 ] == ':')
    {
        st_insert(&table, current, mem_addr);
    }
    mem_addr += sizeof(uint32_t);
  }
}

Parsed_src parse_src(Assembler *asse)
{

  char tempString[511];
}