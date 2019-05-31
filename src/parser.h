#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include "symbol_table.h"
#include "assembler.h"

typedef struct s_Parsed_src
{
  Symbol_Table table;
  char **instructions;
} Parsed_src;

Parsed_src parse_src(Assembler *asse);

#endif /* PARSER_H_ */