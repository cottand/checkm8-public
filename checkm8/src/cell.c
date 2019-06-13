#include "cell.h"
#include <stdio.h>

void cell_print(Cell *cell)
{
  printf("Cell: { Piece=%d (%d) | Color=%d | x=%d, | y=%d }\n",
    cell->piece.type, cell->piece.color, cell->color, cell->x, cell->y);
}
