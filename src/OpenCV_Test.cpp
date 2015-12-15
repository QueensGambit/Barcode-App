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

#include "functions/helperFunctions.h"
#include "functions/drawingFunctions.h"
#include "functions/filterFunctions.h"
#include "functions/finderFunctions.h"

using namespace cv;
using namespace std;

void crop(Mat, Mat);

void bw_thresh_callback(int, void*);

//global variables
Mat skel, skel2, skel3, gray, blank;
int thresh = 127;
int max_thresh = 255;

char* skel_window = "Skel Window";

int main() {
	/// Load source image, convert it to gray and blur it
	Mat src;	//, gray;

//	src = imread("media/internet/test.png");
//	src = imread("media/internet/Chips_rotated.jpg", CV_LOAD_IMAGE_COLOR);
	src = imread("media/internet/per_verzerrt.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/chips.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/gut/mandarine.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Gefro Pesto Verde (2).jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Knorr fix Bolognese (3).JPG", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Knorr fix Bolognes Tomaten-Mozarella (4).JPG", CV_LOAD_IMAGE_COLOR);


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

	adaptiveThreshold(gray, skel3, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -8);

	gray += .4 * skel3;
//	medianBlur ( gray, gray, 3 );
	namedWindow( "Gray", CV_WINDOW_AUTOSIZE );
	imshow( "Gray", gray );

	vector<ContourObject> vecCO(find_mser(gray));

	cout << "vecCO.size() = " << vecCO.size() << endl;

	Mat mfiltered; // = Mat::zeros( skel2.size(), CV_8UC3 );
	skel3.copyTo(mfiltered);
	cvtColor(mfiltered, mfiltered, CV_GRAY2BGR);

	vector<ContourObject> fVecCO(filter_by_rect(vecCO, skel3, .4, 5 )); //.95, 7
	vector<ContourObject> fVecCO2(filter_by_dst(fVecCO, pxl_Sum, 0.0003, skel3.size() )); //0.00001

	cout << "fVecCO.size(): " << fVecCO.size() << endl;
	cout << "fVecCO2.size(): " << fVecCO2.size() << endl;

	draw_minRectangles(fVecCO2, mfiltered);

	namedWindow("mFiltered", CV_WINDOW_AUTOSIZE);	//CV_WINDOW_NORMAL
	imshow("mFiltered", mfiltered);
	Mat cluster = Mat::zeros(mfiltered.size(),CV_8UC1);
	cluster_rect(cluster, fVecCO2);

	draw_massCenter(fVecCO2, mfiltered.size());
//	crop(src, cluster);

	waitKey(0);
	return (0);
}


//callback function for trackbar
void bw_thresh_callback(int, void*) {

	threshold(gray, skel, thresh, max_thresh, cv::THRESH_BINARY);
	make_skelekton(skel);
	resize(skel, skel, blank.size());
	imshow(skel_window, skel);
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

	Size s( roi.width * 0.3, roi.height * 0.3 );					//expanding rectangle by 30%
	Point offset( s.width/2, s.height/2);							//shifting the rectangle
	roi += s;
	roi -= offset;

	Scalar color(255, 255, 255);
	drawContours(img, bc_contours, largest_contour_index, color, CV_FILLED, 8, hierarchy);
	rectangle(src, roi, Scalar(0, 255, 0), 1, 8, 0);



	/////////////////////////////////////////////////////////////////get angle and rotate


	Mat croppedRef(src, roi);										//crop and copy the region
	croppedRef.copyTo(cropImage);


	namedWindow("cropped", 0);
	imshow("cropped", cropImage);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	end of crop
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
