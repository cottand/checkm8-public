#include <stdio.h>
#include <stdlib.h>

void write(char path[], void *ptr)
{
  FILE *file = fopen(path, "wb");
  if (!file)
  {
    fprintf(stderr, "Error: Invalid file path\n");
    exit(0);
  }

  fwrite(ptr, sizeof(ptr), 1, file);

  fclose(file);
}
