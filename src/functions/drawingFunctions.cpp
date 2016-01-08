/*
 * drawingFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "drawingFunctions.h"

RNG rng(12345);
Mat draw_massCenter(vector<ContourObject> vecCO, Size size){
	Mat m = Mat::zeros(size, CV_8UC1);
	vector<Point2f> massPoints;

	for(int i = 0; i<vecCO.size(); i++){
		m.at<uchar>(vecCO[i].getMassCenter())=255;
		massPoints.push_back(vecCO[i].getMassCenter());

	}

//	draw_probabilistic_hough(m,vecCO);
//	draw_fit_line(massPoints, m);

//	namedWindow("malen", CV_WINDOW_AUTOSIZE);
//	imshow("malen", m);

	return m;
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

void draw_hough_lines(Mat m, vector<Vec4i> p_lines) {

	Mat hough(m);
	cvtColor(m, hough, COLOR_GRAY2BGR);

	RNG rng(12345);
//	RNG rng(12335);

	for (size_t i = 0; i < p_lines.size(); i++) {
//			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),rng.uniform(0, 255));
			Scalar color = Scalar(rng.uniform(0, 2) * 255, rng.uniform(0, 2)* 255,rng.uniform(0, 2) * 255);

//			cout << "length in draw:" << norm(p_lines[i]) << endl;
			Vec4i l = p_lines[i];
//			Vec4i l = Vec4i(p_lines[i]);
	//		Vec4i l = p_lines[0];
//			cout << "l: " << l << endl;
//			cout << "l: " << l[0] << ", " << l[1] << ", " << l[2] << ", " << l[3] << endl;
//			cout << "length in draw:" << norm(Point(p_lines[i][0], p_lines[i][1])-Point(p_lines[i][2], p_lines[i][3])) << endl;
//			cout << "length in draw:" << norm(l) << endl;
			line(hough, Point(l[0], l[1]), Point(l[2], l[3]), color, 1);
//			line(hough, Point(0, 0), Point(l[0], l[1]), color, 1);

		}

		namedWindow("hough Window", WINDOW_AUTOSIZE);
		imshow("hough Window", hough); //hough
}
