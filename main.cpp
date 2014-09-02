//#include "TOpenCV.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;


//int main( int argc, char** argv )
//{
//	// OpenCV Objekt
//	OpenCVObj objcv;
//	// Grafik laden
//	objcv.loadImageJPG("D:\\Objekterkennung\\test2.tiff");
//	// Konturen erkennen
//	Mat src = objcv.getImgMat();
//	objcv.trackContours(src);
//}


int main(int argc, char** agrv){
	
	IplImage* img;
	img = cvLoadImage("D:\\Objekterkennung\\test2.tiff");
	IplImage* binaryImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

	cvSplit(img, binaryImage, NULL, NULL, NULL);

	IplImage* normalized = cvCreateImage(cvGetSize(img), img-> depth, 1);
	cvEqualizeHist(binaryImage, normalized);

	IplImage* gauss = cvCreateImage(cvGetSize(img), img -> depth, 1);
	cvSmooth(normalized, gauss, CV_GAUSSIAN, 13, 13);

	IplImage* cannyimg = cvCreateImage ( cvGetSize (img), img ->depth , 1);
	cvCanny(gauss, cannyimg, 40, 130);

	Mat matrix(cannyimg);
	namedWindow("Konturen mit Canny", CV_WINDOW_AUTOSIZE);
	imshow("Konturen mit Canny", matrix);

	cvWaitKey();

	return 0;
}