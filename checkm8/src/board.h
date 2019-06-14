#ifndef BOARD_H_
#define BOARD_H_

#include "cell.h"
#include "vision.h"

typedef struct s_Move
{
  Cell from;
  Cell to;
  Piece piece;
} Move;

typedef struct s_Board
{
  Cell *cells;
} Board;

void board_init(Board *board);
void board_free(Board *board);

void board_set_from_vision(Board *board, Vision *vision);

/* Returns cell according to its pos given by a letter and a number.
 * Uses same coord system as a chess board */
Cell *board_get_cell(Board *board, char x, uint8_t y);

/* Returns cell according to its pos given by two numbers x and y.
 * Uses cartesian-like coord system */
Cell *board_get_cell_coord(Board *board, uint8_t x, uint8_t y);

/* Sets a piece in the cell. Uses cartesian-like coord system */
void board_set_cell_coord(Board *board, uint8_t, uint8_t y, Piece_Type type, Color color);

int get_move(Board *prev, Board *curr, Move *move);
Move move_piece(Cell *from, Cell *to);
Move move_piece_str(Board *board, char *str);

void do_move(Board *board, Move *move);

void board_print(Board *board);
void move_print(Move *move);

void move_to_str(Move *move, char **str);
void str_to_move(Board *board, Move *move, char *str);

#endif /* BOARD_H_ */
