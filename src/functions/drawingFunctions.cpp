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

	/*line( m,
					Point(0,0),
					Point(m.rows,m.cols),
			        Scalar(255,0,0),
			        5,
			        8 );*/
	cout << "mc.size() lines = " << vecCO.size() << endl;
	for (int i = 0; i < vecCO.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));

		vecCO[i].drawContourOnMat(m);
		//cout << "anzahl linen gemalt: " << i << endl;
		//line(m, vecCO[i].getFirstPoint(), vecCO[i].getLastPoint(), color, -1, 8,
		//		0);
		/*cout << "FirstP: " << vecCO[i].getFirstPoint() << endl;
		cout << "LastP: " << vecCO[i].getLastPoint() << endl;
		for (int z = 0; z < vecCO[z].getContour().size(); z++ ) {
			line( m,
					vecCO[i].getFirstPoint(),
					vecCO[i].getContour()[z],
					Scalar(255,0,0),
			        5,
			        8 );
		}
		line( m,
				vecCO[i].getFirstPoint(),
				vecCO[i].getLastPoint(),
				Scalar(255,0,0),
		        5,
		        8 );*/
	}
}

void draw_minRectangles (vector<ContourObject> vecCO, Mat m) {
	  /// Find the rotated rectangles and ellipses for each contour
//	  vector<RotatedRect> minRect( vecCO.size() );
	  //vector<RotatedRect> minEllipse( vecCO.size() );

//	  for( int i = 0; i < vecCO.size(); i++ ) {
//		  minRect[i] = minAreaRect( Mat(vecCO[i].getContour()) );
//	       if( vecCO[i].getContour().size() > 5 ) {
//	    	   //minEllipse[i] = fitEllipse( Mat(vecCO[i].getContour()) ); }
//	       }
//	     }

	  /// Draw contours + rotated rects + ellipses
	  //Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	  for( int i = 0; i< vecCO.size(); i++ )
	     {
	       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	       // contour
	       //drawContours( m, vecCO[i].getContour(), i, color, 1, 8, vector<Vec4i>(), 0, Point() );
	       // ellipse
	       //ellipse( m, minEllipse[i], color, 2, 8 );
	       // rotated rectangle
	       Point2f rect_points[4];
	       vecCO[i].getRectPoints(rect_points);
//	       minRect[i].points( rect_points );
	       Point intRect_points[4];
	       for( int j = 0; j < 4; j++ ) {
	    	  intRect_points[j] = rect_points[j];
	          //line( m, rect_points[j], rect_points[(j+1)%4], color, 2, 8 );
	       }
//	       rectangle( m, minRect[i].boundingRect(), color, 8, 0);
//	       fillConvexPoly( m, intRect_points, 4, color, 8, 0);
	       fillConvexPoly( m, intRect_points, 4, Scalar(255,255,255), 8, 0);

	     }
}
