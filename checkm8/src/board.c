#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static void initialize_pieces(Board *board, uint8_t row);
static void initialize_pawns(Board *board, uint8_t row);

void board_init(Board *board)
{
  uint8_t cell_count = 8 * 8;
  board->cells = malloc(sizeof(Cell*) * cell_count);

  /* Initialize colors */
  for (int i = 0; i < cell_count; i++)
  {
    board->cells[i].piece = None;

    board->cells[i].x = i % 8;
    board->cells[i].y = i / 8;

    int color = pow(-1, board->cells[i].x + board->cells[i].y);
    board->cells[i].color = (color == 1);
  }

  /* Initialize pieces */
  initialize_pieces(board, 1);
  initialize_pieces(board, 8);
  initialize_pawns(board, 2);
  initialize_pawns(board, 7);
}

void initialize_pieces(Board *board, uint8_t row)
{
  board_get_cell(board, 'a', row)->piece = Rook;
  board_get_cell(board, 'b', row)->piece = Knight;
  board_get_cell(board, 'c', row)->piece = Bishop;
  board_get_cell(board, 'd', row)->piece = Queen;
  board_get_cell(board, 'e', row)->piece = King;
  board_get_cell(board, 'f', row)->piece = Bishop;
  board_get_cell(board, 'g', row)->piece = Knight;
  board_get_cell(board, 'h', row)->piece = Rook;
}

void initialize_pawns(Board *board, uint8_t row)
{
  char x;
  for (x = 'a'; x < 'i'; x++)
  {
    board_get_cell(board, x, row)->piece = Pawn;
  }
}

void board_free(Board *board)
{
  free(board->cells);
}

Cell *board_get_cell(Board *board, char x, uint8_t y)
{
  uint8_t pos_x = x - 'a';
  uint8_t pos_y = y - 1;

  return board_get_cell_coord(board, pos_x, pos_y);
}

Cell *board_get_cell_coord(Board *board, uint8_t x, uint8_t y)
{
  if (x > 7 || y > 7)
  {
    perror("Error: of bounds coordinates for board");
    return 0;
  }

  return &board->cells[8 * y + x];
}

void board_print(Board *board)
{
  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 8; x++)
    {
      cell_print(board_get_cell_coord(board, x, y));
    }
  }
}
