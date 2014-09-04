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
#define SOURCEFILE "D:\\Objekterkennung\\test2.tiff"
#define TEMPLATE_DICE1 "D:\\Objekterkennung\\dice_template.png"

class ObjDetection{
private:

public:
	IplImage* analyseRealPicture(char* file);
	void findDice();
};