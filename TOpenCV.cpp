#include "TOpenCV.h"


// ### KONSTRUKTOR ###
OpenCVObj::OpenCVObj(){
	projectFolder = "D:\\Objekterkennung";
	thresh = 166;
	max_thresh = 255;
	rng(12345);
}

// ### GRAFIK EINLESEN IN EINE MATRIX ###
int OpenCVObj::loadImageJPG(string img){
	// liest die Grafik in eine Matrix unverändert ein
	IplImage* pic;
	// Konvertierung von String zu Char*
	const char * path = img.c_str();
	try{
		pic = cvLoadImage(path);
	} catch (exception& e){
		cout << "Error beim Laden der Grafik" << endl;
		return 0;
	}
	Mat tmp(pic);
	OpenCVObj::imgMatrix = tmp; 
	return 1;
}

// ### WIEDERGABE DES PROJEKTORDNERS ###
string OpenCVObj::getProjectFolder(){
	return OpenCVObj::projectFolder;
}


// ###  RETURN IMGMAT ###
Mat OpenCVObj::getImgMat(){
	return OpenCVObj::imgMatrix;
}


// ### TRACK CONTOURS ###
void OpenCVObj::trackContours(Mat src){
	// Konvertierung in Graustufen
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3,3));

	showImg("Source", src);

	
	waitKey(0);
}

// ### SHOW IMAGE IN CV WINDOW ###
void OpenCVObj::showImg(string title, Mat src){
	namedWindow(title, CV_WINDOW_AUTOSIZE);
	imshow(title, src);
}


