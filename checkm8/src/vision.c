#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>

int main()
{
  // Sample Image Loading Test
  IplImage *im = cvLoadImage("image.jpg",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("image", 0);
  cvShowImage("image", im);
  cvWaitKey(0);
  cvReleaseImage(&im);
  cvDestroyAllWindows();
}
