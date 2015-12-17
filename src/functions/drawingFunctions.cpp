/*
 * drawingFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "drawingFunctions.h"

RNG rng(12345);
void draw_massCenter(vector<ContourObject> vecCO, Size size){
	Mat m = Mat::zeros(size, CV_8UC1);
	for(int i = 0; i<vecCO.size(); i++){
		m.at<uchar>(vecCO[i].getMassCenter())=255;
	}
	draw_probabilistic_hough(m);
	namedWindow("malen", CV_WINDOW_AUTOSIZE);
	imshow("malen", m);
}
void draw_Circles(vector<Point2f> mc, Mat m) {

	cout << "mc.size() = " << mc.size() << endl;

	for (int i = 0; i < mc.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		circle(m, mc[i], 0, color, -1, 8, 0);
	}
}

void draw_Circles(vector<ContourObject> vecCO, Mat m) {

	cout << "vecCO.size() = " << vecCO.size() << endl;
	for (int i = 0; i < vecCO.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));

		circle(m, vecCO[i].getMassCenter(), 4, color, -1, 8, 0);
	}
}

void draw_Lines(vector<ContourObject> vecCO, Mat m) {

	cout << "mc.size() lines = " << vecCO.size() << endl;
	for (int i = 0; i < vecCO.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));

		vecCO[i].drawContourOnMat(m);

	}
}

void draw_minRectangles (vector<ContourObject> vecCO, Mat m) {

	  for( int i = 0; i< vecCO.size(); i++ )
	     {
	       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

	       Point2f rect_points[4];
	       vecCO[i].getRectPoints(rect_points);

	       Point intRect_points[4];
	       for( int j = 0; j < 4; j++ ) {
	    	  intRect_points[j] = rect_points[j];
	       }

	       fillConvexPoly( m, intRect_points, 4, Scalar(255,255,255), 8, 0);

	     }
}

void draw_probabilistic_hough(Mat m) {
	vector<Vec4i> p_lines;
	Mat hough(m);
	Mat dst;
	 Canny(m, dst, 50, 200, 3);
	cvtColor(m, hough, COLOR_GRAY2BGR);
//	cvtColor(m, m, COLOR_BGR2GRAY);

	/// 2. Use Probabilistic Hough Transform
	HoughLinesP(dst, p_lines, 1, CV_PI / 180, 30, 30, 80);
	RNG rng(12345);
	/// Show the result
	cout << "p_line.size(): " <<  p_lines.size() << endl;
//	filter_hough_lines(p_lines, 0.00002, m.cols*m.rows);
	cout << "p_line.size(): " <<  p_lines.size() << endl;

	for (size_t i = 0; i < p_lines.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
						rng.uniform(0, 255));
		Vec4i l = p_lines[i];
//		Vec4i l = p_lines[0];
		line(hough, Point(l[0], l[1]), Point(l[2], l[3]), color, 1);

	}

	namedWindow("hough Window", WINDOW_AUTOSIZE);
	imshow("hough Window", hough);
}
