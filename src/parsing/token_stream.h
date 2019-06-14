#ifndef TOKEN_STREAM_H_
#define TOKEN_STREAM_H_

#include <stdint.h>
#include "token.h"

typedef struct s_Token_Stream
{
  Token *first_tok;
  Token *curr_tok;
} Token_Stream;

void token_stream_init(Token_Stream *stream);
void token_stream_free(Token_Stream *stream);

void token_stream_tokenize(Token_Stream *stream, char *str, uint8_t line);

Token *token_stream_peek(Token_Stream *stream);
Token *token_stream_read(Token_Stream *stream);
Token *token_stream_expect(Token_Stream *stream, symbol symb, char *error_msg);

uint8_t token_stream_tokens_left(Token_Stream *stream);

uint8_t token_stream_tokenize_char(char **str, Token *tok);
uint8_t token_stream_tokenize_register(char **str, Token *tok);
uint8_t token_stream_tokenize_immediate(char **str, Token *tok);
uint8_t token_stream_tokenize_address(char **str, Token *tok);
uint8_t token_stream_tokenize_constant(char **str, Token *tok);
uint8_t token_stream_tokenize_opcode(char **str, Token *tok);

char* token_stream_parse_hex(char **str);

void token_stream_print(Token_Stream *stream);

#endif /* TOKEN_STREAM_H_ */
