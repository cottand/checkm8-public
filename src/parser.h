#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include "symbol_table.h"
#include "assembler.h"
#include "llist.h"

typedef struct s_Parsed_src
{
  Symbol_Table *table;
  LList *instructions;
  char* to_free;
} Parsed_src;

/**
 * PRE: List in Parsed_src uninitialised
 */
void parse_src_pass1(Assembler *asse, Parsed_src *parsed);
void parse_src_pass2(Assembler *asse, Parsed_src *parsed);
void parsed_src_delete(Parsed_src *parsed);

#endif /* PARSER_H_ */