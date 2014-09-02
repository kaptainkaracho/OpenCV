#include "TObjekterkennung.h"

IplImage* ObjDetection::analyseRealPicture(char* file){
	IplImage* img;
	if (file == NULL){
		img = cvLoadImage(NULL);
	} else {
		img = cvLoadImage(file);
	}

	IplImage* binaryImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1); 
    IplImage* normalized = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1); 
    IplImage* gauss = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1); 
    IplImage* cannyimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1); 
    IplImage* newimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    
	binaryImage = cvCreateImage ( cvGetSize(img),IPL_DEPTH_8U, 1); 
    cvSplit (img, binaryImage, NULL , NULL , NULL );

	// neues Bild für den Kontrast
    normalized = cvCreateImage ( cvGetSize (img), img ->depth , 1); 
    // Anpassen von Helligkeit und Kontrast 
    cvEqualizeHist ( binaryImage , normalized );
    // Gaussfilter
    gauss = cvCreateImage ( cvGetSize (img), img->depth , 1); 
    cvSmooth ( normalized , gauss , CV_GAUSSIAN , 13, 13);
    // Konturen filtern mit dem Canny-Alogrithmus 
    cannyimg = cvCreateImage ( cvGetSize (img), img ->depth , 1); 
    cvCanny(gauss, cannyimg, 40, 130);

	// Speicher für die Konturen allozieren
	CvMemStorage* canny_storage = cvCreateMemStorage(0); 
	CvSeq* contoursOriginalPicture = 0;

}

IplImage* ObjDetection::analyseAbstractPicture(char* file){

}

