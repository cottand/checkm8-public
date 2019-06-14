#include "checkm8.h"
#include "vision.h"
#include "board.h"
#include "singnet.h"
#include <stdio.h>
#include <stdlib.h>

void player_turn();
void algo_turn(Board *board, Vision *vision);

void checkm8_run(char *ip_cam)
{
  printf("Starting game..\n");
  printf("Initializing vision..\n");
  Vision vision;
  vision_init(&vision, ip_cam);

  printf("Initializing AlphaZero..\n");
  alpha_reset();

  printf("All good.\n");

  /* ALWAYS USE THIS BOARD AS IT IS THE ONLY ONE THAT CONTAINS THE TRUE PIECES
   */
  Board board;
  board_init(&board);

  while (1)
  {
    player_turn();
    algo_turn(&board, &vision);
  }
}

void player_turn()
{
  printf("* Player's turn now *\n");
  printf("Press enter once done..\n");
  getchar();
}

void algo_turn(Board *board, Vision *vision)
{
  printf("* Algo's turn now *\n");
  vision_update(vision);

  Board next;
  board_init(&next);
  board_set_from_vision(&next, vision);

  Move move = get_move(board, &next);
  move_print(&move);

  /* Carry out the move on the board */
  do_move(board, &move);

  char *move_str = NULL;
  move_to_str(&move, &move_str);
  printf("Detected move: %s\n", move_str);

  printf("Internal state of the board:\n");
  board_print(board);

  //char *algo_move = "t0d0";
  char *algo_move = alpha_make_move(move_str);

  printf("AlphaZero move: %s\n", algo_move);

  // get game board state DONE
  // put the state into new Board struct DONE
  // determine the move with the board passed in arg (careful with order) DONE
  // send move_str to SingNET ? DONE TODO CHECK IF IT'S GOOD ?
  // get move from AlphaZero and ask hooman to make it ? DONE TODO CHECK IF IT'S GOOD ?

  free(move_str);
  free(algo_move);
}
