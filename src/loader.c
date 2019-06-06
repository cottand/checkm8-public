#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void load(char *path, void *ptr)
{
  unsigned long fileLen;

  FILE *file = fopen(path, "rb");
  if (!file)
  {
    fprintf(stderr, "Error: Invalid file path\n");
    exit(0);
  }

  /* Get file length */
  fseek(file, 0, SEEK_END);
  fileLen = ftell(file);
  fseek(file, 0, SEEK_SET);

  if(!fread(ptr, fileLen, 1, file))
  {
    fprintf(stderr, "Error: Issue reading file into memory");
  }
  

  fclose(file);
}

uint32_t get_filesize(char *path)
{
  uint32_t to_ret;
  FILE *file = fopen(path, "rb");
  if (!file)
  {
    fprintf(stderr, "Error: Invalid file path\n");
    exit(0);
  }

  /* Get file length */
  fseek(file, 0, SEEK_END);
  to_ret = ftell(file);

  fclose(file);
  return to_ret;
}
