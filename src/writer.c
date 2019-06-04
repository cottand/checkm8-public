#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void write(char path[], void *ptr, size_t *size)
{
  FILE *file = fopen(path, "wb");
  if (!file)
  {
    fprintf(stderr, "Error: Invalid file path\n");
    exit(0);
  }

  printf("size: %ld\n", *size);

  fwrite(ptr, *size, 1, file);

  fclose(file);
}
