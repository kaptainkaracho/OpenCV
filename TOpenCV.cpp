#include "TOpenCV.h"


// ### KONSTRUKTOR ###
OpenCVObj::OpenCVObj(){
	OpenCVObj::projectFolder = "D:\\Objekterkennung";
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

// ### DARSTELLUNG EINER MATRIX IN EINEM FENSTER ###
void OpenCVObj::showImg(Mat matrix, string title){
	// Fenster definieren
	namedWindow(title);
	// Bild zeigen
	imshow(title, matrix);
	// Wartet auf eine Benutzereingabe 
	waitKey(0);
}

// ### SPEICHERN EINER GRAFIK IM PROJEKTORDNER ###
void OpenCVObj::saveImg(Mat matrix, string name){
	imwrite(OpenCVObj::projectFolder + "\\" + name, matrix); 
	cout << "Grafik wurde unter dem Namen " +  name + " im Projektordner gespeichert." << endl;
}

// ### ERKENNEN VON KONTUREN ###
Mat OpenCVObj::detectContours(){
	Mat contours;
	Canny(OpenCVObj::imgMatrix, contours,125,350);
	// invertieren von Schwarzweiß
	threshold(contours, contours, 128, 255, THRESH_BINARY_INV);
	return contours;
}

// ### EXTRAHIEREN VON KONTUREN ###
void OpenCVObj::extractContours(){
	cout << "Not implemented yet" << endl;
}

void OpenCVObj::test(Mat contours){
	// Graustufen 
	Mat cdst;
	cvtColor(contours, cdst, CV_GRAY2BGR);
	// Hough transform für Linienerkennung
	vector<Vec2f> lines;
	HoughLines(contours, 
		lines, 
		1, 
		CV_PI/180, 
		80);

	// Linien zeichnen
	for (size_t i = 0; i < lines.size(); i++){
		float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }

	waitKey();
}


