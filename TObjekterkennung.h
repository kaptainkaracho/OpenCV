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
	double f1;
	double f2;
	double dist1;
	double dist2;
public:
	ObjDetection(char* filename){
		// Gruppierungsparameter
		// distance
		dist1 = 12.0;
		dist2 = 26.0;
		// deviation
		f1 = 0.91;
		f2 = 0.96;

		ShiTomasi st;
		cornerDetector = st;
		// erfasse Ecken 
		corners = cornerDetector.getCorners();
		// sortierte Punkte 
		corners = cornerDetector.sortVectorPoints(corners);
		// gruppieren der Punkte
		groupCorners(corners);
	}
	void groupCorners(vector<Point2f> points){
		// temporäre Gruppe
		vector<Point2f> tmpGroup;

		// Punktnummern
		vector<int> pn;

		int bi = 0;
		int bj = 0;

		// Gruppierung
		for (int i = 0; i < points.size(); i++){
			bi = 0;
			for (int j = 0; j < points.size(); j++){
				bj = 0;
				Point2f p = points[i] - points[j];
				if ((p.x != 0) && (p.y != 0)){
					double euk = sqrt(pow(p.x,2) + pow(p.y,2));
					if ((euk >= dist1) && (euk <= dist2)){
						for (int k = 0; k < tmpGroup.size(); k++){
							if (tmpGroup[k] == points[i]){
								bi = 1;
							}
							if (tmpGroup[k] == points[j]){
								bj = 1;
							}
						}
						if (bi != 1){
							tmpGroup.push_back(points[i]);
							pn.push_back(i);
						}
						if (bj != 1){
							tmpGroup.push_back(points[j]);
							pn.push_back(j);
						}
					}
				}
			}
			if (tmpGroup.size() == 4){
				group.push_back(tmpGroup);
				for (int z = 0; z < pn.size(); z++){
					corners.erase(corners.begin()+pn[z]);
				}
				tmpGroup.clear();
				groupCorners(corners);
				break;
			}
		}
	}
};
