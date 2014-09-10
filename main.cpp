#include "TObjekterkennung.h"


int main (int argc, char** argv){
	
	ObjDetection obj("D:\\Objekterkennung\\test2.tiff");
	obj.displayGroup();
	waitKey();
}