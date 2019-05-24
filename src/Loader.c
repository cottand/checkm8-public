#include <stdio.h>
#include <stdint.h>

void load(char path[], void *ptr)
{
  unsigned long fileLen;

  FILE *file = fopen(path,"rb");

  if(!file)
  {
    fprintf(stderr, "Error: Unable to read file");
  }

  /* Get file length */
  fseek(file, 0, SEEK_END);
  fileLen = ftell(file);
  fseek(file, 0, SEEK_SET);

  fread(ptr, fileLen, 1, file);

  fclose(file);
}
