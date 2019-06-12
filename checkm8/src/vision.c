#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc_c.h>

IplImage *get_thresholded_image(IplImage *img)
{
  IplImage *imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
  cvCvtColor(img, imgHSV, CV_BGR2HSV);

  IplImage *thresholded_img = cvCreateImage(cvGetSize(img), 8, 1);

  cvInRangeS(imgHSV, cvScalar(77, 100, 100, 0), cvScalar(87, 255, 255, 0), thresholded_img);

  cvReleaseImage(&imgHSV);
  return thresholded_img;
}

void get_corners(CvSeq *elems, float **topLeft, float **bottomRight)
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

CvRect **get_cells(float *topLeft, float *bottomRight)
{
  CvRect **cells = malloc(8 * sizeof(CvRect *));

  int width = bottomRight[0] - topLeft[0];
  int height = bottomRight[1] - topLeft[1];
  int cell_width = width / 8;
  int cell_height = height / 8;

  float padding_x = 0.03 * width;
  float padding_y = 0.03 * height;

  for(int i = 0; i < 8; i++)
  {
    cells[i] = malloc(8 * sizeof(CvRect));
    for (int j = 0; j < 8; j++)
    {
      cells[i][j] = cvRect(topLeft[0]+j*cell_width + padding_x, topLeft[1]+i*cell_height + padding_y, cell_width - 2 *padding_x, cell_height - 2 *padding_y);
    }
  }
  return cells;
}

IplImage *get_cell(IplImage *board, CvRect **cells, int row, int column)
{
  cvResetImageROI(board);
  cvSetImageROI(board, cells[row][column]);
  return board;
}

float get_empty_std_dev(IplImage *board, CvRect **cells)
{
  float dev = 0.0f;

  CvScalar mean, std_dev;
  cvAvgSdv(get_cell(board, cells, 2, 0), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(board, cells, 2, 1), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(board, cells, 5, 0), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(board, cells, 5, 1), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(board, cells, 2, 6), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(board, cells, 2, 7), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(board, cells, 5, 6), &mean, &std_dev, NULL);
  dev += std_dev.val[0];
  cvAvgSdv(get_cell(board, cells, 5, 7), &mean, &std_dev, NULL);
  dev += std_dev.val[0];

  dev /= 8.0f;

  return dev;
}

float get_cell_std_dev(IplImage *board, CvRect **cells, int row, int col)
{
  CvScalar mean, std_dev;
  cvAvgSdv(get_cell(board, cells, row, col), &mean, &std_dev, NULL);

  return std_dev.val[0];
}

bool is_cell_empty(IplImage *board, CvRect **cells, int row, int column, float std_dev_empty)
{
  float std_dev_cell = get_cell_std_dev(board, cells, row, column);

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

int main()
{
  // Loading image
  IplImage *original = cvLoadImage("images/chessboard.jpg", CV_WINDOW_AUTOSIZE);

  IplImage *thresholded = get_thresholded_image(original);

  // Creating Memory Storage
  CvMemStorage *storage = cvCreateMemStorage(0);

  // This will store the lines
  CvSeq *circles = NULL;

  circles = cvHoughCircles(thresholded, storage, CV_HOUGH_GRADIENT, 1, 50, 10, 10, 0, 500);
  cvReleaseImage(&thresholded);

  // Determine which circle is topLeft and which is bottomRight
  float *topLeft;
  float *bottomRight;
  get_corners(circles, &topLeft, &bottomRight);

  IplImage *board = cvCreateImage(cvGetSize(original), 8, 1);
  cvCvtColor(original, board, CV_BGR2GRAY);
  cvReleaseImage(&original);

  CvRect **cells = get_cells(topLeft, bottomRight);


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

  cvShowImage("cells[7][7]", get_cell(board, cells, 4, 1));
  cvWaitKey(0);
  cvDestroyAllWindows();
}
