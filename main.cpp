#include "TEckenerkennung.h"


int main (int argc, char** argv){
	/*HarrisCorner hc;

	Mat corner = hc.getHarrisCorner();
	Mat corner_scaled = hc.getScaledMatrix(corner);

	hc.showImgCircle(corner, corner_scaled);
	waitKey();*/

	ShiTomasi st;
	vector<Point2f> corners = st.getCorners();
	st.displayDice3(corners);
	//st.showImgCircles(corners);

	waitKey();
}