#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

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
	MatND getHueHistogram(const Mat &image, int minSaturation = 0){
		MatND hist;

		// Konvertierung zu HSV Farbbereich
		Mat hsv;
		cvtColor(image, hsv, CV_BGR2HSV);
		// benutzte Maske (oder auch nicht)
		Mat mask;

		if (minSaturation> 0){
			// Spalten der drei Kanäle in drei Bilder
			vector<Mat> v;
			split(hsv,v);
			cv::threshold(v[1],mask,minSaturation,255, THRESH_BINARY);
		}

		// Vorbereitung auf ein 1D Hue Histogram
		hranges[0] = 0.0;
		hranges[1] = 180.0;
		channels[0] = 0; // der Hue Kanal

		// Berechne Histogram
		calcHist(&hsv, 1,
			channels,
			mask,
			hist,
			1,
			histSize,
			ranges);
		return hist;
	}
	Mat colorReduce(const Mat& image, int div = 64){
		Mat result = image;
		
		//int nl = image.row;
		//int nc = image.cols * image.channels();

		//for (int j = 0; j < nl; j++){

			//uchar* data = image.ptr<uchar>(j);
			/*for (int i = 0; i < nc; i++){
				data[i] = data[i]/div*div + div/2;
			}*/
		//}
		return result;
	}
};

class ContentFinder{
private:
	float hranges[2];
	const float* ranges[3];
	int channels[3];
	float threshold;
	MatND histogram;
public:
	ContentFinder() : threshold(-1.0f){
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;
	}
	void setThreshold(float t){
		threshold = t;
	}
	float getThreshold(){
		return threshold;
	}
	void setHistogram(const MatND& h){
		histogram = h;
		normalize(histogram, histogram, 1.0);
	}
	Mat find(const Mat& image, float minValue, float maxValue, int *channels, int dim){
		Mat result;

		hranges[0] = minValue;
		hranges[1] = maxValue;

		for (int i = 0; i < dim; i++){
			this->channels[i] = channels[i];
		}

		calcBackProject(&image, 1,
			channels,
			histogram,
			result,
			ranges,
			255.0);
		
		if (threshold>0.0)
			cv::threshold(result, result, 255*threshold, 255, THRESH_BINARY);
		return result;
	}
};

class ImageComparator{
private:
	Mat reference;
	Mat input;
	MatND refH;
	MatND inputH;
	ColorHistogram hist;
	int div;
public:
	ImageComparator() : div(32){
	}
	void setColorReduction(int factor){
		div = factor;
	}
	int getColorReduction(){
		return div;
	}
	void setReferenceImage(const Mat& image){
		reference = hist.colorReduce(image, div);
		refH = hist.getHistogram(image);
	}
	double compare (const Mat& image){
		input = hist.colorReduce(image, div);
		inputH = hist.getHistogram(input);
		return compareHist(refH, inputH, CV_COMP_INTERSECT);
	}
};