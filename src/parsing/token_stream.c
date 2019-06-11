#include "token_stream.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void token_stream_init(Token_Stream *stream)
{
  stream->first_tok = NULL;
  stream->curr_tok = NULL;
}

void token_stream_free(Token_Stream *stream)
{
  Token *tok = stream->first_tok;

  while (tok)
  {
    Token *next = tok->next;

    token_free(tok);
    free(tok);

    tok = next;
  }
}

void token_stream_tokenize(Token_Stream *stream, char *str, uint8_t line)
{
  Token *curr = malloc(sizeof(Token));
  token_init(curr);
  curr->line = line;

  Token *prev = NULL;

  /* First token acts as dummy token */
  stream->first_tok = curr;
  stream->curr_tok = stream->first_tok;

  while (*str)
  {
    curr->next = malloc(sizeof(Token));
    prev = curr;
    curr = curr->next;
    token_init(curr);
    curr->line = line;

    if (token_stream_tokenize_char(&str, curr))
    {
      continue;
    }
    if (token_stream_tokenize_register(&str, curr))
    {
      continue;
    }
    if (token_stream_tokenize_immediate(&str, curr))
    {
      continue;
    }
    if (token_stream_tokenize_address(&str, curr))
    {
      continue;
    }
    if (token_stream_tokenize_constant(&str, curr))
    {
      continue;
    }
    if (token_stream_tokenize_opcode(&str, curr))
    {
      continue;
    }

    free(curr->next);
    curr = prev;
  }
}

/* Tokenizes single chars such as , [ ]  and white spaces */
uint8_t token_stream_tokenize_char(char **str, Token *tok)
{
  /* Skip without tokenising */
  if (**str == ' ' || **str == '\t' || **str == '\n' || **str == ',')
  {
    (*str)++;
    return 0;
  }

  if (**str == '[')
  {
    tok->symb = LBracket;
    tok->value = malloc(sizeof(char) * 2);
    strcpy(tok->value, "[");

    (*str)++;

    return 1;
  }

  if (**str == ']')
  {
    tok->symb = RBracket;
    tok->value = malloc(sizeof(char) * 2);
    strcpy(tok->value, "]");

    (*str)++;

    return 1;
  }

  if (**str == ':')
  {
    tok->symb = Colon;
    tok->value = malloc(sizeof(char) * 2);
    strcpy(tok->value, ":");

    (*str)++;

    return 1;
  }

  return 0;
}

uint8_t token_stream_tokenize_register(char **str, Token *tok)
{
  /* Registers are of the form rNumber */
  if (**str == 'r' && isdigit(*(*str + 1)))
  {
    uint8_t char_count = 0;
    (*str)++;

    while (isdigit(**str))
    {
      char_count++;
      (*str)++;
    }

    tok->symb = Register;
    tok->value = calloc(char_count + 1, sizeof(char) );

    size_t size = char_count * sizeof(char);
    strncpy(tok->value, *str - size, size);

    return 1;
  }

  /* If register is of form PC, replace it with r15 */
  if (!strncmp(*str, "PC", 2))
  {
    tok->symb = Register;
    tok->value = malloc(sizeof(char) * 3);

    /* PC is reg 15 */
    strcpy(tok->value, "15");

    (*str) += 2;

    return 1;
  }

  return 0;
}

uint8_t token_stream_tokenize_immediate(char **str, Token *tok)
{
  if (**str == '#')
  {
    (*str)++;

    tok->symb = Immediate;
    tok->value = token_stream_parse_hex(str);

    return 1;
  }

  return 0;
}

uint8_t token_stream_tokenize_address(char **str, Token *tok)
{
  if (**str == '$')
  {
    (*str)++;

    tok->symb = Address;
    tok->value = token_stream_parse_hex(str);

    return 1;
  }

  return 0;
}

uint8_t token_stream_tokenize_constant(char **str, Token *tok)
{
  if (**str == '=')
  {
    (*str)++;

    tok->symb = Constant;
    tok->value = token_stream_parse_hex(str);

    return 1;
  }

  return 0;
}

uint8_t token_stream_tokenize_opcode(char **str, Token *tok)
{
  if (isalpha(**str))
  {
    uint8_t char_count = 0;
    while (isalnum(**str))
    {
      char_count++;
      (*str)++;
    }

    tok->symb = Opcode;
    tok->value = calloc(char_count + 1, sizeof(char));

    size_t size = char_count * sizeof(char);
    strncpy(tok->value, *str - size, size);

    return 1;
  }

  return 0;
}

char *token_stream_parse_hex(char **str)
{
  uint8_t char_count = 0;

  while (isxdigit(**str) || **str == 'x' || **str == '-' || **str == '+')
  {
    char_count++;
    (*str)++;
  }

  char *ret = malloc(sizeof(char) * char_count + 1);

  size_t size = char_count * sizeof(char);
  strncpy(ret, *str - size, size);

  return ret;
}

Token *token_stream_peak(Token_Stream *stream)
{
  return stream->curr_tok->next;
}

Token *token_stream_read(Token_Stream *stream)
{
  Token *ret = token_stream_peak(stream);
  stream->curr_tok = ret;

  return ret;
}

Token *token_stream_expect(Token_Stream *stream, symbol symb, char *error_msg)
{
  Token *tok = token_stream_read(stream);

  if (tok && tok->symb == symb)
  {
    return tok;
  }

  token_error(tok, error_msg);

  return 0;
}

uint8_t token_stream_tokens_left(Token_Stream *stream)
{
  uint8_t count = 0;

  Token *curr = stream->curr_tok;
  while (curr->next)
  {
    curr = curr->next;
    count++;
  }

  return count;
}

void token_stream_print(Token_Stream *stream)
{
  if (!stream->first_tok)
  {
    return;
  }

  printf("Token stream:\n");
  Token *curr = stream->first_tok->next;

  if (!curr)
  {
    return;
  }

  while (curr)
  {
    token_print(curr);
    curr = curr->next;
  }
}
