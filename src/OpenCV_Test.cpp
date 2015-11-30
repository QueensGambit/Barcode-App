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

using namespace cv;
using namespace std;

RNG rng(12345);

vector<ContourObject> find_moments(Mat, int, Mat);
void bw_thresh_callback(int, void*);
void make_skelekton(Mat);
void probabilistic_hough();
vector<Point2f> filter_moments(vector<Point2f>, Mat);
void draw_Circles(vector<Point2f>, Mat);
void draw_Circles(vector<ContourObject>, Mat);
void draw_Lines(vector<ContourObject>, Mat);

//helper fuctions
Point normalize (Point, Mat);
double get_length(Point, Point);
int chess_length(Point, Point);
int count_whitePixel(Point2f, Point2f, Mat);
Point2f topLeftPixel(Point2f, Mat);
Point2f bottomRightPixel(Point2f, Mat);
bool wPxl_in_Area(Point2f&, Mat&, float);
double filling(Mat&, RotatedRect&);

vector<ContourObject> filter_lines(vector<ContourObject>, Mat, int);
vector<Point2f> island_filter(vector<Point2f>, Mat, int);
void draw_minRectangles (vector<ContourObject> vecCO, Mat m);
vector<ContourObject> filter_by_rect(vector<ContourObject>, Mat, double);
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
	//src = imread("G:/Gimp/DBV/internet/Gefro Pesto Verde (2).jpg", CV_LOAD_IMAGE_COLOR);
	//src = imread("G:/Gimp/DBV/sample.png", CV_LOAD_IMAGE_COLOR);
	//src = imread("G:/Gimp/DBV/barcode_object3.png", CV_LOAD_IMAGE_COLOR);
//	src = imread("G:/Gimp/DBV/bilder/kritisch/2015-10-07 13.18.46.jpg", CV_LOAD_IMAGE_COLOR);
//		src = imread("G:/Gimp/DBV/bilder/gut/20151007_133654.jpg", CV_LOAD_IMAGE_COLOR);
	//blank = imread("C:/Users/Suhay/Dropbox/BarCode/bilder/gut/blank.jpg", CV_LOAD_IMAGE_COLOR);

	src = imread("media/internet/Chips_rotated.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/chips.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Gefro Pesto Verde (2).jpg", CV_LOAD_IMAGE_COLOR);
	//blank = imread("G:/Gimp/DBV/blank2.jpg", CV_LOAD_IMAGE_COLOR);
	blank = imread("media/blank2.jpg", CV_LOAD_IMAGE_COLOR);

	int pxl_Sum = src.cols * src.rows;

	cvtColor(src, gray, CV_BGR2GRAY);
	blur(gray, gray, Size(3, 3));

	//invert the image !
	gray = ~gray;

	resize(src, src, blank.size());
	namedWindow("Source", CV_WINDOW_AUTOSIZE);
	imshow("Source", src);

	//namedWindow( "Gray", CV_WINDOW_AUTOSIZE );
	//imshow( "Gray", gray );

	namedWindow(skel_window, CV_WINDOW_AUTOSIZE);
	//createTrackbar("Threshold: ", "Source", &thresh, max_thresh, bw_thresh_callback);
	//15, -2
	//adaptiveThreshold(gray, skel2, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -8);
	//make_skelekton(skel2);

	adaptiveThreshold(gray, skel3, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -8);
	//make_skelekton(skel3);
	//cout << "pxl_Sum * 0.0005 = " << pxl_Sum * 0.00005 << endl;
	//vector<Point2f> mc (find_moments( gray , pxl_Sum * 0.00005));
	vector<ContourObject> vecCO(find_moments(gray, pxl_Sum * 0.00004, skel3));
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

	vector<ContourObject> fVecCO(filter_by_rect(vecCO, skel3, .95 ));
	cout << "fVecCO.size(): " << fVecCO.size() << endl;
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
	draw_minRectangles(fVecCO, mfiltered);
	//bitwise_and(mfiltered, mRotRect, mfiltered);

	//resize(skel2, skel2, blank.size());
	//resize(skel3, skel3, blank.size());
//	resize(mfiltered, mfiltered, blank.size());

	namedWindow("mFiltered", CV_WINDOW_AUTOSIZE);	//CV_WINDOW_NORMAL
	imshow("mFiltered", mfiltered);
	Mat cluster = Mat::zeros(mfiltered.size(),CV_8UC1);
	cluster_rect(cluster, fVecCO);
	/* namedWindow("Skel2 Adaptive Threshold", CV_WINDOW_AUTOSIZE);
	 imshow("Skel2 Adaptiv Threshold", skel2);
	 namedWindow("Skel3 Adaptive Threshold Gauss", CV_WINDOW_AUTOSIZE);
	 imshow("Skel3 Adaptive Threshold Gauss", skel3);*/

	//probabilistic_hough( );
	//cout << "wPixel 0,0 - 50,50" << count_whitePixel(Point2f(0,0), Point2f(50, 50), skel3) << endl;
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

