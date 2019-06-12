#ifndef CELL_H_
#define CELL_H_

#include <stdint.h>

typedef enum e_Color
{
  White,
  Black
} Color;

typedef enum e_Piece
{
  None,
  Pawn,
  Rook,
  Knight,
  Bishop,
  Queen,
  King
} Piece;

typedef struct s_Cell
{
  Color color;
  Piece piece;

  uint8_t x;
  uint8_t y;
} Cell;

void cell_print(Cell *cell);

#endif /* CELL_H_ */
