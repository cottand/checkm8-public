#include "vision.h"

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/videoio/videoio_c.h>

#define URL "http://146.169.218.9:8080"

#define PADDING 0.03

#define DELTA_LINE_ERROR 2
#define DELTA_LINE 3

#define THRESH_STD_DEV 16
#define THRESH_CIRCLES 25

static IplImage *get_image_from_camera(void)
{
  char python_request[60];
  snprintf(python_request, 60, "python python/read_stream.py %s", URL);
  system(python_request);
  IplImage *image = cvLoadImage("images/chessboard.jpg", CV_WINDOW_AUTOSIZE);
  return image;
}

static IplImage *get_image_from_path(char *path)
{
  IplImage *image = cvLoadImage(path, CV_WINDOW_AUTOSIZE);
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

void get_lines(CvSeq *lines, CvSeq *h_lines, CvSeq *v_lines)
{
  for (int i = 0; i < lines->total; i++)
  {
    CvPoint *line = (CvPoint *)cvGetSeqElem(lines, i);
    if (abs(line[1].y - line[0].y) < DELTA_LINE)
    {
      cvSeqPush(h_lines, line);
    }
    else if (abs(line[1].x - line[0].x) < DELTA_LINE)
    {
      cvSeqPush(v_lines, line);
    }
  }
}

static CvRect **get_cells(float *topLeft, float *bottomRight, IplImage *board)
{
  CvRect **cells = malloc(8 * sizeof(CvRect *));

  int width = bottomRight[0] - topLeft[0];
  int height = bottomRight[1] - topLeft[1];
  int cell_width = width / 8;
  int cell_height = height / 8;

  float padding_x = PADDING * cell_width;
  float padding_y = PADDING * cell_height;

  CvSeq *lines = NULL;
  CvMemStorage *storage = cvCreateMemStorage(0);

  for (int i = 0; i < 8; i++)
  {
    cells[i] = malloc(8 * sizeof(CvRect));
    for (int j = 0; j < 8; j++)
    {
      int pos_x = topLeft[0] + j * cell_width + padding_x;
      int pos_y = topLeft[1] + i * cell_height + padding_y;
      int wid = cell_width - 2 * padding_x;
      int hei = cell_height - 2 * padding_y;

      cells[i][j] = cvRect(pos_x, pos_y , wid, hei);

      bool lines_remain = true;
      while (lines_remain)
      {
        IplImage *image1 = cvCreateImage(cvGetSize(board), 8, 1);

        cvCanny(board, image1, 50, 200, 3);
        
        // Using Hough Lines Probabilistic Algorithm
        cvSetImageROI(image1, cells[i][j]);
        int min_length = wid < hei ? wid : hei;
        min_length -= 0.5 * min_length;
        lines = cvHoughLines2(image1, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 35, min_length, 40, 0, 360);
        
        CvSeq *h_lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint) * 2, storage);
        CvSeq *v_lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint) * 2, storage);
        get_lines(lines, h_lines, v_lines);
        /*
        for (int z = 0; z < lines->total; z++)
        {
          CvPoint *line = (CvPoint *)cvGetSeqElem(lines, z);
          line[0].x += cells[i][j].x;
          line[1].x += cells[i][j].x;
          line[0].y += cells[i][j].y;
          line[1].y += cells[i][j].y;
          cvLine(image1, line[0], line[1], cvScalar(255, 0, 0, 0), 3, 8, 0);
        }
        char name[11];
        snprintf(name, 11, "cell[%d][%d]", i, j);
        cvShowImage(name, image1);
        cvWaitKey(0);*/
        printf("Cell[%d][%d]: %d\n", i, j, lines->total);
      
        printf("Found %d h lines\n", h_lines->total);
        printf("Found %d v lines\n", v_lines->total);
        float padding_top = 0, padding_bot = 0;
        float center_y = hei / 2;
        for (int h = 0; h < h_lines->total; h++)
        {
          CvPoint *line = (CvPoint *)cvGetSeqElem(h_lines, h);
          if (line[0].y > center_y)
          {
            float new_padding_bot = hei - MIN(line[0].y, line[1].y);
            padding_bot = MAX(padding_bot, new_padding_bot) + DELTA_LINE_ERROR;
          }
          else
          {
            float new_padding_top = MAX(line[0].y, line[1].y);
            padding_top = MAX(padding_top, new_padding_top) + DELTA_LINE_ERROR;
          }
        }

        float padding_left = 0, padding_right = 0;
        float center_x = wid / 2;
        for (int v = 0; v < v_lines->total; v++)
        {
          CvPoint *line = (CvPoint *)cvGetSeqElem(v_lines, v);
          if (line[0].x > center_x)
          {
            float new_padding_right = wid - MIN(line[0].x, line[1].x);
            padding_right = MAX(padding_right, new_padding_right) + DELTA_LINE_ERROR;
          }
          else
          {
            float new_padding_left = MAX(line[0].x, line[1].x);
            padding_left = MAX(padding_left, new_padding_left) + DELTA_LINE_ERROR;
          }
        }

        pos_x += padding_left;
        pos_y += padding_top;
        wid -= padding_right + padding_left;
        hei -= padding_top + padding_bot;
        printf("Padding left : %f\n", padding_left);
        printf("Padding right : %f\n", padding_right);
        printf("Padding top : %f\n", padding_top);
        printf("Padding bot : %f\n", padding_bot);
        cells[i][j] = cvRect(pos_x, pos_y , wid, hei);

        lines_remain = !(padding_left < 1 && padding_right < 1 && padding_top < 1 && padding_bot < 1);
      }
    }
  }
  cvReleaseMemStorage(&storage);
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

