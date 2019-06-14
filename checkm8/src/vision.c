#include "vision.h"

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/videoio/videoio_c.h>

#define MIN_HUE 77
#define MAX_HUE 87

#define PADDING 0.03

#define DELTA_LINE_ERROR 2
#define DELTA_LINE 3

#define THRESH_STD_DEV 14
#define THRESH_CIRCLES 22

#define CAMERA 1

/* Reads an image from the camera */
static IplImage *get_image_from_camera(Vision *vision)
{
  system("rm images/chessboard.jpg");
  char python_request[60];
  snprintf(python_request, 60, "python python/read_stream.py %s", vision->camera_url);
  system(python_request);
  IplImage *image = cvLoadImage("images/chessboard.jpg", CV_WINDOW_AUTOSIZE);
  return image;
}

/* Reads an image from a given path */
static IplImage *get_image_from_path(char *path)
{
  IplImage *image = cvLoadImage(path, CV_WINDOW_AUTOSIZE);
  return image;
}

/* Depending on the CAMERA constant, get the image from Camera or Path */
static IplImage *get_image(Vision *vision)
{
  if (CAMERA)
  {
    return get_image_from_camera(vision);
  }
  else
  {
    return get_image_from_path("images/chessboard.jpg");
  }
}

/* Isolates the markers on the board */
static IplImage *get_thresholded_image(IplImage *img)
{
  IplImage *imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
  cvCvtColor(img, imgHSV, CV_BGR2HSV);

  IplImage *thresholded_img = cvCreateImage(cvGetSize(img), 8, 1);

  cvInRangeS(imgHSV, cvScalar(MIN_HUE, 100, 100, 0), cvScalar(MAX_HUE, 255, 255, 0), thresholded_img);

  cvReleaseImage(&imgHSV);
  return thresholded_img;
}

/* Finds top left corner and bottom right corner */
static void get_corners(CvSeq *elems, float **top_left, float **bottom_right)
{
  float *point1 = (float *)cvGetSeqElem(elems, 0);
  float *point2 = (float *)cvGetSeqElem(elems, 1);
  if (point1[0] < point2[0])
  {
    point1[0] += point1[2];
    point1[1] += point1[2];
    point2[0] -= point2[2];
    point2[1] -= point2[2];

    *top_left = point1;
    *bottom_right = point2;
  }
  else
  {
    point2[0] += point2[2];
    point2[1] += point2[2];
    point1[0] -= point1[2];
    point1[1] -= point1[2];

    *top_left = point2;
    *bottom_right = point1;
  }
}

/* Finds horizontal and vertical lines */
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

/* Calculates the coordinates of each cell on the board */
static CvRect **get_cells(float *top_left, float *bottom_right, IplImage *board)
{
  // Allocate memory for the 2D array
  CvRect **cells = malloc(8 * sizeof(CvRect *));
  if(!cells)
  {
    printf("Vision Error: Cannot allocate cells 2d array\n");
    return NULL;
  }

  // We calculate the board dimensions and the cell dimensions
  int width = bottom_right[0] - top_left[0];
  int height = bottom_right[1] - top_left[1];
  int cell_width = width / 8;
  int cell_height = height / 8;

  // We calculate an initial padding to remove potential errors
  float padding_x = PADDING * cell_width;
  float padding_y = PADDING * cell_height;

  CvSeq *lines = NULL;
  CvMemStorage *storage = cvCreateMemStorage(0);

  for (int i = 0; i < 8; i++)
  {
    cells[i] = malloc(8 * sizeof(CvRect));
    if(!cells[i])
    {
      printf("Vision Error: Cannot allocate cells[%d] array\n", i);
      return NULL;
    }
    for (int j = 0; j < 8; j++)
    {
      // Apply the initial padding
      int pos_x = top_left[0] + j * cell_width + padding_x;
      int pos_y = top_left[1] + i * cell_height + padding_y;
      int wid = cell_width - 2 * padding_x;
      int hei = cell_height - 2 * padding_y;

      if(!(pos_x >= 0 && pos_y >= 0 && wid >= 1 && hei >= 1))
      {
        printf("Vision Error: Wrong coordinates in calculation of cell[%d][%d]\n", i, j);
        return NULL;
      }
      cells[i][j] = cvRect(pos_x, pos_y, wid, hei);

      // Detect if lines are found in the cells
      bool lines_remain = true;
      while (lines_remain)
      {
        // We use Canny edge detector to find the contours in our cell
        IplImage *image1 = cvCreateImage(cvGetSize(board), 8, 1);
        if(!image1)
        {
          printf("Vision Error: Cannot detect edges on cell[%d][%d]\n", i, j);
          return NULL;
        }
        cvCanny(board, image1, 50, 200, 3);

        // Focus the board image on the cell
        cvSetImageROI(image1, cells[i][j]);
        
        // Find lines in the cell
        int min_length = MIN(wid, hei);
        min_length -= 0.5 * min_length;
        lines = cvHoughLines2(image1, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 35, min_length, 40, 0, 360);

        // Get horizontal and vertical lines
        CvSeq *h_lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint) * 2, storage);
        CvSeq *v_lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint) * 2, storage);
        get_lines(lines, h_lines, v_lines);

        // If line is horizontal, we will crop the top or the bottom
        float padding_top = 0, padding_bot = 0;
        float center_y = hei / 2;
        for (int h = 0; h < h_lines->total; h++)
        {
          CvPoint *line = (CvPoint *)cvGetSeqElem(h_lines, h);
          // If line is at the bottom, we crop the bottom
          if (line[0].y > center_y)
          {
            float new_padding_bot = hei - MIN(line[0].y, line[1].y);
            padding_bot = MAX(padding_bot, new_padding_bot) + DELTA_LINE_ERROR;
          }
          // If line is at the top, we crop the top
          else
          {
            float new_padding_top = MAX(line[0].y, line[1].y);
            padding_top = MAX(padding_top, new_padding_top) + DELTA_LINE_ERROR;
          }
        }

        // If line is vertical, we will crop the left or the right
        float padding_left = 0, padding_right = 0;
        float center_x = wid / 2;
        for (int v = 0; v < v_lines->total; v++)
        {
          CvPoint *line = (CvPoint *)cvGetSeqElem(v_lines, v);
          // If line is at the right, we will crop the right
          if (line[0].x > center_x)
          {
            float new_padding_right = wid - MIN(line[0].x, line[1].x);
            padding_right = MAX(padding_right, new_padding_right) + DELTA_LINE_ERROR;
          }
          // If line is at the left, we will crop the left
          else
          {
            float new_padding_left = MAX(line[0].x, line[1].x);
            padding_left = MAX(padding_left, new_padding_left) + DELTA_LINE_ERROR;
          }
        }

        // Apply the paddings that has been calculated
        pos_x += padding_left;
        pos_y += padding_top;
        wid -= padding_right + padding_left;
        hei -= padding_top + padding_bot;
        cells[i][j] = cvRect(pos_x, pos_y, wid, hei);

        // If no padding has been found, the cell has no more line
        lines_remain = !(padding_left < 1 && padding_right < 1 && padding_top < 1 && padding_bot < 1);
      }
    }
  }
  // Free the memory
  cvClearSeq(lines);
  cvReleaseMemStorage(&storage);

  return cells;
}

