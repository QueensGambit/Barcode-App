/*
 * finderFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "finderFunctions.h"

/*
 * Lines of Barcodes have to be grouped now
 */

void find_Colour_for_Groups(map<Scalar, vector< vector< ContourObject> > >& scalarMap,Scalar scalar, ContourObject vecCO) {

	map<Scalar, vector<vector<ContourObject> > >::iterator it;
	vector<ContourObject> temp;

	for (it = scalarMap.begin(); it != scalarMap.end(); it++) {
		if (it == scalar) {
			//get group of line
			temp = scalarMap[scalar];
			// extend the group with the contour
			temp.push_back(vecCO);
			//push it back
			scalarMap[scalar].push_back(temp);
		} else {
			vector<vector<ContourObject> > group;
			scalarMap.insert(pair<Scalar, vector<ContourObject>> (scalar, group));

		}
	}
//	if (scalarMap.find(scalar)) {
//		scalarMap[scalar].push_back(vecCO);
//	} else {
//		vector<ContourObject> group;
//		scalarMap.insert(pair<Scalar, vector<ContourObject>> (scalar, group));
//
//	}
}
void find_groups(Mat m, vector<ContourObject> vecCO) {
	vector<vector<ContourObject> > group;
	for (int i = 0; i < vecCO.size(); i++) {
		Scalar scalar = m.at<uchar>(vecCO[i].getMassCenter());
		if ((m.at<uchar>(vecCO[i].getMassCenter())) != 0) {
			find_Colour_for_Groups(scalar, vecCO[i]);
		}
	}
	return group;
}


vector<ContourObject> find_moments(Mat gray, int thresh, Mat skel, Size size) {

	RNG rng(12345);
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	//						  50
	Canny(gray, canny_output, 50, 150, 3);

	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	cout << "contours.size()" << contours.size() << endl;

	// Get the Area of the contours
	vector<ContourObject> vecCO;

	/// Get the moments
	vector<Moments> mu(contours.size());
	///  Get the mass centers:
	vector<Point2f> mc(mu.size());

	for (int i = 0; i < contours.size(); i++) {

		if (contourArea(contours[i]) > thresh) {

			mu[i] = moments(contours[i], false);
			//calc the mass centers
			float x = mu[i].m10 / mu[i].m00;
			float y = mu[i].m01 / mu[i].m00;
			//x != x return true, if x is NaN
			if (!(x != x) && !(y != y)) {
				Point2f tempMassCenter = Point2f(mu[i].m10 / mu[i].m00,
						mu[i].m01 / mu[i].m00);
				//mc.push_back(Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ));

				int maxDst = 1;

				Matx33f mask(0, 255, 0, 255, 255, 0, 0, 255, 0);

				Matx33f maskResult;

				if (wPxl_in_Area(tempMassCenter, skel, 2)) {
					ContourObject temp(tempMassCenter, contours[i]);
					vecCO.push_back(temp);

				}
			}
		}
	}

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));

		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);

	}

	resize(canny_output, canny_output, size);

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	namedWindow("Canny", CV_WINDOW_AUTOSIZE);
	imshow("Canny", canny_output);

	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	//printf("\t Info: Area and Contour Length \n");
	//for( int i = 0; i< contours.size(); i++ )
	//printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(vecCO[i].getContour()), arcLength( vecCO[i].getContour(), true ) );
	return vecCO;
}

vector<ContourObject> find_mser(Mat gray) {
	RNG rng(12345);
	MSER ms;

	ms.set("delta", 3); //default 5
	ms.set("minArea", 30); //default 60
	ms.set("maxArea", 3500); //default 14400
	ms.set("minMargin", 0.001); //.003
	ms.set("edgeBlurSize", 1); //5
	ms.set("minDiversity", .9); //0.2
	ms.set("maxVariation", .3); //0.25g

	vector<vector<Point> > regions;

	ms(gray, regions, Mat());
	vector<ContourObject> vecCO(regions.size());

	Mat mser = Mat::zeros(gray.size(), CV_8UC3);

	for (int i = 0; i < regions.size(); i++) {
		ContourObject temp(regions[i]);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		drawContours(mser, regions, i, color, 1, 8, vector<Vec4i>(), 0,
				Point());
		vecCO[i] = temp;
	}

	namedWindow("mser", WINDOW_AUTOSIZE);
	imshow("mser", mser);

	return vecCO;

}
