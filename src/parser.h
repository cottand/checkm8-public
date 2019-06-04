#ifndef PARSER_H_
#define PARSER_H_

#include "symbol_table.h"
#include "token_stream.h"
#include "llist.h"

typedef struct s_Parser
{
  Symbol_Table *labels;
  LList tokenized_lines;
  LList constants;
} Parser;

void parse(void *src, void **output, size_t *output_size);

void parser_parse1(Parser *parser, char *src);
void parser_parse2(Parser *parser, void **output, size_t *output_size);

void parser_init(Parser *parser);
void parser_free(Parser *parser);

void parser_do_substitutions(Parser *parser, Token_Stream *tokens, uint8_t line);
void parser_substitute_for_branch(Parser *parser, Token_Stream *tokens, uint8_t line);
void parser_substitute_for_constant(Parser *parser, Token_Stream *tokens, uint8_t line);

void parser_check_for_constant(Parser *parser, Token_Stream *tokens);
uint8_t parser_check_for_label(Parser *parser, Token_Stream *tokens, uint8_t line);


#endif /* PARSER_H_ */
