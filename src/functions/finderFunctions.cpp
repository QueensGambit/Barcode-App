/*
 * finderFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "finderFunctions.h"

vector<ContourObject> find_moments(Mat gray, int thresh, Mat skel, Size size) {

	RNG rng(12345);
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	//						  50
	Canny(gray, canny_output, 50, 150, 3);

	/// Find contours
	//findContours(canny_output, contours, hierarchy, CV_RETR_TREE,
			//CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	cout << "contours.size()" << contours.size() << endl;

	// Get the Area of the contours
	/*for (int z = 0; z < contours[0].size(); z++) {
		cout << "p[" << z << "].x: " << contours[0][z].x << endl;
		cout << "p[" << z << "].y: " << contours[0][z].y << endl << endl;
	}*/

	vector<ContourObject> vecCO;
	//vector<ContourObject> vecCO(contours.size());
	/// Get the moments
	vector<Moments> mu(contours.size());
	///  Get the mass centers:
	vector<Point2f> mc(mu.size());
	//vector<Moments> mu;
	for (int i = 0; i < contours.size(); i++) {
		//cout << "area" << area << endl;
		//cout << "contour[" << i << "].size()" << vecCO[i].getContour().size() << endl;
		if (contourArea(contours[i]) > thresh) {
			//if (vecCO[i].getContour().size() > thresh) {
			//mu.push_back(moments( vecCO[i].getContour(), false));
			//if (arcLength(vecCO[i].getContour(), false) > thresh) {
			//int cont_size = vecCO[i].getContour().size();
			//if (chess_length(vecCO[i].getContour()[0], vecCO[i].getContour()[cont_size-1]) > thresh) {
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

				Matx33f mask(0, 255, 0,
				          255, 255, 0,
				          0, 255, 0);

				Matx33f maskResult;
				//bitwise_and(skel, mask, maskResult);

//				for (int x = tempMassCenter.x-maxDst; x <= tempMassCenter.x + maxDst && x <= skel.rows; x++) {
//					for (int y = tempMassCenter.y-maxDst; y <= tempMassCenter.y + maxDst && y <= skel.cols; y++) {

						//if (skel.at<uchar>(Point(tempMassCenter.x+dstX % skel.rows, tempMassCenter.y+dstY % skel.cols)) == 255) {
						//if (skel.at<uchar>(Point(x , y )) == 255) {
//						if (skel.at<uchar>( normalize(Point(x, y), skel) ) == 255) {
						if (wPxl_in_Area(tempMassCenter, skel, 2)) {
							//int tempSize = vecCO[i].getContour().size();
							ContourObject temp(tempMassCenter, contours[i]);
							vecCO.push_back(temp);
//							break;
							//vecCO[i] = temp;
						}
					}
				}

//			}
//		}

		//mu[i] = moments( vecCO[i].getContour(), false );
	}

	//Mat bw;
	//adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
	//int horizontalsize = bw.cols / 30;
	//Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(2,2));
	// Apply morphology operations
	//erode(bw, bw, horizontalStructure, Point(-1, -1),2);

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		//int val = 127; //rng.uniform(0, 255);
		//Scalar colorGray = Scalar( val, val, val);
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
		//line(drawing, vecCO[i].getFirstPoint(), vecCO[i].getLastPoint());
		//circle( bw, mc[i], 4, color, -1, 8, 0 );
		//circle( skel2, mc[i], 4, color, -1, 8, 0 );
		//circle( skel3, mc[i], 4, color, -1, 8, 0 );
	}

	//resize(bw, bw, blank.size());
//	resize(drawing, drawing, blank.size());
	resize(canny_output, canny_output, size);

	//namedWindow("Black_White", CV_WINDOW_AUTOSIZE);
	//imshow("Black_White", bw);
	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	namedWindow("Canny", CV_WINDOW_AUTOSIZE);
	imshow("Canny", canny_output);

	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	//printf("\t Info: Area and Contour Length \n");
	//for( int i = 0; i< contours.size(); i++ )
	//printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(vecCO[i].getContour()), arcLength( vecCO[i].getContour(), true ) );
	//return mc;
	return vecCO;
}

vector<ContourObject> find_mser(Mat gray) {
	RNG rng(12345);
    MSER ms;

    ms.set("delta", 3); //default 5
    ms.set("minArea", 30); //default 60
    ms.set("maxArea", 3500 ); //default 14400
    ms.set("minMargin", 0.001); //.003
    ms.set("edgeBlurSize", 1); //5
    ms.set("minDiversity", .9); //0.2
    ms.set("maxVariation", .3); //0.25

    vector<vector<Point> > regions;

    ms(gray, regions, Mat());
	vector<ContourObject> vecCO(regions.size());
//	vector<Moments> mu(contours.size());
    Mat mser = Mat::zeros(gray.size(), CV_8UC3);

    for (int i = 0; i < regions.size(); i++) {
	ContourObject temp(regions[i]);
    Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
    drawContours(mser,regions,i,color,1,8, vector<Vec4i>(), 0, Point());
	vecCO[i] = temp;
    }

    namedWindow("mser", WINDOW_AUTOSIZE);
    imshow("mser", mser);


    return vecCO;

}