bool is_cell_empty_std_dev(Vision_State *vision_state, int row, int column)
{
  float std_dev_cell = get_cell_std_dev(vision_state, row, column);

  return fabs(std_dev_cell - vision_state->std_dev_empty) <= THRESH_STD_DEV;
}

bool is_cell_empty_circles(Vision_State *vision_state, int row, int column)
{
  CvMemStorage *storage = cvCreateMemStorage(0);

  // This will store the lines
  CvSeq *circles = NULL;

  IplImage *image = get_cell(vision_state, row, column);
  circles = cvHoughCircles(image, storage, CV_HOUGH_GRADIENT, 1, 100, 25, THRESH_CIRCLES, 5, 20);

  // Code to draw circle
  //printf("total : %d\n", circles->total);
  //float *circle1 = (float *)cvGetSeqElem(circles, 0);
  //cvCircle(image, cvPoint(circle1[0], circle1[1]), circle1[2], cvScalar(255, 0, 0, 0), 1, 8, 0);

  return circles->total == 0;
}

bool is_cell_empty_substraction(Vision_State *vision_state, int row, int column)
{
  return false;
}

bool is_cell_empty(Vision_State *vision_state, int row, int column)
{
  return is_cell_empty_circles(vision_state, row, column) && is_cell_empty_std_dev(vision_state, row, column);
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
  printf("Found %d markers\n", markers->total);

  float *top_left;
  float *bottom_right;
  get_corners(markers, &top_left, &bottom_right);

  vision_state->board = board;

  CvRect **cells = get_cells(top_left, bottom_right, vision_state->board);

  vision_state->board_empty = board;
  vision_state->cells = cells;
  vision_state->std_dev_empty = get_empty_std_dev(vision_state);

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

Vision_Change get_vision_change(Vision *vision)
{
  Vision_Change vision_change;
  vision_change.emptied_cell[0] = -1;
  vision_change.emptied_cell[1] = -1;
  vision_change.filled_cell[0] = -1; //scanf("");
  vision_change.filled_cell[1] = -1;
  // TODO: Array of 2 elements to keep i and j !
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      IplImage *cell_prev = get_cell(vision->prev, i, j);
      IplImage *cell_curr = cvCreateImage(cvGetSize(cell_prev), 8, 1);
      IplImage *subtraction = cvCreateImage(cvGetSize(cell_prev), 8, 1);
      cvResize(get_cell(vision->curr, i, j), cell_curr, CV_INTER_LINEAR);
      cvSub(cell_prev, cell_curr, subtraction, 0);

      // TODO : test the subtraction -> mean???
      // If mostly white, a piece has been added
      // If mostly black, nothing changed ??
      CvScalar mean, std_dev;
      cvAvgSdv(subtraction, &mean, &std_dev, NULL);

      if (mean.val[0] >= 5)
      {
      }
    }
  }
  return vision_change;
}

void vision_init(Vision *vision)
{
  Vision_State *vision_state = malloc(sizeof(Vision_State *));
  IplImage *image = get_image_from_camera();
  vision_state_init(image, vision_state);
  vision->prev = vision_state;
  vision->curr = vision_state;
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

void vision_free(Vision *vision)
{
  vision_state_free(vision->prev);
  vision_state_free(vision->curr);
  free(vision);
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


  */
  /*
  Vision_State vision_state;
  vision_state_init(get_image_from_camera(), &vision_state);
  printf("Waiting..\n");
  getchar();
  vision_state.board = get_image_from_camera();
  */
   /* Loading the image */
  //IplImage *image = cvLoadImage("images/chessboard.jpg", CV_WINDOW_AUTOSIZE);

  Vision_State vision_state;
  vision_state_init(get_image_from_path("images/chessboard.jpg"), &vision_state);

  int count = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (is_cell_empty(&vision_state, i, j))
      {
        printf(".");
      }
      else
      {
        printf("O");
        count++;
      }
      //cvShowImage("Cell", get_cell(board, cells, i, j));
      //cvWaitKey(0);
    }
    printf("\n");
  }
  printf("%d\n\n", count);

