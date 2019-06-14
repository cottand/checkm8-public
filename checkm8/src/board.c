#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static void initialize_pieces(Board *board, uint8_t row, Color piece_color);
static void initialize_pawns(Board *board, uint8_t row, Color piece_color);

void board_init(Board *board)
{
  uint8_t cell_count = 8 * 8;
  board->cells = malloc(sizeof(Cell) * cell_count);

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

void board_set_from_vision(Board *board, Vision *vision)
{
  /* The origin of vision is upper left, board uses bot left */
  int count = 0;
  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 8; x++)
    {
      if (!is_cell_empty(vision, 7 - y, x))
      {
        ++count;
        board_set_cell_coord(board, x, y, Pawn, White);
      }
      else
      {
        board_set_cell_coord(board, x, y, None, White);
      }
    }
  }
  printf("Detected %d pieces\n", count);
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

void board_set_cell_coord(Board *board, uint8_t x, uint8_t y, Piece_Type type, Color color)
{
  if (x > 7 || y > 7)
  {
    perror("Error: of bounds coordinates for board\n");
    return;
  }

  board->cells[8 * y + x].piece.type  = type;
  board->cells[8 * y + x].piece.color = color;
}

int get_move(Board *prev, Board *curr, Move *move)
{
  /* How many moves we detect, useful for error handling */
  int nb_missing = 0;
  int nb_new = 0;

  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 8; x++)
    {
      Cell *prev_cell = board_get_cell_coord(prev, x, y);
      Cell *curr_cell = board_get_cell_coord(curr, x, y);

      if (prev_cell->piece.type != None && curr_cell->piece.type == None)
      {
        move->from = *curr_cell;
        move->piece = prev_cell->piece;

        nb_missing++;
      }

      if (prev_cell->piece.type == None && curr_cell->piece.type != None)
      {
        move->to = *curr_cell;

        nb_new++;
      }
    }
  }

  /* If we've lost more than one piece */
  if (abs(nb_missing - nb_new) > 1)
  {
    printf("Error in detecting move, board is incoherent\n");
  }

  return MAX(nb_missing, nb_new);
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

Move move_piece_str(Board *board, char *str)
{
  int from_x = str[0] - 'a';
  int from_y = str[1] - '1';
  int to_x   = str[2] - 'a';
  int to_y   = str[3] - '1';

  Cell *from = board_get_cell_coord(board, from_x, from_y);
  Cell *to = board_get_cell_coord(board, to_x, to_y);

  return move_piece(from, to);
}

void do_move(Board *board, Move *move)
{
  Cell *from = board_get_cell_coord(board, move->from.x, move->from.y);
  Cell *to = board_get_cell_coord(board, move->to.x, move->to.y);

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

void move_to_str(Move *move, char **str)
{
  *str = malloc(sizeof(char) * 5);

  char from_x = move->from.x + 'a';
  int from_y = move->from.y + 1;

  char to_x = move->to.x + 'a';
  int to_y = move->to.y + 1;

  snprintf(*str, 5, "%c%d%c%d", from_x, from_y, to_x, to_y);
}

void str_to_move(Board *board, Move *move, char *str)
{
  int from_x = str[0] - 'a';
  int from_y = str[1] - '1';
  int to_x   = str[2] - 'a';
  int to_y   = str[3] - '1';

  move->from  = *board_get_cell_coord(board, from_x, from_y);
  move->to    = *board_get_cell_coord(board, to_x, to_y);
  move->piece = move->from.piece;
}
