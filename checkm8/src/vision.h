#ifndef VISION_H_
#define VISION_H_

#include <opencv2/core/core_c.h>

typedef struct s_Vision_State
{
  IplImage *board;
  CvRect **cells;

  IplImage *board_empty;
  float std_dev_empty;
} Vision_State;

typedef struct s_Vision
{
  Vision_State *prev;
  Vision_State *curr;
} Vision;

typedef struct s_Vision_Change
{
  int emptied_cell[2];
  int filled_cell[2];
} Vision_Change;

void vision_init(Vision *vision);
void vision_update(Vision *vision);
void vision_free(Vision *vision);

#endif /* VISION_H_ */
