#include "TObjekterkennung.h"


int maxCorners;
double qualityLevel;
double minDistance;
int blockSize;
bool useHarrisDetector = false;
double k;
string exportFile;
string exportPicFile;
string inputPicture;
int circle_radius;

char* inifile = "D:\\Objekterkennung\\Objekterkennung.ini";

// lesen der Ini-Datei
void initialize(char* filename){
	ifstream infile(filename);
	vector <vector <string> > data;

	while (infile)
	{
		string s;
		if (!getline( infile, s )) break;
		istringstream ss( s );
		vector <string> record;
		while (ss)
		{
			string s;
			if (!getline( ss, s, '=' )) break;
			record.push_back( s );
		}

		data.push_back( record );
	}
	if (!infile.eof())
	{
		cerr << "Fooey!\n";
	}
	infile.close();
	
	for (int i = 0; i < data.size(); i++){
		vector<string> vec = data[i];
		if (vec[0] == "MAXCORNERS"){
			maxCorners = atoi(vec[1].c_str());
		} else if (vec[0] == "QUALITYLEVEL"){
			qualityLevel = atof(vec[1].c_str());
		} else if (vec[0] == "MINDISTANCE"){
			minDistance = atoi(vec[1].c_str());
		} else if (vec[0] == "BLOCKSIZE"){
			blockSize = atoi(vec[1].c_str());
		} else if (vec[0] == "K"){
			k = atof(vec[1].c_str());
		} else if (vec[0] == "INPUT_CORNERS"){
			exportFile = vec[1];
		} else if (vec[0] == "PICTUREOUTPUT"){
			exportPicFile = vec[1];
		} else if (vec[0] == "PICTUREINPUT"){
			inputPicture = vec[1];
		} else if (vec[0] == "CIRCLE_RADIUS"){
			circle_radius = atoi(vec[1].c_str());
		}
	}
}


vector<Point2f> sortVectorPoints(vector<Point2f> vec){
		std::sort(vec.begin(), vec.end(), [](const cv::Point2f &a, const cv::Point2f &b) {
		return a.x*a.x + a.y*a.y < b.x*b.x + b.y*b.y;});
		return vec;
	}

int main (int argc, char** argv){
	// Initialisierung
	if (argc == 1){
		initialize(inifile);
	} else if (argc > 1){
		initialize(argv[1]);
	}

	// Objekterkennung - Shi Tomasi
	Mat src = imread(inputPicture.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	vector<Point2f> corners;

	goodFeaturesToTrack(src, corners, maxCorners, qualityLevel, 
			minDistance, Mat(), blockSize, useHarrisDetector, k);
	cout << "Erkannte Ecken :" << corners.size() << endl;

	// sortieren der Eckpunkte nach der Entfernung zum Ursprung
	vector<Point2f> sortCorners = sortVectorPoints(corners);

	
	// exportieren der Eckpunkte
	ofstream exportfile(exportFile, ios::trunc);
	if (exportfile.is_open()){
		for (int i = 0; i < sortCorners.size(); i++){
			string line = to_string(sortCorners[i].x) + ";" + to_string(sortCorners[i].y);
			exportfile << line << endl;
		}
		exportfile.close();
	}

	
	// exportieren eines Bildes mit erkannten Ecken
	Mat copy;
	copy = src.clone();

	for( int i = 0; i < sortCorners.size(); i++ )
		{ circle( copy, sortCorners[i], circle_radius, Scalar(0),
					2, 8, 0 ); }
	imwrite(exportPicFile, copy);
}

