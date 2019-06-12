#ifndef BOARD_H_
#define BOARD_H_

#include "cell.h"

typedef struct s_Board
{
  Cell *cells;
} Board;

void board_init(Board *board);
void board_free(Board *board);

/* Returns cell according to its pos given by a letter and a number.
 * Uses same coord system as a chess board */
Cell *board_get_cell(Board *board, char x, uint8_t y);

/* Returns cell according to its pos given by two numbers x and y.
 * Uses cartesian-like coord system */
Cell *board_get_cell_coord(Board *board, uint8_t x, uint8_t y);

void board_print(Board *board);

#endif /* BOARD_H_ */
