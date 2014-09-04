#include "TOpenCV.h"
//#include "TObjekterkennung.h"




int main( int argc, char** argv )
{
	// OpenCV Objekt
	OpenCVObj objcv;
	// Grafik laden
	objcv.loadImageJPG("D:\\Objekterkennung\\test2.tiff");
	// Konturen erkennen
	Mat src = objcv.getImgMat();
	objcv.trackContours(src);
}


//int main(int argc, char** agrv){
//	
//	ObjDetection obj;
//	IplImage* test;
//	test = obj.analyseRealPicture("D:\\Objekterkennung\\test2.tiff");
//	Mat matrix(test);
//
//	namedWindow("Test");
//	imshow("Test", matrix);
//
//	cvWaitKey();
//	
//	return 0;
//}