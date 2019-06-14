#include "vision.h"

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/videoio/videoio_c.h>

// TODO: pass URL as argument of main function
#define URL "http://146.169.218.9:8080"

#define PADDING 0.03

#define DELTA_LINE_ERROR 2
#define DELTA_LINE 3

#define THRESH_STD_DEV 16
#define THRESH_CIRCLES 25

#define CAMERA 0

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

static IplImage *get_image()
{
  if (CAMERA)
  {
    return get_image_from_camera();
  }
  else
  {
    return get_image_from_path("images/chessboard.jpg");
  }
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

static void get_lines(CvSeq *lines, CvSeq *h_lines, CvSeq *v_lines)
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

      cells[i][j] = cvRect(pos_x, pos_y, wid, hei);

      bool lines_remain = true;
      while (lines_remain)
      {
        IplImage *image1 = cvCreateImage(cvGetSize(board), 8, 1);

        cvCanny(board, image1, 50, 200, 3);

        cvSetImageROI(image1, cells[i][j]);
        int min_length = wid < hei ? wid : hei;
        min_length -= 0.5 * min_length;
        lines = cvHoughLines2(image1, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 35, min_length, 40, 0, 360);

        CvSeq *h_lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint) * 2, storage);
        CvSeq *v_lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint) * 2, storage);
        get_lines(lines, h_lines, v_lines);

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
        cells[i][j] = cvRect(pos_x, pos_y, wid, hei);

        lines_remain = !(padding_left < 1 && padding_right < 1 && padding_top < 1 && padding_bot < 1);
      }
    }
  }
  cvReleaseMemStorage(&storage);
  return cells;
}

static IplImage *get_cell(Vision *vision, int row, int column)
{
  cvResetImageROI(vision->board);
  cvSetImageROI(vision->board, vision->cells[row][column]);
  return vision->board;
}

static float get_empty_std_dev(Vision *vision)
{
  float dev = 0.0f;

  CvScalar mean, std_dev;
  cvAvgSdv(get_cell(vision, 2, 0), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision, 2, 1), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision, 5, 0), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision, 5, 1), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision, 2, 6), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision, 2, 7), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision, 5, 6), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(vision, 5, 7), &mean, &std_dev, NULL);
  dev += std_dev.val[0];

  dev /= 8.0f;

  return dev;
}

static float get_cell_std_dev(Vision *vision, int row, int col)
{
  CvScalar mean, std_dev;
  cvAvgSdv(get_cell(vision, row, col), &mean, &std_dev, NULL);

  return std_dev.val[0];
}

static bool is_cell_empty_std_dev(Vision *vision, int row, int column)
{
  float std_dev_cell = get_cell_std_dev(vision, row, column);

  return fabs(std_dev_cell - vision->std_dev_empty) <= THRESH_STD_DEV;
}

static bool is_cell_empty_circles(Vision *vision, int row, int column)
{
  CvMemStorage *storage = cvCreateMemStorage(0);

  CvSeq *circles = NULL;

  IplImage *image = get_cell(vision, row, column);
  circles = cvHoughCircles(image, storage, CV_HOUGH_GRADIENT, 1, 100, 25, THRESH_CIRCLES, 5, 20);

  return circles->total == 0;
}

bool is_cell_empty(Vision *vision, int row, int column)
{
  return is_cell_empty_circles(vision, row, column) && is_cell_empty_std_dev(vision, row, column);
}

static void process_image(IplImage *file, Vision *vision)
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

  vision->board = board;

  CvRect **cells = get_cells(top_left, bottom_right, vision->board);

  vision->cells = cells;

  // Clear Memory
  cvClearSeq(markers);
  cvReleaseMemStorage(&tmp_storage);
  cvReleaseImage(&thresholded);
  cvReleaseImage(&file);
}

// Pre: Board is in initial state
void vision_init(Vision *vision)
{
  // Setting Up std_dev_empty
  IplImage *image = get_image();
  process_image(image, vision);
  vision->std_dev_empty = get_empty_std_dev(vision);
}

void vision_update(Vision *vision)
{
  cvReleaseImage(&vision->board);
  for (int i = 0; i < 8; i++)
  {
    free(vision->cells[i]);
  }
  free(vision->cells);
  IplImage *image = get_image();
  process_image(image, vision);
}

void vision_free(Vision *vision)
{
  cvReleaseImage(&vision->board);
  for (int i = 0; i < 8; i++)
  {
    free(vision->cells[i]);
  }
  free(vision->cells);
  free(vision);
}

int main(int argc, char **argv)
{
  Vision vision;
  vision_init(&vision);

  int count = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (is_cell_empty(&vision, i, j))
      {
        printf(".");
      }
      else
      {
        printf("O");
        count++;
      }
    }
    printf("\n");
  }
  printf("%d\n\n", count);
}
