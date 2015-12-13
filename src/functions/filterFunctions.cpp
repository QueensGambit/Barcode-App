/*
 * filterFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "filterFunctions.h"

vector<Point2f> filter_moments(vector<Point2f> mc, Mat skel) {

	int dst = 2, xDst = 0, yDst = 0;
	cout << "mc.size() = " << mc.size() << endl;
	vector<Point2f> tempMC;
	for (unsigned int i = 0; i < mc.size(); i++) {
		//weisser Punkt auf moment Zentrum
		//cout << "mc[" << i << "]: " << mc[i].x << endl;
		//for (int xDst = -dst; xDst <= dst; xDst++) {
		//for (int yDst = -dst; yDst <= dst; yDst++) {
		if (skel.at<uchar>(Point(mc[i].x + xDst, mc[i].y + yDst)) == 255) {
			tempMC.push_back(mc[i]);
		}
		//}
		//}
	}

	return tempMC;
}

vector<ContourObject> filter_by_rect(vector<ContourObject>vecCO, Mat m, float threshWPxl, float threshAspect) {

	  vector<RotatedRect> minRect( vecCO.size() );
	  vector<ContourObject> fVecCO;
	  //vector<RotatedRect> minEllipse( vecCO.size() );
	  //Mat mask(m.size(), CV_8UC1, cv::Scalar(0));
	  //Mat dst(m.size(), CV_8UC1, cv::Scalar(0));

	  for( int i = 0; i < vecCO.size(); i++ ) {
		  //out << "m.size()" << m.size() << endl;•
//		  cout << "i: " << i << endl;


		 /* Mat mask = Mat::zeros(m.size(), CV_8UC3);
		  Mat dst = Mat::zeros(m.size(), CV_8UC3);*/

	     minRect[i] = minAreaRect( Mat(vecCO[i].getContour()) );

	    /* if (i == 100) {
	 // rect is the RotatedRect (I got it from a contour...)
//			 RotatedRect rect;
			 // matrices we'll use
			 Mat M, rotated, cropped;
			 // get angle and size from the bounding box
			 float angle = minRect[i].angle;
			 Size rect_size = minRect[i].size;
			 // thanks to http://felix.abecassis.me/2011/10/opencv-rotation-deskewing/
			 if (minRect[i].angle < -45.) {
				 angle += 90.0;
				 swap(rect_size.width, rect_size.height);
			 }
			 // get the rotation matrix
			 M = getRotationMatrix2D(minRect[i].center, angle, 1.0);
			 // perform the affine transformation
			 warpAffine(m, rotated, M, m.size(), INTER_CUBIC);
			 // crop the resulting image
			 getRectSubPix(rotated, rect_size, minRect[i].center, cropped);

	    	 namedWindow("cropped", WINDOW_AUTOSIZE);
	    	 imshow("cropped", cropped);
	     }*/
	       /*Point2f rect_points[4];
	       minRect[i].points( rect_points );
	       //int iteration = 5;
	       //float fac = 0.2;
	       int hits = 0;
//	       Point intRect_points[4];
	       for( int j = 0; j < 4; j++ ) {
	    	  //intRect_points[j] = rect_points[j];
	    	   //Vektor2d direction = Vektor2d(rect_points[j], rect_points[(j+1)%4]);
	    	   //Point cur = rect_points[j];
	    		//for (int z = 0; z < iteration; z++) {
	    			//cur += fac * direction.getOrtsV();

//	    			if (m.at<uchar>(rect_points[j]) == 255) {
	    	   	    if (wPxl_in_Area(rect_points[j], m, 1)) {
	    		   		circle(m, normalize(rect_points[j], m), 2, Scalar(255,0,255), -1, 8, 0);
	    				hits++;
	    			}
	    		//}
	          //line( m, rect_points[j], rect_points[(j+1)%4], color, 2, 8 );
	       }

	       //Mat cropedImage = fullImage(Rect(X,Y,Width,Height));
	       //Mat cropedM = m(minRect[i].boundingRect());
	       //Mat mask = Mat::zeros(cropedM.size(), CV_8UC3);
	       //Mat dst = Mat::zeros(cropedM.size(), CV_8UC3);

	       //fillConvexPoly( mask, intRect_points, 4, Scalar(255, 255, 255), 8, 0);
	       //bitwise_and(m, mask, dst);

	       if (hits >= thresh) {
//	    	   cout << "hits: " << hits << endl;
	    	   vecCO[i].setRectPoints(rect_points);
	    	   fVecCO.push_back(vecCO[i]);

	       }*/

	       /*if (countNonZero(dst) >= countNonZero(mask)*thresh) {
	    	   cout << "countNonZero(mask): " << countNonZero(mask) << endl;
	    	   fVecCO.push_back(vecCO[i]);
	       }*/
	     	 float width = minRect[i].size.width;
	     	 float height = minRect[i].size.height;
	     	 float ratio;

	     	 if (width > height) {
	     		 ratio = width / height;
	     	 }
	     	 else {
	     		 ratio = height / width;
	     	 }
