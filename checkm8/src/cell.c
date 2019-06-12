#include "cell.h"
#include <stdio.h>

void cell_print(Cell *cell)
{
  printf("Cell: { Piece=%d | Color=%d | x=%d, | y=%d }\n",
    cell->piece, cell->color, cell->x, cell->y);
}
