#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static void initialize_pieces(Board *board, uint8_t row, Color piece_color);
static void initialize_pawns(Board *board, uint8_t row, Color piece_color);

void board_init(Board *board)
{
  uint8_t cell_count = 8 * 8;
  board->cells = malloc(sizeof(Cell*) * cell_count);

  /* Initialize colors */
  for (int i = 0; i < cell_count; i++)
  {
    board->cells[i].piece = (Piece) { .type = None, .color = White };

    board->cells[i].x = i % 8;
    board->cells[i].y = i / 8;

    int color = pow(-1, board->cells[i].x + board->cells[i].y);
    board->cells[i].color = (color == 1);
  }

  /* Initialize pieces */
  initialize_pieces(board, 1, White);
  initialize_pieces(board, 8, Black);
  initialize_pawns(board, 2, White);
  initialize_pawns(board, 7, Black);
}

void initialize_pieces(Board *board, uint8_t row, Color piece_color)
{
  /* Boring stuff to init a row of pieces */
  board_get_cell(board, 'a', row)->piece
    = (Piece) { .type = Rook, .color = piece_color };
  board_get_cell(board, 'b', row)->piece
    = (Piece) { .type = Knight, .color = piece_color };
  board_get_cell(board, 'c', row)->piece
    = (Piece) { .type = Bishop, .color = piece_color };
  board_get_cell(board, 'd', row)->piece
    = (Piece) { .type = Queen, .color = piece_color };
  board_get_cell(board, 'e', row)->piece
    = (Piece) { .type = King, .color = piece_color };
  board_get_cell(board, 'f', row)->piece
    = (Piece) { .type = Bishop, .color = piece_color };
  board_get_cell(board, 'g', row)->piece
    = (Piece) { .type = Knight, .color = piece_color };
  board_get_cell(board, 'h', row)->piece
    = (Piece) { .type = Rook, .color = piece_color };
}

void initialize_pawns(Board *board, uint8_t row, Color piece_color)
{
  char x;
  for (x = 'a'; x < 'i'; x++)
  {
    board_get_cell(board, x, row)->piece
      = (Piece) { .type = Pawn, .color = piece_color};
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
    perror("Error: of bounds coordinates for board\n");
    return 0;
  }

  return &board->cells[8 * y + x];
}

Move get_move(Board *prev, Board *curr)
{
  Move move;
  board_print(prev);
  board_print(curr);

  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 8; x++)
    {
      Cell *prev_cell = board_get_cell_coord(prev, x, y);
      Cell *curr_cell = board_get_cell_coord(curr, x, y);

      if (prev_cell->piece.type != None && curr_cell->piece.type == None)
      {
        move.from = *curr_cell;
      }

      if (prev_cell->piece.type == None && curr_cell->piece.type != None)
      {
        move.to = *curr_cell;
        move.piece = curr_cell->piece;
      }
    }
  }

  return move;
}

Move move_piece(Cell *from, Cell *to)
{
  Move move;

  move.piece = from->piece;
  move.from = *from;
  move.to = *to;

  to->piece = from->piece;
  from->piece.type = None;

  return move;
}

void do_move(Board *board, Move *move)
{
  Cell *from = board_get_cell_coord(board, move->from.x, move->from.y);
  Cell *to = board_get_cell_coord(board, move->from.y, move->from.y);

  from->piece.type = None;
  to->piece = move->piece;
}

void board_print(Board *board)
{
  printf("  a\tb\tc\td\te\tf\tg\th\n");

  for (int y = 7; y > -1; y--)
  {
    printf("%d ", y + 1);
    for (int x = 0; x < 8; x++)
    {
      Cell *cell = board_get_cell_coord(board, x, y);

      if (cell->piece.type == None)
      {
        printf("-\t");
      }
      else
      {
        printf("%d", cell->piece.type);
        if (cell->piece.color == 0)
        {
          printf("w\t");
        }
        else
        {
          printf("b\t");
        }
      }
    }
    printf("\n");
  }
}

void move_print(Move *move)
{
  printf("Move: { from (%d, %d) | to (%d, %d), piece (%d, %d) }\n",
    move->from.x, move->from.y, move->to.x, move->to.y,
    move->piece.type, move->piece.color);
}
