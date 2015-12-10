#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "ContourObject.h"
#include "Vektor2d.h"

#include "helperFunctions.h"

using namespace cv;
using namespace std;

RNG rng(12345);

void crop(Mat, Mat);

vector<ContourObject> find_moments(Mat, int, Mat);
vector<ContourObject> find_mser(Mat);
void bw_thresh_callback(int, void*);
void make_skelekton(Mat);
void probabilistic_hough();
vector<Point2f> filter_moments(vector<Point2f>, Mat);
void draw_Circles(vector<Point2f>, Mat);
void draw_Circles(vector<ContourObject>, Mat);
void draw_Lines(vector<ContourObject>, Mat);



vector<ContourObject> filter_lines(vector<ContourObject>, Mat, int);
vector<Point2f> island_filter(vector<Point2f>, Mat, int);
void draw_minRectangles (vector<ContourObject> vecCO, Mat m);
vector<ContourObject> filter_by_rect(vector<ContourObject>, Mat, float, float);
vector<ContourObject> filter_by_dst(vector<ContourObject>, int, float);
void cluster_rect(Mat, vector<ContourObject>);
//global variables
Mat skel, skel2, skel3, gray, blank, hough;
int thresh = 127;
int max_thresh = 255;

char* skel_window = "Skel Window";

//gute Internetquellen:
//http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/moments/moments.html