static IplImage *get_cell(Vision *vision, int row, int column)
{
  cvResetImageROI(vision->board);
  cvSetImageROI(vision->board, vision->cells[row][column]);
  return vision->board;
}

/* Returns an average of the standard deviations of known empty cells on an initial state */
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

/* Returns the standard deviation of the cell */
static float get_cell_std_dev(Vision *vision, int row, int col)
{
  CvScalar mean, std_dev;
  cvAvgSdv(get_cell(vision, row, col), &mean, &std_dev, NULL);

  return std_dev.val[0];
}

/* Detects if the cell is empty by comparing the standard deviation to the one of an empty cell */
static bool is_cell_empty_std_dev(Vision *vision, int row, int column)
{
  float std_dev_cell = get_cell_std_dev(vision, row, column);

  return fabs(std_dev_cell - vision->std_dev_empty) <= THRESH_STD_DEV;
}

/* Detects if the cell is empty by trying to find circles in the cell */
static bool is_cell_empty_circles(Vision *vision, int row, int column)
{
  CvMemStorage *storage = cvCreateMemStorage(0);

  CvSeq *circles = NULL;

  IplImage *image = get_cell(vision, row, column);
  circles = cvHoughCircles(image, storage, CV_HOUGH_GRADIENT, 1, 100, 25, THRESH_CIRCLES, 5, 20);

  return circles->total == 0;
}

/* Detects if the cell is empty */
bool is_cell_empty(Vision *vision, int row, int column)
{
  // Combination of two detection algorithms
  return is_cell_empty_circles(vision, row, column) && is_cell_empty_std_dev(vision, row, column);
}

/* Detects markers on board and calculates corresponding cells coordinates */
static bool process_image(IplImage *file, Vision *vision)
{
  // Isolate markers on board
  IplImage *thresholded = get_thresholded_image(file);
  if(!thresholded)
  {
    printf("Vision Error: Cannot identify markers on board\n");
    return false;
  }

  // Convert board image to greyscale
  IplImage *board = cvCreateImage(cvGetSize(file), 8, 1);
  cvCvtColor(file, board, CV_BGR2GRAY);

  // Allocate temporary memory storage
  CvMemStorage *tmp_storage = cvCreateMemStorage(0);

  // Detect the circle markers
  CvSeq *markers = NULL;
  markers = cvHoughCircles(thresholded, tmp_storage, CV_HOUGH_GRADIENT, 1, 50, 10, 10, 0, 500);

  if(!markers->total)
  {
    printf("Vision Error: No markers were found\n");
    return false;
  }

  // Get coordinates of markers
  float *top_left;
  float *bottom_right;
  get_corners(markers, &top_left, &bottom_right);

  // Set the values of the vision struct
  vision->board = board;
  CvRect **cells = get_cells(top_left, bottom_right, vision->board);
  if(!cells)
  {
    printf("Vision Error: Cannot calculate cells coordinates\n");
    return false;
  }
  vision->cells = cells;

  // Clear Memory
  cvClearSeq(markers);
  cvReleaseMemStorage(&tmp_storage);
  cvReleaseImage(&thresholded);
  cvReleaseImage(&file);

  return true;
}

/* Initialises Vision struct */
// Pre: Board is in initial state
bool vision_init(Vision *vision, char *url)
{
  if(!url)
  {
    printf("Vision Error: URL is null\n");
    return false;
  }
  vision->camera_url = url;

  // Setting Up std_dev_empty
  IplImage *image = get_image(vision);
  if(!image)
  {
    printf("Vision Error: Cannot read image to initialise Vision (CAMERA = %d)\n", CAMERA);
    return false;
  }
  if(!process_image(image, vision))
  {
    printf("Vision Error: Cannot process image to initialise Vision\n");
    return false;
  }
  vision->std_dev_empty = get_empty_std_dev(vision);

  return true;
}

/* Updates image and cells coordinates */
void vision_update(Vision *vision)
{
  cvReleaseImage(&vision->board);
  for (int i = 0; i < 8; i++)
  {
    free(vision->cells[i]);
  }
  free(vision->cells);
  IplImage *image = get_image(vision);
  process_image(image, vision);
}

/* Frees Vision struct */
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
