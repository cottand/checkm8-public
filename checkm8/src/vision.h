#ifndef VISION_H_
#define VISION_H_

#include <opencv2/core/core_c.h>

typedef struct s_Vision
{
  float std_dev_empty;
  IplImage *board;
  CvRect **cells;
  char *camera_url;
} Vision;

typedef struct s_Vision_Change
{
  int emptied_cell[2];
  int filled_cell[2];
} Vision_Change;

bool is_cell_empty(Vision *vision, int row, int column);

void vision_init(Vision *vision, char *url);
void vision_update(Vision *vision);
void vision_free(Vision *vision);

#endif /* VISION_H_ */
