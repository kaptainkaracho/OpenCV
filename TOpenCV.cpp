#include "TOpenCV.h"


// ### KONSTRUKTOR ###
OpenCVObj::OpenCVObj(){
	projectFolder = "D:\\Objekterkennung";
	thresh = 166;
	max_thresh = 255;
	rng(12345);
	deviation = 0.3;
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
	// Zeige das Original
	showImg("Source", src);

	// Canny Funktion, um Konturen zu filtern
	Mat canny_out;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	Canny( src_gray, canny_out, thresh, thresh*2, 3);
	findContours(canny_out, contours, hierarchy, CV_RETR_TREE, 
		CV_CHAIN_APPROX_SIMPLE, Point(0,0)); 

	// Zeichnen der Konturen
	Mat drawing = Mat::zeros(canny_out.size(), CV_8UC3);
	/*for (int i = 0; i < contours.size(); i++){
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	}*/

	
	// Zeichne nur Würfel !(kalibriert an test2.tiff)!
	for (int i = 0; i < contours.size(); i++){
		double area = calcSurfArea(contours[i]);
		if ((area <= (398.5*(1+deviation))) && (area >= (398.5*(1-deviation)))){
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
			drawContours(drawing, contours, i, color);
		}
	}


	showImg("Konturen", drawing);

	waitKey(0);
}

// ### SHOW IMAGE IN CV WINDOW ###
void OpenCVObj::showImg(string title, Mat src){
	namedWindow(title, CV_WINDOW_AUTOSIZE);
	imshow(title, src);
}

// ### BERECHNET DEN FLÄCHENINHALT EINES POLYGONS ### 
double OpenCVObj::calcSurfArea(vector<Point> contour){
	double surfArea;
	surfArea = 0.0;
	int dx; int dy;
	dx = 0;
	dy = 0;
	for (int i = 0; i < contour.size()-1; i++){
		dx = dx + (contour[i].x * contour[i+1].y);
		dy = dy + (contour[i].y * contour[i+1].x);
	}
	// Umfang
	double d = dx - dy;
	// Flächeninhalt
	surfArea = d / 2;
	if (surfArea < 0){
		surfArea = surfArea * -1;
	}
	return surfArea;
}




