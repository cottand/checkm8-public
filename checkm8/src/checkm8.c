#include "checkm8.h"
#include "vision.h"
#include "board.h"
#include "singnet.h"
#include <stdio.h>
#include <stdlib.h>

static void player_turn();
static void algo_turn(Board *board, Vision *vision);
static char *detect_player_move(Board *board, Vision *vision);

void checkm8_run(char *ip_cam)
{
  printf("Starting game..\n");
  printf("Initializing vision..\n");
  Vision vision;
  bool success = vision_init(&vision, ip_cam);

  if (!success)
  {
    printf("Error in initializing vision. Abroting.\n");
    return;
  }

  printf("Initializing AlphaZero..\n");
  alpha_reset();

  printf("All good.\n");

  Board board;
  board_init(&board);

  while (1)
  {
    player_turn();
    algo_turn(&board, &vision);

    printf("Internal state of the board:\n");
    board_print(&board);
  }

  board_free(&board);
  vision_free(&vision);
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

  char *move = detect_player_move(board, vision);

  char *algo_move = alpha_make_move(move);
  move_piece_str(board, algo_move);

  printf("AlphaZero move: %s\n", algo_move);

  free(move);
  free(algo_move);
}

char *detect_player_move(Board *board, Vision *vision)
{
  Move move;
  char *move_str = NULL;

  bool successful = false;
  while (!successful)
  {
    successful = true;

    bool vis_success = vision_update(vision);

    Board next;
    board_init(&next);
    board_set_from_vision(&next, vision);

    int nb_moves = get_move(board, &next, &move);

    move_to_str(&move, &move_str);
    printf("Detected move: %s\n", move_str);

    if (nb_moves != 1 || !vis_success)
    {
      free(move_str);

      char *answer = calloc(1, sizeof(char) * 5);
      printf("There was an error in detecting the player's move. Press enter to retry or input the move directly: ");
      fgets(answer, 4, stdin);

      /* The user has inputed an anser eg c2c4 */
      if (strcmp(answer, ""))
      {
        successful = true;
        move_str = answer;
        str_to_move(board, &move, answer);

        printf("Registering move as: ");
        move_print(&move);
      }
      /* We try again */
      else
      {
        successful = false;
        free(answer);
      }
    }

    board_free(&next);
  }

  /* Carry out the move on the board */
  do_move(board, &move);

  return move_str;
}