int main() {
	//Test Comment
	/// Load source image, convert it to gray and blur it
	//Mat blank;
	Mat src;	//, gray;
	//src = imread("C:/Users/Suhay/Dropbox/BarCode/bilder/gut/2015-10-07 13.27.19.jpg", CV_LOAD_IMAGE_COLOR);
	//src = imread("G:/Gimp/DBV/internet/Chips_rotated.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("G:/Gimp/DBV/internet/Gefro Pesto Verde (2).jpg", CV_LOAD_IMAGE_COLOR);
	//src = imread("G:/Gimp/DBV/sample.png", CV_LOAD_IMAGE_COLOR);
	//src = imread("G:/Gimp/DBV/barcode_object3.png", CV_LOAD_IMAGE_COLOR);
//	src = imread("G:/Gimp/DBV/bilder/kritisch/2015-10-07 13.18.46.jpg", CV_LOAD_IMAGE_COLOR);
//		src = imread("G:/Gimp/DBV/bilder/gut/20151007_133654.jpg", CV_LOAD_IMAGE_COLOR);
	//blank = imread("C:/Users/Suhay/Dropbox/BarCode/bilder/gut/blank.jpg", CV_LOAD_IMAGE_COLOR);

	src = imread("media/internet/Chips_rotated.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/chips.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/gut/mandarine.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Gefro Pesto Verde (2).jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Knorr fix Bolognese (3).JPG", CV_LOAD_IMAGE_COLOR);

	//blank = imread("G:/Gimp/DBV/blank2.jpg", CV_LOAD_IMAGE_COLOR);
	//blank = imread("media/blank2.jpg", CV_LOAD_IMAGE_COLOR);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	get source from camera by video capture
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
		VideoCapture capture(0); 								// open default camera

		if (!capture.isOpened()) {
			cout << "ERROR: Could not open Camera";
			return -1;
		}

		namedWindow("Camera", CV_WINDOW_AUTOSIZE);

		while (true) {

			Mat frame;
			capture >> frame; 									// get frames
			imshow("Camera", frame);

			if (waitKey(30) == 13) {// press enter to break loop and copy frame into source image
				frame.copyTo(src);
				destroyAllWindows();
				break;
			}
		}



//		blank = imread("C:/Users/Björn/Documents/Programme/eclipse c++/media/blank.jpg", CV_LOAD_IMAGE_COLOR);			//new blank image
*/
	blank = imread("media/blank2.jpg", CV_LOAD_IMAGE_COLOR);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	end of capturing
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	int pxl_Sum = src.cols * src.rows;

	cvtColor(src, gray, CV_BGR2GRAY);
//	blur(gray, gray, Size(3, 3));

	//invert the image !
	gray = ~gray;

//	resize(src, src, blank.size());
	namedWindow("Source", CV_WINDOW_AUTOSIZE);
	imshow("Source", src);

	namedWindow(skel_window, CV_WINDOW_AUTOSIZE);
	//createTrackbar("Threshold: ", "Source", &thresh, max_thresh, bw_thresh_callback);
	//15, -2
//	adaptiveThreshold(gray, skel2, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -8);
	//make_skelekton(skel2);

	adaptiveThreshold(gray, skel3, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -8);

	gray += skel3;
	namedWindow( "Gray", CV_WINDOW_AUTOSIZE );
	imshow( "Gray", gray );

	//make_skelekton(skel3);
	//cout << "pxl_Sum * 0.0005 = " << pxl_Sum * 0.00005 << endl;
	//vector<Point2f> mc (find_moments( gray , pxl_Sum * 0.00005));
//	vector<ContourObject> vecCO(find_moments(gray, pxl_Sum * 0.00004, skel3));
	vector<ContourObject> vecCO(find_mser(gray));
	//vector<ContourObject> vecCO;
	//vector<ContourObject> vecCO(find_moments(skel3, pxl_Sum * 0.00001, skel3));

	//vector<Point2f> mc_filtered (filter_moments ( mc, skel3 ));

	//vector<Point2f> mc_island_filtered (island_filter(mc_filtered, skel3, 30));
	//cout << "mc.size() = " << mc.size() << endl;
	//cout << "mc_filtered.size() = " << mc_filtered.size() << endl;
	cout << "vecCO.size() = " << vecCO.size() << endl;

	/*for (unsigned int i = 0; i < vecCO.size(); i++) {
		cout << vecCO[i] << endl;
	}*/
//	vector<ContourObject> vecCOfiltered(filter_lines(vecCO, skel3, 5));
	//cout << "vecCOfiltered.size(): " << vecCOfiltered.size() << endl;
	//cout << "mc_island_filtered.size() = " << mc_island_filtered.size() << endl;

	Mat mfiltered; // = Mat::zeros( skel2.size(), CV_8UC3 );
	skel3.copyTo(mfiltered);
	cvtColor(mfiltered, mfiltered, CV_GRAY2BGR);

//	cvtColor(skel3, skel3, CV_GRAY2BGR);

	vector<ContourObject> fVecCO(filter_by_rect(vecCO, skel3, .9, 7 )); //.95, 7
	vector<ContourObject> fVecCO2(filter_by_dst(fVecCO, pxl_Sum, 0.00003 )); //0.00001

	cout << "fVecCO.size(): " << fVecCO.size() << endl;
	cout << "fVecCO2.size(): " << fVecCO2.size() << endl;

	//draw_Lines(vecCOfiltered, mfiltered);
	//draw_Circles(mc_island_filtered, mfiltered);
	//draw_Circles(mc_filtered, mfiltered);
	//draw_Circles(mc, mfiltered);
//	draw_Circles(vecCO, mfiltered);
//	draw_Circles(fVecCO, mfiltered);
	//draw_Lines(vecCOfiltered, mfiltered);
	//draw_Lines(vecCO, mfiltered);
//	draw_Lines(fVecCO, mfiltered);

	//Mat mRotRect = Mat::zeros(mfiltered.size(), CV_8UC3);
//	draw_minRectangles(vecCO, mfiltered);
	draw_minRectangles(fVecCO2, mfiltered);
	//bitwise_and(mfiltered, mRotRect, mfiltered);

	//resize(skel2, skel2, blank.size());
	//resize(skel3, skel3, blank.size());
//	resize(mfiltered, mfiltered, blank.size());

	namedWindow("mFiltered", CV_WINDOW_AUTOSIZE);	//CV_WINDOW_NORMAL
	imshow("mFiltered", mfiltered);
	Mat cluster = Mat::zeros(mfiltered.size(),CV_8UC1);
	cluster_rect(cluster, fVecCO2);
	/* namedWindow("Skel2 Adaptive Threshold", CV_WINDOW_AUTOSIZE);
	 imshow("Skel2 Adaptiv Threshold", skel2);
	 namedWindow("Skel3 Adaptive Threshold Gauss", CV_WINDOW_AUTOSIZE);
	 imshow("Skel3 Adaptive Threshold Gauss", skel3);*/

	//probabilistic_hough( );
	//cout << "wPixel 0,0 - 50,50" << count_whitePixel(Point2f(0,0), Point2f(50, 50), skel3) << endl;

	crop(src, cluster);

	waitKey(0);
	return (0);
}

