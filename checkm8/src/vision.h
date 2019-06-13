#ifndef VISION_H_
#define VISION_H_

#include <opencv2/core/core_c.h>

typedef struct s_Vision_State
{
  IplImage *board;
  CvRect **cells;
} Vision_State;

typedef struct s_Vision
{
  Vision_State *prev;
  Vision_State *curr;
} Vision;

void vision_init(Vision *vision);
void vision_update(Vision *vision);

#endif /* VISION_H_ */
