#include <stdlib.h>
#include "assembler.h"

int main(int argc, char **argv)
{
  assemble(argv[1], argv[2]);

  return EXIT_SUCCESS;
}
