// INCLUDES
#include "TEckenerkennung.h"
#include <stdlib.h>
#include <sstream>


// NAMESPACES
using namespace cv;
using namespace std;

class ObjDetection{
private:
	vector<vector<Point2f>> group;
	ShiTomasi cornerDetector;
	vector<Point2f> corners;
public:
	ObjDetection(){
	}
	void startDetection(){
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
	void importPoints(char* filename){
		// lösche alte Eckdaten
		corners.clear();
		
		vector <vector <string> > data;
		ifstream infile( filename );

		while (infile)
		{
			string s;
			if (!getline( infile, s )) break;
			istringstream ss( s );
			vector <string> record;
			while (ss)
			{
				string s;
				if (!getline( ss, s, ';' )) break;
				record.push_back( s );
			}

			data.push_back( record );
		}
		if (!infile.eof())
		{
			cerr << "Fooey!\n";
		}

		for (int i = 0; i < data.size(); i++){
			Point2f point;
			vector<string> vec = data[i];
			point.x = atof(vec[0].c_str());
			point.y = atof(vec[1].c_str());
			corners.push_back(point);
		}
	}
	void showCorners(){
		cornerDetector.showImgCircles(corners);
	}
};
