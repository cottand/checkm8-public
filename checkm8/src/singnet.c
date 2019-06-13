#include <stdlib.h>
#include <stdio.h>
#include "singnet.h"
#include <string.h>

#define max_cmd_length 200
#define max_response_length 300
#define empty_string "\0"
#define game_id "checkm8"

void alpha_make_move(char *move)
{
  Alpha_Request *request = malloc(sizeof(Alpha_Request));
  init_alpha_request(request);

  request->move = move;

  FILE *res_file = snet_alpha_request(request); 

  char *res_buffer = calloc(sizeof(char), max_response_length);
  if(res_file == NULL)
  {
    perror("Error: response is NULL");
  }
  
  char buffer[30];
  
  char error_pattern[] = "status: \"move_error\"\n"; 
  while (fgets(buffer, 30, res_file))
  {
    printf("%s", buffer);
    if(strcmp(error_pattern, buffer) == 0)
    {
      printf("\nMove error!!");
    }
    else if(strncmp("status", buffer, 6) == 0)
    {
      //TODO: Extract the move here......
    }
  }
  
  fclose(res_file);
  free(res_buffer);
  free(request);
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
  char *alpha_out = "../resources/alpha_out.txt";

  char *call = "snet client call -y snet zeta36-chess-alpha-zero play";
  char *json = calloc(sizeof(char), max_cmd_length);

  snprintf(json, max_cmd_length,
   "{\"uid\": \"%s\", \"move\": \"%s\", \"cmd\": \"%s\"}",
   request->uid, request->move, request->cmd);
  printf("\n\n%s\n\n", json);
  char *cmd = calloc(sizeof(char), max_cmd_length);

  snprintf(cmd, max_cmd_length,
   "{ printf \"\n$(%s '%s')\n\"; } > %s",
   call, json, alpha_out);

  if(system(cmd) == -1)
  {
    perror("Error: failed to send request");
  }
  
  FILE *res_file = fopen(alpha_out, "r");
  
  free(cmd); 
  free(json);
  return res_file;
}

void init_alpha_request(Alpha_Request *request)
{
  request->uid = game_id;
  request->move = "\0";
  request->cmd = "\0";
}
