#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "checkm8.h"

int main()
{
  Board b1;
  board_init(&b1);

  Board b2;
  board_init(&b2);

  board_print(&b1);

  Cell *from = board_get_cell(&b1, 'a', 7);
  Cell *to   = board_get_cell(&b1, 'a', 6);

  Move m = move_piece(from, to);
  move_print(&m);

  //Move m2 = get_move(&b2, &b1);
  //move_print(&m2);

  return 0;
}
