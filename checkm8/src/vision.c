#include "vision.h"

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/videoio/videoio_c.h>

#define URL "http://146.169.211.105:8080"

static IplImage *get_image_from_camera(void)
{
  char python_request[60];
  snprintf(python_request, 60, "python python/read_stream.py %s", URL);
  system(python_request);
  IplImage *image = cvLoadImage("images/chessboard.jpg", CV_WINDOW_AUTOSIZE);
  return image;
}

static IplImage *get_thresholded_image(IplImage *img)
{
  IplImage *imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
  cvCvtColor(img, imgHSV, CV_BGR2HSV);

  IplImage *thresholded_img = cvCreateImage(cvGetSize(img), 8, 1);

  cvInRangeS(imgHSV, cvScalar(77, 100, 100, 0), cvScalar(87, 255, 255, 0), thresholded_img);

  cvReleaseImage(&imgHSV);
  return thresholded_img;
}

static void get_corners(CvSeq *elems, float **topLeft, float **bottomRight)
{
  float *point1 = (float *)cvGetSeqElem(elems, 0);
  float *point2 = (float *)cvGetSeqElem(elems, 1);
  if (point1[0] < point2[0])
  {
    point1[0] += point1[2];
    point1[1] += point1[2];
    point2[0] -= point2[2];
    point2[1] -= point2[2];

    *topLeft = point1;
    *bottomRight = point2;
  }
  else
  {
    point2[0] += point2[2];
    point2[1] += point2[2];
    point1[0] -= point1[2];
    point1[1] -= point1[2];

    *topLeft = point2;
    *bottomRight = point1;
  }
}

static CvRect **get_cells(float *topLeft, float *bottomRight)
{
  CvRect **cells = malloc(8 * sizeof(CvRect *));

  int width = bottomRight[0] - topLeft[0];
  int height = bottomRight[1] - topLeft[1];
  int cell_width = width / 8;
  int cell_height = height / 8;

  float padding_x = 0.03 * width;
  float padding_y = 0.03 * height;

  for (int i = 0; i < 8; i++)
  {
    cells[i] = malloc(8 * sizeof(CvRect));
    for (int j = 0; j < 8; j++)
    {
      cells[i][j] = cvRect(topLeft[0] + j * cell_width + padding_x, topLeft[1] + i * cell_height + padding_y, cell_width - 2 * padding_x, cell_height - 2 * padding_y);
    }
  }
  return cells;
}

static IplImage *get_cell(Vision_State *vision_state, int row, int column)
{
  cvResetImageROI(vision_state->board);
  cvSetImageROI(vision_state->board, vision_state->cells[row][column]);
  return vision_state->board;
}

float get_empty_std_dev(Vision_State *vision_state)
{
  float dev = 0.0f;

  CvScalar mean, std_dev;
  cvAvgSdv(get_cell(vision_state, 2, 0), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision_state, 2, 1), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision_state, 5, 0), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision_state, 5, 1), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision_state, 2, 6), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision_state, 2, 7), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision_state, 5, 6), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision_state, 5, 7), &mean, &std_dev, NULL);
  dev += std_dev.val[0];

  dev /= 8.0f;

  return dev;
}

float get_cell_std_dev(Vision_State *vision_state, int row, int col)
{
  CvScalar mean, std_dev;
  cvAvgSdv(get_cell(vision_state, row, col), &mean, &std_dev, NULL);

  return std_dev.val[0];
}

bool is_cell_empty(Vision_State *vision_state, int row, int column, float std_dev_empty)
{
  float std_dev_cell = get_cell_std_dev(vision_state, row, column);

  return fabs(std_dev_cell - std_dev_empty) <= 5;
  /*
  CvMemStorage *storage = cvCreateMemStorage(0);

  // This will store the lines
  CvSeq *circles = NULL;

  IplImage *image = get_cell(board, cells, row, column);
  circles = cvHoughCircles(image, storage, CV_HOUGH_GRADIENT, 1, 100, 25, 27, 5, 20);

  //printf("total : %d\n", circles->total);
  //float *circle1 = (float *)cvGetSeqElem(circles, 0);
  //cvCircle(image, cvPoint(circle1[0], circle1[1]), circle1[2], cvScalar(255, 0, 0, 0), 1, 8, 0);
  return circles->total == 0;*/
}

