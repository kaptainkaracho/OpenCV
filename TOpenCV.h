#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;


class OpenCVObj{
private:
	string projectFolder;
	Mat imgMatrix;
	Mat contoursMat;
	int thresh;
	int max_thresh;
	RNG rng;
	Mat src_gray;
	double deviation; 
public:
	OpenCVObj();
	string getProjectFolder();
	int loadImageJPG(string img);
	void showImg(string title, Mat src);
	void trackContours(Mat src);
	Mat getImgMat();
	double calcSurfArea(vector<Point> contour);
};