vector<ContourObject> find_moments(Mat gray, int thresh, Mat skel) {

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
	resize(canny_output, canny_output, blank.size());

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

//function with side-effects
void make_skelekton(Mat img) {
	//Mat img;
	//gray.copyTo(img);

	Mat skel(img.size(), CV_8UC1, cv::Scalar(0));
	Mat temp;
	Mat eroded;

	Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

	bool done;
	do {
		erode(img, eroded, element);
		dilate(eroded, temp, element); // temp = open(img)
		subtract(img, temp, temp);
		bitwise_or(skel, temp, skel);
		eroded.copyTo(img);

		done = (countNonZero(img) == 0);
	} while (!done);

	skel.copyTo(img);
}

//callback function for trackbar
void bw_thresh_callback(int, void*) {

	threshold(gray, skel, thresh, max_thresh, cv::THRESH_BINARY);
	make_skelekton(skel);
	resize(skel, skel, blank.size());
	imshow(skel_window, skel);
}

void probabilistic_hough() {
	vector<Vec4i> p_lines;
	cvtColor(skel3, hough, COLOR_GRAY2BGR);

	/// 2. Use Probabilistic Hough Transform
	HoughLinesP(skel3, p_lines, 1, CV_PI / 180, 50, 30, 10);

	/// Show the result
	for (size_t i = 0; i < p_lines.size(); i++) {
		Vec4i l = p_lines[i];
		line(hough, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 3);

		//  line( probabilistic_hough, Point(lines[i][0], lines[i][1]),        //8
		//     Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, CV_AA );
	}

	namedWindow("hough Window", WINDOW_AUTOSIZE);
	imshow("hough Window", hough);
}

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



vector<ContourObject> find_mser(Mat gray) {

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

vector<ContourObject> filter_by_dst(vector<ContourObject> vecCO, int pxlSum, float threshDst) {

	vector<ContourObject> fVecCO;
	Mat m = Mat::zeros(skel3.size(), CV_8UC1);

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	find contours and crop region of interest
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void crop(Mat src, Mat img) {



	double largest_area = 0;
	int largest_contour_index = 0;
	Mat cropImage;
	Rect roi;														//region of interest

//	cvtColor(img, grey, CV_BGR2GRAY);
//	threshold(grey, grey, 125, 255, THRESH_BINARY);

	vector<vector<Point> > bc_contours; 							//contains potential barcode contour
	vector<Vec4i> hierarchy;

	findContours(img, bc_contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


	for (int i = 0; i < bc_contours.size(); i++) {

		double area = contourArea(bc_contours[i], false);

		if (area > largest_area) {

			largest_area = area;
			largest_contour_index = i;
			roi = boundingRect(bc_contours[i]); 					//find rectangle with largest contour
		}
	}

//	Scalar color(255, 255, 255);
//	drawContours(img, bc_contours, largest_contour_index, color, CV_FILLED, 8,	hierarchy);
//	rectangle(img, roi, Scalar(0, 255, 0), 1, 8, 0);

		Scalar color(255, 255, 255);
		drawContours(img, bc_contours, largest_contour_index, color, CV_FILLED, 8, hierarchy);
		rectangle(src, roi, Scalar(0, 255, 0), 1, 8, 0);




	/////////////////////////////////////////////////////////////////get angle and rotate


	Mat croppedRef(src, roi);											//crop and copy the region
	croppedRef.copyTo(cropImage);


	namedWindow("cropped", 0);
	imshow("cropped", cropImage);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	end of crop
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
