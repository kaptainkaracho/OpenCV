#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv\cv.h>
#include <iostream>


using namespace cv;
using namespace std;


class OpenCVObj{
private:
	string projectFolder;
	Mat imgMatrix;
	Mat contoursMat;
public:
	OpenCVObj();
	string getProjectFolder();
	int loadImageJPG(string img);
	Mat detectContours(); 
	void extractContours();
	void test(Mat contours);
	void showImg(Mat matrix, string title);
	void saveImg(Mat matrix, string name);
};