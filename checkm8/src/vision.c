#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc_c.h>

int main()
{
  /* Loading the image */
  IplImage *image = cvLoadImage("images/chessboard.jpg", CV_WINDOW_AUTOSIZE);

  /* Croping the image to remove the numberson the sides */
  cvSetImageROI(image, cvRect(25, 32, image->width - 50, image->height - 60));
  
  /* We'll use a temporary image with a single colour channel, to recognise the lines */
  IplImage *image1 = cvCreateImage(cvGetSize(image), 8, 1);
  // IplImage* image2 = cvCreateImage(cvGetSize(image), 8, 3);

  /* Creating Memory Storage */
  CvMemStorage *storage = cvCreateMemStorage(0);

  /* This will store the lines */
  CvSeq *lines = NULL;

  /* Converting our image to greyscale */
  cvCanny(image, image1, 50, 200, 3);
  // cvCvtColor(image1, image2, CV_GRAY2BGR);

  /* Using Hough Lines Probabilistic Algorithm */
  lines = cvHoughLines2(image1, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 40, 0, 360);

  /* Writing all lines to image */
  for (int i = 0; i < lines->total; i++)
  {
    CvPoint *line = (CvPoint *) cvGetSeqElem(lines, i);
    cvLine(image, line[0], line[1], cvScalar(0, 0, 255, 0), 3, 8, 0);
  }

  /* Printing image */
  cvNamedWindow("image", 0);
  cvShowImage("Original", image);
  //cvShowImage("Chessboard lines bruv", image2);
  cvWaitKey(0);
  cvReleaseImage(&image);
  //cvReleaseImage(&image2);
  cvDestroyAllWindows();
}