/*
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      char name[11];   
      snprintf(name, 11, "cell[%d][%d]", i, j);
      cvShowImage(name, get_cell(&vision_state, i, j));
      cvWaitKey(0);
      cvDestroyAllWindows();
    }
  }*/

  /*int count = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      float dev = get_cell_std_dev(&vision_state, i, j);
      if (dev < 10)
      {
        printf(".");
      }
      else
      {
        printf("O");
        count++;
      }
      //cvShowImage("Cell", get_cell(board, cells, i, j));
      //cvWaitKey(0);
    }
    printf("\n");
  }
  printf("%d\n\n", count);*/
/*
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      cvShowImage("cell", get_cell(&vision_state, i, j));
      cvWaitKey(0);
      cvDestroyAllWindows();
    }
  }*/
/*
  // We'll use a temporary image with a single colour channel, to recognise the lines
  IplImage *image1 = cvCreateImage(cvGetSize(vision_state.board), 8, 1);
  // IplImage* image2 = cvCreateImage(cvGetSize(image), 8, 3);

  // Creating Memory Storage
  CvMemStorage *storage = cvCreateMemStorage(0);

  // This will store the lines
  CvSeq *lines = NULL;

  // Converting our image to greyscale
  cvCanny(vision_state.board, image1, 50, 200, 3);
  // cvCvtColor(image1, image2, CV_GRAY2BGR);

  // Using Hough Lines Probabilistic Algorithm
  cvSetImageROI(image1, vision_state.cells[0][0]);
  lines = cvHoughLines2(image1, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 40, 0, 360);

    printf("lines: %d\n", lines->total);
  for (int i = 0; i < lines->total; i++)
  {
    CvPoint *line = (CvPoint *)cvGetSeqElem(lines, i);
    line[0].x += vision_state.cells[0][0].x;
    line[1].x += vision_state.cells[0][0].x;
    line[0].y += vision_state.cells[0][0].y;
    line[1].y += vision_state.cells[0][0].y;
    printf("line found\n");
    cvLine(vision_state.board, line[0], line[1], cvScalar(0, 0, 0, 0), 3, 8, 0);
  }

  // Printing image
  cvNamedWindow("image", 0);
  //cvResetImageROI(vision_state.board);
  cvShowImage("Original", vision_state.board );
  //cvShowImage("Chessboard lines bruv", image2);
  cvWaitKey(0);
  cvReleaseImage(&vision_state.board);
  //cvReleaseImage(&image2);
  cvDestroyAllWindows();*/


  //Vision_State vision_state;
  //vision_state_init(get_image_from_path("images/chessboard.jpg"), &vision_state);

  /* printf("std_dev_empty : %f\n", vision_state.std_dev_empty);
  printf("cell 7 7 : %f\n", get_cell_std_dev(&vision_state, 0, 0));
  printf("Cell[6][6] empty : %d\n", is_cell_empty(&vision_state, 0, 0));*/
/*
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      float dev = get_cell_std_dev(&vision_state, i, j);
      printf("%0.1f ", dev);
    }
    printf("\n");
  }
  printf("\n\n");*/
/*
  int count = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      bool empty = is_cell_empty(&vision_state, i, j);
      float dev = get_cell_std_dev(&vision_state, i, j);
      if (dev < 10)
      {
        printf(".");
      }
      else
      {
        printf("O");
        count++;
      }
      //cvShowImage("Cell", get_cell(board, cells, i, j));
      //cvWaitKey(0);
    }
    printf("\n");
  }
  printf("%d\n\n", count);*/
/*
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      cvShowImage("cells[7][7]", get_cell(&vision_state, i, j));
      cvWaitKey(0);
      cvDestroyAllWindows();
    }
  }*/
  //cvShowImage("cells[7][7]", get_cell(&vision_state, 0, 0));

  //cvShowImage("histogram", imgHistogram);
  cvWaitKey(0);
  cvDestroyAllWindows();

  /*
  vision_state_free(state0);
  vision_state_free(state1);

  cvShowImage("camera", get_image_from_camera());
  cvWaitKey(0);
  //cvReleaseImage(&image);
  cvDestroyAllWindows();
  */
}
