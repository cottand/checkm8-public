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

void token_print(Token *tok)
{
  printf("Token -> symbol: %d | value: %s | next: %p"
      , tok->symb, tok->value, (void *) tok->next);
}
