// INCLUDES
#include "TEckenerkennung.h"


// NAMESPACES
using namespace cv;
using namespace std;

class ObjDetection{
private:
	vector<vector<Point2f>> group;
	ShiTomasi cornerDetector;
	vector<Point2f> corners;

	// Gruppierungsparameter
	/*double f1;
	double f2;
	double dist1;
	double dist2;*/
	double area_min;
	double area_max;
public:
	ObjDetection(char* filename){
		// Gruppierungsparameter
		//// distance
		//dist1 = 12.0;
		//dist2 = 26.0;
		//// deviation
		//f1 = 0.91;
		//f2 = 0.96;
		// Fläche eines Würfels
		area_min = 100;
		area_max = 180;

		ShiTomasi st;
		cornerDetector = st;
		// erfasse Ecken 
		vector<Point2f> tmpCorners;
		tmpCorners = cornerDetector.getCorners();
		// sortierte Punkte 
		corners = cornerDetector.sortVectorPoints(tmpCorners);
		// exportiere Ecken in eine CSV
		cornerDetector.exportCorners("D:\\Objekterkennung\\corners.csv", corners);
	}
	int groupCorners(vector<Point2f> points){
		int hit = 0;

		// Gruppierung
		for (int i = 0; i < points.size(); i++){
			for (int j = 0; j < points.size(); j++){
				for (int k = 0; k < points.size(); k++){
					for (int l = 0; l < points.size(); l++){
						if ((i != j) && ( i != k) && ( i != l) &&
							(j != k ) && ( j != l) && ( k != l)){
							vector<Point2f> tmpGroup;
							tmpGroup.push_back(points[i]);
							tmpGroup.push_back(points[j]);
							tmpGroup.push_back(points[k]);
							tmpGroup.push_back(points[l]);
							double a = cornerDetector.calcArea(tmpGroup);
							cout << "i: "<< i << " j: " << j << " k: " << k << " l: " << l << endl; 
							cout << "Flaecheninhalt: " << a << endl;
							if ((a >= area_min) && (a <= area_max)){
								group.push_back(tmpGroup);
								points.erase(points.begin()+i);
								points.erase(points.begin()+j);
								points.erase(points.begin()+k);
								points.erase(points.begin()+l);
								hit = 1;
							} 
							tmpGroup.clear();
							if (hit > 0){
								groupCorners(points);
								return 0;
							}
						}
					}
				}
			}				
		}
		return 0;
	}
	void displayGroup(){
		vector<Point2f> centers;
		for (int i = 0; i < group.size(); i++){
			centers.push_back(cornerDetector.centerDice(group[i]));
		}
	}
};
