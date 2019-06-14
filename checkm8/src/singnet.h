#ifndef SINGNET_H_
#define SINGNET_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct s_Alpha_Request
{
  char *uid;
  char *move;
  char *cmd;
} Alpha_Request;

bool alpha_make_move(char *player_move, char **alpha_move);
void alpha_reset(void);
FILE *snet_alpha_request(Alpha_Request *request);
void init_alpha_request(Alpha_Request *request);

#endif /* SINGNET_H_ */
