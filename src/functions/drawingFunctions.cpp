/*
 * drawingFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "drawingFunctions.h"

RNG rng(12345);

void draw_Circles(vector<Point2f> mc, Mat m) {

	cout << "mc.size() = " << mc.size() << endl;

	for (int i = 0; i < mc.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		circle(m, mc[i], 4, color, -1, 8, 0);
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
