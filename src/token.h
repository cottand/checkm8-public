#ifndef TOKEN_H_
#define TOKEN_H_

#include "symbol.h"

typedef struct s_Token
{
  symbol symb;
  char *value;

  struct s_Token *next;
} Token;

void token_init(Token *tok);
void token_free(Token *tok);

void token_error(Token *tok, char *msg);

void token_print(Token *tok);

#endif /* TOKEN_H_ */
