#include "token_stream.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void token_stream_init(Token_Stream *stream)
{
  stream->first_tok = NULL;
  stream->curr_tok  = NULL;
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

void token_stream_tokenize(Token_Stream *stream, char *str)
{
  Token *curr = malloc(sizeof(Token));
  token_init(curr);

  /* First token acts as dummy token so curr_tok points to next */
  stream->first_tok = curr;
  stream->curr_tok  = stream->first_tok->next;

  while (*str)
  {
    curr->next = malloc(sizeof(Token));
    curr = curr->next;
    token_init(curr);

    if (token_stream_tokenize_char(&str, curr))      { continue; }
    if (token_stream_tokenize_register(&str, curr))  { continue; }
    if (token_stream_tokenize_constant(&str, curr))  { continue; }
    if (token_stream_tokenize_opcode(&str, curr))    { continue; }

    printf("Error in tokenizing: %s", str);
    return;
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
    tok->symb  = LBracket;
    tok->value = malloc(sizeof(char));
    strcpy(tok->value, "[");

    return 1;
  }

  if (**str == ']')
  {
    tok->symb  = RBracket;
    tok->value = malloc(sizeof(char));
    strcpy(tok->value, "]");

    return 1;
  }

  return 0;
}

uint8_t token_stream_tokenize_register(char **str, Token* tok)
{
  /* Registers are of the form rNumber */
  if (**str == 'r' && isdigit(*(*str + 1)))
  {
    uint8_t char_count = 1;
    (*str)++;

    while (isdigit(**str))
    {
      char_count++;
      (*str)++;
    }

    tok->symb  = Register;
    tok->value = malloc(sizeof(char) * char_count);

    size_t size = char_count * sizeof(char);
    strncpy(tok->value, *str - size, size);

    return 1;
  }

  return 0;
}

uint8_t token_stream_tokenize_constant(char **str, Token *tok)
{
  if (**str == '#')
  {
    uint8_t char_count = 0;
    (*str)++;

    while (isdigit(**str) || **str == 'x')
    {
      char_count++;
      (*str)++;
    }

    tok->symb  = Immediate;
    tok->value = malloc(sizeof(char) * char_count);

    size_t size = char_count * sizeof(char);
    strncpy(tok->value, *str - size, size);

    return 1;
  }

  return 0;
}

uint8_t token_stream_tokenize_opcode(char **str, Token *tok)
{
  if (isalpha(**str))
  {
    uint8_t char_count = 0;
    while (isalpha(**str))
    {
      char_count++;
      (*str)++;
    }

    tok->symb  = Opcode;
    tok->value = malloc(sizeof(char) * char_count);

    size_t size = char_count * sizeof(char);
    strncpy(tok->value, *str - size, size);
  }

  return 0;
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
