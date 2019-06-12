#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc_c.h>

void get_lines(CvSeq *lines, CvSeq *h_lines, CvSeq *v_lines)
{
  for (int i = 0; i < lines->total; i++)
  {
    CvPoint *line = (CvPoint *)cvGetSeqElem(lines, i);
    if (abs(line[1].y - line[0].y) < 1)
    {
      cvSeqPush(h_lines, line);
    }
    else if (abs(line[1].x - line[0].x) < 1)
    {
      cvSeqPush(v_lines, line);
    }
  }
}

bool intersection(CvPoint o1, CvPoint p1, CvPoint o2, CvPoint p2, CvPoint *r)
{
  CvPoint x = cvPoint(o2.x - o1.x, o2.y - o1.y);
  CvPoint d1 = cvPoint(p1.x - o1.x, p1.y - o1.y);
  CvPoint d2 = cvPoint(p2.x - o2.x, p2.y - o2.y);

  float cross = d1.x * d2.y - d1.y * d2.x;

  if (fabs(cross) < 1e-8)
    return false;

  double t1 = (x.x * d2.y - x.y * d2.x) / cross;
  *r = cvPoint(o1.x + d1.x * t1, o1.y + d1.y * t1);
  return true;
}

int main()
{

  /* Loading the image */
  IplImage *image = cvLoadImage("images/chessboard.jpg", CV_WINDOW_AUTOSIZE);

  // Croping the image to remove the numberson the sides
  cvSetImageROI(image, cvRect(25, 32, image->width - 50, image->height - 60));

  // We'll use a temporary image with a single colour channel, to recognise the lines
  IplImage *image1 = cvCreateImage(cvGetSize(image), 8, 1);
  // IplImage* image2 = cvCreateImage(cvGetSize(image), 8, 3);

  // Creating Memory Storage
  CvMemStorage *storage = cvCreateMemStorage(0);

  // This will store the lines
  CvSeq *lines = NULL;

  // Converting our image to greyscale
  cvCanny(image, image1, 50, 200, 3);
  // cvCvtColor(image1, image2, CV_GRAY2BGR);

  // Using Hough Lines Probabilistic Algorithm
  lines = cvHoughLines2(image1, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 40, 0, 360);

  // Get Horizontal & Vertical lines
  CvSeq *h_lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint) * 2, storage);
  CvSeq *v_lines = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint) * 2, storage);
  get_lines(lines, h_lines, v_lines);

  // Get list of intersection points
  CvSeq *points = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint *), storage);
  CvPoint *point = calloc(1, sizeof(CvPoint));

  for (int j = 0; j < h_lines->total; j++)
  {
    CvPoint *h_line = (CvPoint *)cvGetSeqElem(h_lines, j);
    for (int i = 0; i < v_lines->total; i++)
    {
      CvPoint *v_line = (CvPoint *)cvGetSeqElem(v_lines, i);
      if (intersection(h_line[0], h_line[1], v_line[0], v_line[1], point))
      {
        cvSeqPush(points, point);
      }
    }
  }

  printf("Found %d points \n", points->total);

  // Drawing points
  for (int i = 0; i < points->total; i++)
  {
    CvPoint *point_to_draw = (CvPoint *)cvGetSeqElem(points, i);
    cvLine(image, *point_to_draw, *point_to_draw, cvScalar(0, 0, 255, 0), 3, 8, 0);
  }

  // Printing image
  cvNamedWindow("image", 0);
  cvShowImage("Original", image);
  //cvShowImage("Chessboard lines bruv", image2);
  cvWaitKey(0);
  cvReleaseImage(&image);
  //cvReleaseImage(&image2);
  cvDestroyAllWindows();
}
