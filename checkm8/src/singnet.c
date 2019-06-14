#include <stdlib.h>
#include <stdio.h>
#include "singnet.h"
#include <string.h>

#define MAX_CMD_LENGTH 200
#define MAX_RESPONSE_LENGTH 300
#define MAX_RES_LINE_LENGTH 30

#define GAME_ID "checkm8"
#define ALPHA_OUT "../resources/alpha_out.txt"
#define MOVE_OFFSET 23
#define ERROR_PATTERN "status: \"move_error\"\n"
#define MOVE_PATTERN "status: \"game_running: "


char *alpha_make_move(char *player_move, bool *success)
{
  *success = true;
  Alpha_Request *request = malloc(sizeof(Alpha_Request));
  init_alpha_request(request);

  request->move = player_move;

  FILE *res_file = snet_alpha_request(request); 

  char *res_buffer = calloc(sizeof(char), MAX_RESPONSE_LENGTH);
  if(res_file == NULL)
  {
    perror("Error: response is NULL");
  }

  char *res_move = calloc(sizeof(char), 4);

  char buffer[MAX_RES_LINE_LENGTH];

  while (fgets(buffer, MAX_RES_LINE_LENGTH, res_file))
  {
    if(strcmp(ERROR_PATTERN, buffer) == 0)
    {
      printf("Error: Alpha move error\n");
      free(res_move);
      *success = false;
    }
    else if(strncmp(MOVE_PATTERN  , buffer, MOVE_OFFSET) == 0)
    {
      strncpy(res_move, buffer + MOVE_OFFSET, 4);
    }
  }

  fclose(res_file);
  free(res_buffer);
  free(request);

  return res_move;
}

void alpha_reset(void)
{
  printf("Resetting board...\n");

  Alpha_Request *request = malloc(sizeof(Alpha_Request));
  init_alpha_request(request);

  request->cmd = "restart";
  FILE *res_file = snet_alpha_request(request);

  if(res_file == NULL)
  {
    perror("Error: response is NULL");
  }

  free(request);
}

FILE *snet_alpha_request(Alpha_Request *request)
{
  char *call = "snet client call -y snet zeta36-chess-alpha-zero play";
  char *json = calloc(sizeof(char), MAX_CMD_LENGTH);

  snprintf(json, MAX_CMD_LENGTH,
   "{\"uid\": \"%s\", \"move\": \"%s\", \"cmd\": \"%s\"}",
   request->uid, request->move, request->cmd);
  char *cmd = calloc(sizeof(char), MAX_CMD_LENGTH);

  snprintf(cmd, MAX_CMD_LENGTH,
   "{ printf \"\n$(%s '%s')\n\"; } > %s",
   call, json, ALPHA_OUT);

  if(system(cmd) == -1)
  {
    perror("Error: failed to send request");
  }

  FILE *res_file = fopen(ALPHA_OUT, "r");

  free(cmd);
  free(json);
  return res_file;
}

void init_alpha_request(Alpha_Request *request)
{
  request->uid = GAME_ID;
  request->move = "\0";
  request->cmd = "\0";
}