static void vision_state_init(IplImage *file, Vision_State *vision_state)
{
  // Create images
  IplImage *thresholded = get_thresholded_image(file);
  IplImage *board = cvCreateImage(cvGetSize(file), 8, 1);
  cvCvtColor(file, board, CV_BGR2GRAY);

  // Allocate temporary memory storage
  CvMemStorage *tmp_storage = cvCreateMemStorage(0);

  CvSeq *markers = NULL;
  markers = cvHoughCircles(thresholded, tmp_storage, CV_HOUGH_GRADIENT, 1, 50, 10, 10, 0, 500);

  float *top_left;
  float *bottom_right;
  get_corners(markers, &top_left, &bottom_right);

  CvRect **cells = get_cells(top_left, bottom_right);

  vision_state->board = board;
  vision_state->cells = cells;

  // Clear Memory
  cvClearSeq(markers);
  cvReleaseMemStorage(&tmp_storage);
  cvReleaseImage(&thresholded);
  cvReleaseImage(&file);
}

static void vision_state_free(Vision_State *vision_state)
{
  cvReleaseImage(&vision_state->board);
  for (int i = 0; i < 8; i++)
  {
    free(vision_state->cells[i]);
  }
  free(vision_state->cells);
  free(vision_state);
}

void vision_init(Vision *vision)
{

}

void vision_update(Vision *vision)
{
  vision_state_free(vision->prev);
  vision->prev = vision->curr;
  Vision_State *curr = malloc(sizeof(Vision_State *));
  IplImage *image = get_image_from_camera();
  vision_state_init(image, curr);
  vision->curr = curr;
}

int main(int argc, char **argv)
{
  /*
  Vision_State *state0 = malloc(sizeof(Vision_State));
  Vision_State *state1 = malloc(sizeof(Vision_State));

  vision_state_init("images/chessboard.jpg", state0);
  vision_state_init("images/chessboard1.jpg", state1);

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      IplImage *cell0 = get_cell(state0, i, j);
      IplImage *cell1 = cvCreateImage(cvGetSize(cell0), 8, 1);
      IplImage *result = cvCreateImage(cvGetSize(cell0), 8, 1);
      cvResize(get_cell(state1, i, j), cell1, CV_INTER_LINEAR);
      cvSub(cell0, cell1, result, 0);
      cvShowImage("Result", result);
      cvWaitKey(0);
      cvDestroyAllWindows();
    }
  }

  
  // Initialise Histogram
  int bins = 256;
  int hsize[] = {bins};
  float xranges[] = {0, 256};
  float *ranges[] = {xranges};
  CvHistogram *hist;
  IplImage *imgHistogram;

  // Create Histogram
  hist = cvCreateHist(1, hsize, CV_HIST_ARRAY, ranges, 1);
  
  char name[11];
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8 ; j++)
    {
      IplImage *image = get_cell(board, cells, i, j);
      snprintf(name, 11, "cell[%d][%d]", i, j);
      cvShowImage(name, image);

      cvCalcHist(&image, hist, 0, NULL);
      float max_value = 0, min_value = 0;
      cvGetMinMaxHistValue(hist, &min_value, &max_value, 0, 0);
      //printf("min : %f, max : %f\n", min_value, max_value);

      // Create image of Histogram
      imgHistogram = cvCreateImage(cvSize(bins, 50), 8, 1);
      cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(256,50), cvScalar(255,255,255, 0), -1, 8, 0);

      // Draw Histogram
      float value;
      int normalized;
      for(int i=0; i < bins; i++){
        value = cvGetReal1D(hist->bins, i);
        normalized = cvRound(value*50/max_value);
        cvLine(imgHistogram,cvPoint(i,50), cvPoint(i,50-normalized), cvScalar(0,0,0,0), 1, 8, 0);
      }

      cvShowImage("histogram", imgHistogram);
      cvWaitKey(0);
      cvDestroyAllWindows();
    }
  }
  


  float std_dev_empty = get_empty_std_dev(board, cells);
  printf("std_dev_empty : %f\n", std_dev_empty);
  printf("cell 7 7 : %f\n", get_cell_std_dev(board, cells, 4, 1));
  printf("Cell[6][6] empty : %d\n", is_cell_empty(board, cells, 4, 1, std_dev_empty));

  int count = 0;
  for(int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      bool empty = is_cell_empty(board, cells, i, j, std_dev_empty);
      if(empty) {printf(".");}
      else {printf("O"); count++;}
      //cvShowImage("Cell", get_cell(board, cells, i, j));
      //cvWaitKey(0);
    }
    printf("\n");
  }
  printf("%d\n", count);

  //cvShowImage("cells[7][7]", get_cell(board, cells, 4, 1));

  //cvShowImage("histogram", imgHistogram);
  cvWaitKey(0);
  cvDestroyAllWindows();

  vision_state_free(state0);
  vision_state_free(state1);*/

  cvShowImage("camera", get_image_from_camera());
  cvWaitKey(0);
  //cvReleaseImage(&image);
  cvDestroyAllWindows();
}
