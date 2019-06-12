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

int main()
{
  // Loading image
  IplImage *image = cvLoadImage("images/chessboard.jpg", CV_WINDOW_AUTOSIZE);

  IplImage *thresholded = get_thresholded_image(image);

  // Creating Memory Storage
  CvMemStorage *storage = cvCreateMemStorage(0);

  // This will store the lines
  CvSeq *circles = NULL;

  circles = cvHoughCircles(thresholded, storage, CV_HOUGH_GRADIENT, 1, 50, 10, 10, 0, 500);

  // TODO: Differentiate topLeft and bottomRight
  float *topLeft = (float *)cvGetSeqElem(circles, 0);
  float *bottomRight = (float *)cvGetSeqElem(circles, 1);

  topLeft[0] += topLeft[2];
  topLeft[1] += topLeft[2];
  
  bottomRight[0] -= bottomRight[2];
  bottomRight[1] -= bottomRight[2];

  int width = bottomRight[0] - topLeft[0];
  int height = bottomRight[1] - topLeft[1];


  //int cell_width = width / 8;
 // int cell_height = height / 8;

  cvSetImageROI(image, cvRect(topLeft[0], topLeft[1], width, height));

  // Printing image
  cvShowImage("Original", image);
  cvWaitKey(0);
  cvReleaseImage(&image);
  cvDestroyAllWindows();
}