//	     	 cout << "ratio: " << ratio << endl;

			 if (getfilledRatio(m, minRect[i]) >= threshWPxl && ratio >= threshAspect) {
				   Point2f rect_points[4];
				   minRect[i].points( rect_points );
				   vecCO[i].setRectPoints(rect_points);
				   vecCO[i].setMassCenter(minRect[i].center);

				   fVecCO.push_back(vecCO[i]);
			 }



 		   // cout << "aspectRatio:" << minRect[i].size.width / minRect[i].size.height << endl;

	     }

      /*namedWindow("bitwise_and", 1);
      imshow("bitwise_and", dst);*/
	  return fVecCO;
}

vector<ContourObject> filter_lines(vector<ContourObject> vecCO, Mat skel, int hitThresh) {
	Point last;
	Point temp;
	int hits = 0;
	vector<ContourObject> newVecCO;

	for (int i = 0; i < vecCO.size(); i++) {
		temp = vecCO[i].getFirstPoint();
		last = vecCO[i].getLastPoint();
		Vektor2d vektor(temp, last);
		//cout << "vecCO.size(): " << vecCO.size() << endl;
		//cout << "vektor: " << vektor << endl;
		float lambda = 0.0;
		float schritt = 0.1;
		while (lambda <= 1.0) {
			//cout << "lambda" << lambda << endl;
			if (skel.at<uchar>(temp) == 255) {
				temp.x += lambda * vektor.getOrtsV().x;
				temp.y += lambda * vektor.getOrtsV().y;
				cout << "temp: " << temp << endl;
				//temp=lambda*vektor.getOrtsV();
				hits++;
				cout << "hits: " << hits << endl;
			}
			lambda += schritt;
		}

		if (hits >= hitThresh) {
			newVecCO.push_back(vecCO[i]);
		}
	}
	return newVecCO;
}

vector<Point2f> island_filter(vector<Point2f> mc, Mat skel, int thresh) {

	Point2f tL, bR;
	vector<Point2f> temp;
	for (unsigned int i = 0; i < mc.size(); i++) {
		tL = topLeftPixel(mc[i], skel);
		bR = bottomRightPixel(mc[i], skel);

		/*cout << "tL.x" << tL.x << endl;
		 cout << "tL.y" << tL.y << endl;
		 cout << "bR.x" << bR.x << endl;
		 cout << "bR.y" << bR.y << endl;*/

		if (count_whitePixel(tL, bR, skel) >= thresh) {
			temp.push_back(mc[i]);
		}
	}

	return temp;
}

vector<ContourObject> filter_by_dst(vector<ContourObject> vecCO, int pxlSum, float threshDst, Size size) {

	vector<ContourObject> fVecCO;
	Mat m = Mat::zeros(size, CV_8UC1);

	   float dist;
	   for (int i = 0; i < vecCO.size(); i++) {
		 for (int z = 0; z < vecCO.size(); z++) {
			 if (z != i) {
				 dist = norm(vecCO[z].getMassCenter() - vecCO[i].getMassCenter());
				 if (dist <= threshDst*pxlSum) { //0.00008
//								 cout << "dist: " << dist << endl;
					 line( m, vecCO[z].getMassCenter(), vecCO[i].getMassCenter(), Scalar(255,255,255), 2, 8 );
					fVecCO.push_back(vecCO[i]);
					 break;
				 }
			 }
		 }
	   }

	   namedWindow("dst", WINDOW_AUTOSIZE);
	   imshow("dst", m);

	   return fVecCO;
}

void cluster_rect(Mat b, vector<ContourObject> vecCO){
//	cvtColor(b, b, CV_BGR2);
		draw_minRectangles(vecCO, b);

		namedWindow("Rect", CV_WINDOW_AUTOSIZE);
		imshow("Rect", b);


		Mat kernel = getStructuringElement(MORPH_RECT, Size(21, 7));
		morphologyEx(b, b, MORPH_CLOSE, kernel);

					//namedWindow("closedThresh", 1);
					//imshow("closedThresh", closed);

					erode(b, b, kernel);

					//namedWindow("closedDilate", 1);
					//imshow("closedErode", closed);
					blur(b,b,Size(9,9));
					dilate(b, b, kernel);
//--------------------------------------------------------------------------------------
//					Rect bounding_rect;
//
//					for (unsigned int i = 0; i < contours.size(); i++) {
//									double a = contourArea(contours[i], false);
//									if (a > largest_area) {
//										largest_area = a;
//										largest_cnt_index = i;
//										bounding_rect = boundingRect(contours[i]);
//									}
//								}
//
//								Scalar color(255, 255, 255);
//								drawContours(saveImg, contours, largest_cnt_index, color,
//										CV_WARP_INVERSE_MAP, 8, hierarchy);
//								rectangle(saveImg, bounding_rect, Scalar(0, 255, 0), 2, 8, 0);
		namedWindow("RectDilate", CV_WINDOW_AUTOSIZE);
		imshow("RectDilate", b);
/*
	    Mat dist;
	    distanceTransform(b, dist, CV_DIST_L2, 3);
		namedWindow("non-normalized", CV_WINDOW_AUTOSIZE);
	    imshow("non-normalized", dist);

	    normalize(dist, dist, 0.0, 1.0, NORM_MINMAX);
		namedWindow("normalized", CV_WINDOW_AUTOSIZE);
	    imshow("normalized", dist);*/
}
