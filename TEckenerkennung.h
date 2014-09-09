#include "opencv2\highgui\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;


class HarrisCorner{
private:
	Mat src;
	int thresh;
	int max_thresh;
	char* source_window;
	char* corners_window;
	// Detector Parameter
	int blockSize;
	int apertureSize;
	double k;

public:
	HarrisCorner(){
		src = imread("D:\\Objekterkennung\\Base6.png", CV_LOAD_IMAGE_GRAYSCALE);
		thresh = 150;
		max_thresh = 255;
		source_window = "Source image";
		corners_window = "Eckenerkennung";
		// Detector Parameter
		blockSize = 4;
		apertureSize = 3;
		k = 0.08;
	}
	void loadImage(char* file){
		src = imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	}
	Mat getHarrisCorner(){
		Mat dst, dst_norm, dst_norm_scaled;
		dst = Mat::zeros(src.size(), CV_32FC1);

		// Eckenerkennen
		cornerHarris(src, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

		// Normalize (Kontrast anpassen)
		normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());

		return dst_norm;
	}
	Mat getScaledMatrix(Mat src){
		Mat dst;
		convertScaleAbs(src, dst);

		return dst;
	}
	void showImgCircle(Mat src, Mat src_scaled){
		int count = 0;
		for (int j = 0; j < src.rows; j++){
			for (int i = 0; i < src.cols; i++){
				if ((int) src.at<float>(j,i) > thresh){
					circle (src_scaled, Point(i,j), 5, Scalar(0), 2, 8, 0);
					count++;
				}
			}
		}
		string str_count = to_string(count); 
		cout << "Anzahl der Kreise: " + str_count << endl;
		namedWindow( corners_window, CV_WINDOW_AUTOSIZE );
		imshow( corners_window, src_scaled );
		imwrite("D:\\Objekterkennung\\out.png", src_scaled);
	}

};


class ShiTomasi{
private:
	Mat src;
	// Dector Parameter (Eckenerkennung)
	int maxCorners;
	double qualityLevel;
	double minDistance;
	int blockSize;
	bool useHarrisDetector;
	double k;
	

	// private Constructor
	//ShiTomasi() {};
public:
	ShiTomasi() {
		//src = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		src = imread("D:\\Objekterkennung\\test2.tiff", CV_LOAD_IMAGE_GRAYSCALE);
		// Detector Parameter
		maxCorners = 30;
		qualityLevel = 0.01;
		minDistance = 9;
		blockSize = 3;
		useHarrisDetector = false;
		k = 0.04;

		

	}
	void loadImage(char* file){
		src = imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	}
	vector<Point2f> getCorners(){
		vector<Point2f> corners;
		// Eckenerkennen
		goodFeaturesToTrack(src, corners, maxCorners, qualityLevel, 
			minDistance, Mat(), blockSize, useHarrisDetector, k);
		cout << "Erkannte Ecken :" << corners.size()<<endl;
		return corners;
	}
	void showImgCircles(vector<Point2f> corners){
		/// Kopie der Quelle
		Mat copy;
		copy = src.clone();

		int r = 4;
		for( int i = 0; i < corners.size(); i++ )
			{ circle( copy, corners[i], r, Scalar(0),
					 2, 8, 0 ); }
		namedWindow("Ecken", CV_WINDOW_AUTOSIZE);
		imshow("Ecken", copy);
		imwrite("D:\\Objekterkennung\\ShiTomasi.png", copy);
	}
	void displayDice3(vector<Point2f> corners){
		vector<Point2f> points = sortVectorPoints(corners);

		// Diagonalen berechnen
		Point2f d1;
		d1.x= points[0].x - points[3].x;
		d1.y= points[0].y - points[3].y;
		Point2f d2;
		d2.x = points[1].x - points[2].x;
		d2.y = points[1].y - points[2].y;

		// euklidische Länge der Diagonalen
		double d1l = sqrt(pow(d1.x,2) + pow(d1.y,2));
		double d2l = sqrt(pow(d2.x,2) + pow(d2.y,2));

		// Flächeninhalt 
		double a = 0.5 * d1l * d2l;
		cout << "Flächeninhalt eines Würfels (Augenzahl 3): "<< a << endl;
		// Gerade 1
		double m = 0.0;
		double b = 0.0;
		if ((points[0].y == points[3].y) ||(points[0].x == points[3].x)){
			m = 0.0;
		} else {
			m = (points[0].y - points[3].y) / (points[0].x - points[3].x);
		}
		b = points[0].y - points[0].x * m;

		double line1[2];
		line1[0] = m;
		line1[1] = b;
		// Gerade 2
		m = 0.0;
		b = 0.0;
		if ((points[1].y == points[2].y) ||(points[1].x == points[2].x)){
			m = 0.0;
		} else {
			m = (points[1].y - points[2].y) / (points[1].x - points[2].x);
		}
		b = points[1].y - points[1].x * m;

		double line2[2];
		line2[0] = m;
		line2[1] = b;

		// Schnittpunkt der zwei Geraden
		double xs = line1[0] - line2[0]; 
		double bs = line2[1] - line1[1];
		double x = bs/xs;
		double y = x * line1[0] + line1[1];
		Point2f intersection;
		intersection.x = x;
		intersection.y = y;

		Mat copy;
		copy = src.clone();
		int r = 4;
		circle( copy, intersection, r, Scalar(0), 2, 8, 0 ); 
		namedWindow("Schnittpunkt", CV_WINDOW_AUTOSIZE);
		imshow("Schnittpunkt", copy);
	}
	// sortiert die Würfeldaten nach einer Drei
	vector<Point2f> sortVectorPoints(vector<Point2f> vec){
		std::sort(vec.begin(), vec.end(), [](const cv::Point2f &a, const cv::Point2f &b) {
		return a.x*a.x + a.y*a.y < b.x*b.x + b.y*b.y;});
		return vec;
	}
};