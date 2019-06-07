#include "parser.h"
#include "token_stream.h"
#include "instruction.h"
#include "encode.h"
#include <stdio.h>
#include <string.h>

void parser_init(Parser *parser)
{
  st_init(&parser->labels);
  llist_init(&parser->tokenized_lines);
  llist_init(&parser->constants);
}

void parser_free(Parser *parser)
{
  st_free(&parser->labels);
  llist_free(&parser->tokenized_lines);
  llist_free(&parser->constants);
}

void parse(void *src, void **output, size_t *output_size)
{
  Parser parser;
  parser_init(&parser);

  parser_parse1(&parser, src);
  parser_parse2(&parser, output, output_size);

  parser_free(&parser);
}

void parser_parse1(Parser *parser, char *file)
{
  char *line = strtok(file, "\n");
  uint8_t line_nb = 0;

  while (line)
  {
    Token_Stream *stream = malloc(sizeof(Token_Stream));
    token_stream_tokenize(stream, line, line_nb);

    parser_check_for_constant(parser, stream);

    if (!parser_check_for_label(parser, stream, line_nb))
    {
      llist_add(&parser->tokenized_lines, stream);
    }

    line = strtok(0, "\n");
    line_nb++;
  }
}

void parser_parse2(Parser *parser, void **output, size_t *output_size)
{
  uint8_t total_lines  = llist_size(&parser->tokenized_lines);
  uint8_t total_consts = llist_size(&parser->constants);

  *output_size = (total_lines + total_consts) * sizeof(uint32_t);
  *output = malloc(*output_size);

  int i;
  for (i = 0; i < total_lines; i++)
  {
    Token_Stream *stream = llist_pop_first(&parser->tokenized_lines);

    parser_substitute_for_branch(parser, stream, i);
    parser_substitute_for_constant(parser, stream, i, total_lines);

    Instr encoded   = encode_instr(stream);
    uint32_t binary = instr_to_uint32(&encoded);
    instr_free(&encoded);
    memcpy((char *) *output + i * sizeof(uint32_t), &binary, sizeof(uint32_t));

    token_stream_print(stream);

    token_stream_free(stream);
    free(stream);
  }

  /* Add all constants at the end of the file */
  for (i = total_lines; i < total_lines + total_consts; i++)
  {
    uint32_t *constant = llist_pop_first(&parser->constants);

    memcpy((char *) *output + i * sizeof(uint32_t), constant, sizeof(uint32_t));

    free(constant);
  }
}

void parser_substitute_for_branch(Parser *parser, Token_Stream *tokens, uint8_t line)
{
  Token *opcode = token_stream_peak(tokens);

  if (opcode && opcode->value[0] == 'b')
  {
    Token *label = opcode->next;

    if (!label || label->symb != Opcode)
    {
      return;
    }

    uint8_t label_line = st_search(&parser->labels, label->value);

    /* Compute offset taking into account the pipeline offset */
    int32_t offset = (label_line - line) * sizeof(uint32_t);
    offset -= 2 * sizeof(uint32_t);
    offset >>= 2;

    /* Realloc enough space to store hex like 0x12345678 */
    label->value = realloc(label->value, 11 * sizeof(char));
    sprintf(label->value, "0x%x", offset);

    label->symb = Address;
  }
}

void parser_substitute_for_constant(Parser *parser, Token_Stream *tokens, uint8_t line, uint8_t total_lines)
{
  Token *opcode = token_stream_peak(tokens);

  if (opcode && !strcmp(opcode->value, "ldr") && opcode->next->next->symb == Constant)
  {
    Token *reg = opcode->next;
    Token *constant = reg->next;

    uint8_t const_no;
    memcpy(&const_no, constant->value, sizeof(uint8_t));

    uint8_t offset = (total_lines + const_no - line) * sizeof(uint32_t);
    offset -= 2 * sizeof(uint32_t);

    /* Now reformat the instruction */
    Token *lbracket = malloc(sizeof(Token));
    token_init(lbracket);
    lbracket->symb = LBracket;
    lbracket->value = malloc(sizeof(char) * 2);
    strcpy(lbracket->value, "[");

    Token *rbracket = malloc(sizeof(Token));
    token_init(rbracket);
    rbracket->symb = RBracket;
    rbracket->value = malloc(sizeof(char) * 2);
    strcpy(rbracket->value, "]");

    Token *pc = malloc(sizeof(Token));
    token_init(pc);
    pc->symb = Register;
    pc->value = malloc(sizeof(char) * 3);
    strcpy(pc->value, "15");

    constant->symb = Address;
    constant->value = realloc(constant->value, sizeof(char) * 11);
    sprintf(constant->value, "0x%x", offset);

    reg->next = lbracket;
    lbracket->next = pc;
    pc->next = constant;
    constant->next = rbracket;
  }
}

void parser_check_for_constant(Parser *parser, Token_Stream *tokens)
{
  Token *opcode = token_stream_peak(tokens);

  if (opcode && !strcmp(opcode->value, "ldr"))
  {
    Token *constant = opcode->next->next;

    if (!constant || constant->symb != Constant)
    {
      return;
    }

    uint32_t const_val = strtoul(constant->value, 0, 16);

    /* If the constant is 2 bytes we can treat it as a mov */
    if (const_val <= 0xff)
    {
      /* Turn the instr into an immediate mov */
      strcpy(opcode->value, "mov");
      constant->symb = Immediate;

      return;
    }
    else
    {
      /* The constant is too big so we add it to the list of constants
       * which will be stored at the end of the file */

      /* Add the constant to the list of constants */
      uint32_t *pconst = malloc(sizeof(uint32_t));
      memcpy(pconst, &const_val, sizeof(uint32_t));
      llist_add(&parser->constants, pconst);

      /* Value will now carry the constant number (eg const no 0, 1 etc) */
      constant->value = realloc(constant->value, sizeof(uint8_t));
      uint8_t const_no = llist_size(&parser->constants) - 1;
      memcpy(constant->value, &const_no, sizeof(uint8_t));
    }
  }
}

uint8_t parser_check_for_label(Parser *parser, Token_Stream *tokens, uint8_t line)
{
  Token* label = token_stream_peak(tokens);

  if (label->next && label->next->symb == Colon)
  {
    st_insert(&parser->labels, label->value, line);
    return 1;
  }

  return 0;
}
