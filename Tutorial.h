#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;


class ContentFinder{
private:
	float hranges[2];
	const float* ranges[3];
	int channels[3];
	float threshold;
	
public:

};

class Histogram1D{
private:
	int histSize[1]; // Anzahl von Kästen
	float hranges[2]; // min und max Pixelwerte
	const float* ranges[1]; 
	int channels[1]; // nur 1 Kanal wird hier benutzt
public:
	Histogram1D(){
		histSize[0] = 256;
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		ranges[0] = hranges;
		channels[0] = 0; // default value 
	}
	MatND getHistogram(const Mat &image){
		MatND hist;
		// berechnen des Histograms
		calcHist(&image,
			1,	// Histogram nur vom ersten Bild
			channels,	// benutzter Kanal
			Mat(),	// keine Maske wird benutzt
			hist,	// resultierendes Histogram
			1,	// 1D Histogram
			histSize,	// Anzahl der Kästen
			ranges);	// Weite der Pixelwerte
		return hist;
	}
	Mat getHistogramImage(const Mat &image){
		// Berechnen des Histograms
		MatND hist = getHistogram(image);

		// Min.- und Maxwerte bestimmen
		double maxVal = 0;
		double minVal = 0;
		minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		// Abbildung für das Histogram
		Mat histImg(histSize[0], histSize[0], CV_8U, Scalar(255));

		// setze höchsten Punkt auf 90% von nbins
		int hpt = static_cast<int>(0.9*histSize[0]);

		// zeichnen der vertikalen Linie für jeden Kasten
		for (int h = 0; h < histSize[0]; h++){
			float binVal = hist.at<float>(h);
			int intensity = static_cast<int>(binVal*hpt/maxVal);

			line(histImg, Point(h, histSize[0]), 
				Point(h, histSize[0]-intensity),
				Scalar::all(0));
		}
		return histImg;
	}
};

class ColorHistogram{
private:
	int histSize[3]; 
	float hranges[2]; 
	const float* ranges[3]; 
	int channels[3]; 
public:
	ColorHistogram(){
		histSize[0] = histSize[1] = histSize[2] = 256;
		hranges[0] = 0.0;	// BRG Weite
		hranges[1] = 255.0;
		ranges[0] = hranges;	// alle Kanäle haben den gleichen Level
		ranges[1] = hranges;
		ranges[2] = hranges;
		channels[0] = 0;	// alle drei Kanäle
		channels[1] = 1;
		channels[2] = 2;
	}
	MatND getHistogram(const Mat &image){
		MatND hist;

		// Berechne Histogram
		calcHist(&image,
			1,	// 
			channels,
			Mat(),
			hist,
			3,	// 3D Histogram
			histSize,
			ranges
			);
		return hist;
	}
	Mat applyLookUp(const Mat &image, Mat& lookup){
		Mat result;

		LUT(image, lookup, result);
		return result;
	}
	Mat strech(const Mat &image, int minValue = 0){
		// Berechnung des Histograms
		MatND hist = getHistogram(image);

		// Linkes Extrem
		int imin = 0;
		for ( ; imin < histSize[0]; imin++){
			if (hist.at<float>(imin) > minValue)
				break;
		}
		// Rechtes Extrem
		int imax = histSize[0]-1;
		for ( ; imax >= 0; imax++){
			if (hist.at<float>(imax) > minValue)
				break;
		}

		// Erstelle Lookup Table
		int dim(256);
		Mat lookup(1,
			&dim,
			CV_8U);

		// Baue Lookup Table
		for (int i = 0; i < 256; i++){
			// Strech zwischen imin und imax
			if (i < imin) lookup.at<uchar>(i) = 0;
			else if (i > imax) lookup.at<uchar>(i) = 255;
			else lookup.at<uchar>(i) = static_cast<uchar>(
				255.0*(i-imin)/(imax-imin)+0.5);
		}
		Mat result;
		result = applyLookUp(image, lookup);
		return result;
	}
	Mat equalize(Mat &image){
		Mat result;
		equalizeHist(image, result);
		return result;
	}
};