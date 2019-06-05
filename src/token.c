#include "token.h"
#include <stdlib.h>
#include <stdio.h>

void token_init(Token *tok)
{
  tok->symb  = Undefined;
  tok->next  = 0;
  tok->value = 0;
}

void token_free(Token *tok)
{
  if (tok->value)
  {
    free(tok->value);
  }
}

void token_error(Token *tok, char *msg)
{
  printf("Token error (line %d): %s\n", tok->line + 1, msg);
  token_print(tok);
}

void token_print(Token *tok)
{
  printf("Token -> symbol: %d | value: %s | next: %p\n"
      , tok->symb, tok->value, (void *) tok->next);
}
