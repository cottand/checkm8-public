#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"
#include "execute.h"
#include "decode.h"
#include "token_stream.h"

int main(int argc, char **argv)
{
  char *str = "mov r0,#1";

  Token_Stream stream;
  token_stream_init(&stream);

  token_stream_tokenize(&stream, str);

  token_stream_print(&stream);

  token_stream_free(&stream);

  return EXIT_SUCCESS;
}
