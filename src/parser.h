#ifndef PARSER_H_
#define PARSER_H_

#include "symbol_table.h"
#include "llist.h"

typedef struct s_Parser
{
  Symbol_Table *labels;
  LList *token_streams;
} Parser;

char *parse(char *file);

void parser_parse1(Parser *parser, char *file);
void parser_parse2(Parser *parser);

void parser_init(Parser *parser);
void parser_free(Parser *parser);

#endif /* PARSER_H_ */