bool wPxl_in_Area(Point2f& center, Mat& m, float dst) {

	float endX = center.x + dst;
	float endY = center.y + dst;
	//cout << "endX: " << endX << endl;

	for (float x = center.x-dst; x <= endX && x <= m.rows && x >= 0; x+= 1.0f) {
		for (float y = center.y-dst; y <= endY && y <= m.cols && y >= 0; y+= 1.0f) {
			//if (m.at<uchar>( normalize(Point(x, y), m) ) == 255) {
//				if (m.at<uchar>( Point(x, y)) == 255) {
				if (m.at<uchar>(Point2f(x,y)) != 0) {
				circle(m, Point(x, y), 2, Scalar(255,0,255), -1, 8, 0);
				return true;
			}
		}
	}
	return false;
}

double get_length(Point p1, Point p2) {
	double x = p2.x - p1.x;
	double y = p2.y - p1.y;
	return sqrt(x * x + y * y);
}

int chess_length(Point p1, Point p2) {
	return max(abs(p2.x - p1.x), abs(p2.y - p1.y));
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

//get top left white pixel
Point2f topLeftPixel(Point2f mc, Mat skel) {

	Point2f topLeftPixel;
	vector<Point2f> border;
	int wPixel;
	int iteration = 0;
	int xStart = mc.x - 1;
	int xEnd = mc.x + 1;
	int yStart = mc.y - 1;
	int yEnd = mc.y + 1;

	border.push_back(mc);
	do {
		wPixel = 0;

		//correction
		if (xStart < 0)
			xStart = 0;
		if (yStart < 0)
			yStart = 0;

		if (xEnd > skel.rows)
			xEnd = skel.rows;
		if (yEnd > skel.cols)
			yEnd = skel.cols;

		for (int x = xStart; x <= xEnd; x++) {
			for (int y = yEnd; y <= yEnd; y++) {
				if (skel.at<uchar>(Point(x, y)) == 255) {
					wPixel++;
					iteration++;
				}
			}
		}

		//expand the border
		xStart--;
		xEnd++;
		yStart--;
		yEnd++;

	} while (wPixel != 0 && xStart != 0 && yStart != 0 && xEnd != skel.rows
			&& yEnd != skel.rows);

	int xPos = mc.x - iteration;
	int yPos = mc.y - iteration;
	if (xPos < 0)
		xPos = 0;
	if (yPos < 0)
		yPos = 0;

	topLeftPixel.x = xPos;
	topLeftPixel.y = yPos;

	return topLeftPixel;
}

//get bottom right white pixel
Point2f bottomRightPixel(Point2f mc, Mat skel) {

	Point2f tLPixel = topLeftPixel(mc, skel);
	Point2f bRPixel;

	int diff = mc.x - tLPixel.x;
	int xPos = mc.x + diff;
	int yPos = mc.y + diff;
	if (xPos > skel.rows)
		xPos = skel.rows;
	if (yPos > skel.cols)
		yPos = skel.cols;

	bRPixel.x = xPos;
	bRPixel.y = yPos;

	return bRPixel;
}

int count_whitePixel(Point2f topLeft, Point2f bottomRight, Mat skel) {
	int c = 0;
	for (int x = topLeft.x; x <= bottomRight.x; x++) {
		for (int y = topLeft.y; y <= bottomRight.y; y++) {
			if (skel.at<uchar>(Point(x, y)) == 255) {
				c++;
			}
		}
	}
	//cout << "wPixel: " << c << endl;
	return c;
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

vector<ContourObject> filter_by_rect(vector<ContourObject>vecCO, Mat m, double thresh) {

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

			 if (filling(m, minRect[i]) >= thresh && ratio >= 10) {
				   Point2f rect_points[4];
				   minRect[i].points( rect_points );
				   vecCO[i].setRectPoints(rect_points);
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
}

Point normalize (Point p, Mat m) {
	if (p.x < 0) {
		p.x = 0;
	}
	if (p.y < 0) {
		p.y = 0;
	}
	if (p.x > m.rows) {
		p.x = m.rows;
	}
	if (p.y > m.cols) {
		p.y = m.cols;
	}
	return p;
}

double filling(Mat& img, RotatedRect& rect){

    double non_zero = 0;
    double total = 0;
    Point2f rect_points[4];
    rect.points( rect_points );

//    for(Point2f i=rect_points[0]; norm(i-rect_points[1])>1; i+=(rect_points[1]-i)/norm((rect_points[1]-i)))  {
      for(Point2f i=rect_points[0]; norm(i-rect_points[1])>1;
    		  i+=Point2f((rect_points[1]-i).x/norm((rect_points[1]-i)),
    				  (rect_points[1]-i).y/norm((rect_points[1]-i))))  {
        Point2f destination = i+rect_points[2]-rect_points[1];
//        for(Point2f j=i; norm(j-destination)>1; j+=(destination-j)/norm((destination-j))) {
          for(Point2f j=i; norm(j-destination)>1;
        		  j+=Point2f((destination.x-j.x)/norm((destination-j)),
        				  (destination.y-j.y)/norm((destination-j)))) {
//            if(img.at<uchar>(j) != 0){
            if(wPxl_in_Area(j, img, 0)){
                non_zero+=1;
            }
            total+=1;
        }
    }

    return non_zero/total;
}
