#include "assembler.h"
#include "loader.h"
#include "writer.h"
#include "parser.h"
#include <stdlib.h>

void assemble(char *src_path, char *dst_path)
{
  uint32_t src_size = get_filesize(src_path);
  void *src = malloc(src_size + 1);
  load(src_path, src);

  void *output = 0;
  size_t output_size = 0;
  parse(src, &output, &output_size);

  write(dst_path, output, &output_size);

  free(src);
  free(output);
}
