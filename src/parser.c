#include "parser.h"

void parser_init(Parser *parser)
{
  parser->symbol_table = st_create();
}

void parser_free(Parser *parser)
{
  st_delete(parser_free);
}

char *parse(char *file)
{
  Parser *parser = malloc(sizeof(Parser));
  parser_init(parser);

  parser_parse1(parser, file);
}

void parser_parse1(Parser *parser, char *file)
{
  char *line = strtok(file, "\n");

  while (line)
  {
    line = strtok(0, "\n");
  }
}

void parser_parse2(Parser *parser)
{
}
