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
	vector<Point2f> massPoints;

	for(int i = 0; i<vecCO.size(); i++){
		m.at<uchar>(vecCO[i].getMassCenter())=255;
		massPoints.push_back(vecCO[i].getMassCenter());

	}

	draw_probabilistic_hough(m,vecCO);
//	draw_fit_line(massPoints, m);

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

void draw_probabilistic_hough(Mat m,vector<ContourObject> vecCO) {
	vector<Vec4i> p_lines;
	Mat hough(m);
	Mat dst;
//	blur(m, m, Size(3, 3));
//	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
//	dilate( m, m, element );
	 Canny(m, dst, 50, 200, 3);
	cvtColor(m, hough, COLOR_GRAY2BGR);
//	cvtColor(m, gray, COLOR_BGR2GRAY);

//	Mat bw = m > 128;

	/// 2. Use Probabilistic Hough Transform //30	//80
	HoughLinesP(dst, p_lines, 1, CV_PI / 180, 30, 30, 80);
	RNG rng(12345);
	/// Show the result
	cout << "p_line.size(): " <<  p_lines.size() << endl;
//	filter_hough_lines(p_lines, 0.00002, m.cols*m.rows);
	p_lines = filter_hough_lines2(p_lines, vecCO);
	cout << "p_line.size(): " <<  p_lines.size() << endl;

	for (size_t i = 0; i < p_lines.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
						rng.uniform(0, 255));
		Vec4i l = p_lines[i];
//		Vec4i l = p_lines[0];
		line(hough, Point(l[0], l[1]), Point(l[2], l[3]), color, 1);

	}

	namedWindow("hough Window", WINDOW_AUTOSIZE);
	imshow("hough Window", hough); //hough
}

void draw_fit_line(vector<Point2f> points, Mat m) {

//	Mat mLines;
//	cvtColor(m, mLines, COLOR_GRAY2BGR);
	vector<Vec4i> lines;
    fitLine(points,lines,CV_DIST_L1,0,0.01,0.01);
	/*for (size_t i = 0; i < lines.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
						rng.uniform(0, 255));
		Vec4i l = lines[i];
//		line(mLines, Point(l[0], l[1]), Point(l[2], l[3]), color, 1);

	}*/

	namedWindow("fitLines Window", WINDOW_AUTOSIZE);
//	imshow("fitLines Window", mLines); //hough
}
