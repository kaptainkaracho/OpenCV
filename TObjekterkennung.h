// INCLUDES
#include <opencv/cv.h> 
#include <opencv/highgui.h> 
#include <conio.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <iostream> 
#include <math.h> 


// NAMESPACES
using namespace cv;
using namespace std;

// DEFINITIONEN
#define PROJECTFOLDER "D:\\Objekterkennung"

class ObjDetection{
private:

public:
	IplImage* analyseAbstractPicture(char* file);
	IplImage* analyseRealPicture(char* file);
};