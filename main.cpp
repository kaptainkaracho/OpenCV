#include "TOpenCV.h"

int main( int argc, char** argv )
{
	OpenCVObj objcv;
	objcv.loadImageJPG("D:\\Objekterkennung\\test2.tiff");
	Mat src = objcv.getImgMat();
	objcv.trackContours(src);
}

///** @function thresh_callback */
//void thresh_callback(int, void* )
//{
//  Mat canny_output;
//  vector<vector<Point> > contours;
//  vector<Vec4i> hierarchy;
//
//  /// Detect edges using canny
//  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
//  /// Find contours
//  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
//
//  /// Draw contours
//  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
//  for( int i = 0; i< contours.size(); i++ )
//     {
//       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//     }
//
//  /// Show in a window
//  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//  imshow( "Contours", drawing );
//}