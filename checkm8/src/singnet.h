#ifndef SINGNET_H__
#define SINGNET_H__

typedef struct s_Alpha_Request
{
  char *uid;
  char *move;
  char *cmd;
} Alpha_Request;

char *alpha_make_move(char *move);
void alpha_reset(void);
FILE *snet_alpha_request(Alpha_Request *request);
void init_alpha_request(Alpha_Request *request);

#endif /* SINGNET_H__ */