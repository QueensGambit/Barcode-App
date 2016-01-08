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
#include "functions/loaderFunctions.h"

using namespace cv;
using namespace std;

void crop(Mat, Mat);

void bw_thresh_callback(int, void*);
void p_transform(Mat, vector<vector<Point2f> >);

//global variables
Mat skel, skel2, skel3, gray;//, //blank;
int thresh = 127;
int max_thresh = 255;

char* skel_window = "Skel Window";

int main() {
	/// Load source image, convert it to gray and blur it
	Mat src;	//, gray;

//	src = get_image_from_webcam();

//	src = imread("media/gut/highQu_scaled.jpg");
	src = imread("media/gut/toffifee_scaled.jpg");
//	src = imread("media/gut/mandarine_scaled.jpg");
//	src = imread("media/gut/bottle_scaled.jpg");
//	src = imread("media/internet/test.png");
//	src = imread("media/internet/Chips_rotated.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/per_verzerrt.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/chips.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/gut/mandarine.jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Gefro Pesto Verde (2).jpg", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Knorr fix Bolognese (3).JPG", CV_LOAD_IMAGE_COLOR);
//	src = imread("media/internet/Knorr fix Bolognes Tomaten-Mozarella (4).JPG", CV_LOAD_IMAGE_COLOR);


	//blank = imread("media/blank2.jpg", CV_LOAD_IMAGE_COLOR);

	if (src.empty()) {
		cout << "src is empty!" << endl;
		return -1;
	}

//	blank = imread("media/blank2.jpg", CV_LOAD_IMAGE_COLOR);


	int pxl_Sum = src.cols * src.rows;
	cout << "pxl_Sum: " << pxl_Sum << endl;

	Mat canny_output;

	cvtColor(src, gray, CV_BGR2GRAY);
	blur(gray, canny_output, Size(3, 3));

	//invert the image !
	gray = ~gray;

	Canny(gray, canny_output, 50, 150, 3);
	namedWindow("Canny", CV_WINDOW_AUTOSIZE);
	imshow("Canny", canny_output);

//	resize(src, src, blank.size());
	namedWindow("Source", CV_WINDOW_AUTOSIZE);
	imshow("Source", src);

//	namedWindow(skel_window, CV_WINDOW_AUTOSIZE);

	adaptiveThreshold(gray, skel3, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -8);

//	gray += ~gray;
//	gray /= 2;
	gray += .4 * skel3; //*.4
//	gray += .4 * canny_output;

//	medianBlur ( gray, gray, 3 );
	namedWindow( "Gray", CV_WINDOW_AUTOSIZE );
	imshow( "Gray", gray );

//	find_moments(canny_output, 3, skel3, gray.size());

	vector<ContourObject> vecCO(find_mser(gray));

	cout << "vecCO.size() = " << vecCO.size() << endl;

	Mat mfiltered; // = Mat::zeros( skel2.size(), CV_8UC3 );
	skel3.copyTo(mfiltered);
	cvtColor(mfiltered, mfiltered, CV_GRAY2BGR);


	vector<ContourObject> fVecCO(filter_by_rect(vecCO, skel3, .4, 5 )); //.95, 7

	/*while (true) {

			if (waitKey(30) == 13) {// press enter to break loop and copy frame into source image
//				frame.copyTo(src);
//				destroyAllWindows();
				break;
			}
		}*/

	vector<ContourObject> fVecCO2(fVecCO);
//	vector<ContourObject> fVecCO2(filter_by_dst(fVecCO, pxl_Sum, 0.0003, skel3.size() )); //0.00001

	cout << "fVecCO.size(): " << fVecCO.size() << endl;
	cout << "fVecCO2.size(): " << fVecCO2.size() << endl;

	draw_minRectangles(fVecCO2, mfiltered);

	namedWindow("mFiltered", CV_WINDOW_AUTOSIZE);	//CV_WINDOW_NORMAL
	imshow("mFiltered", mfiltered);

	Mat cluster = Mat::zeros(mfiltered.size(),CV_8UC1);
	//cout << "mfilteed size: " << mfiltered.size();
	cluster_rect(cluster, fVecCO2);

	Mat mCenter = draw_massCenter(fVecCO2, mfiltered.size());

	vector<Vec4i> pLines = get_probabilistic_hough_lines(mCenter);
	vector<Vec4i> fLines = filter_hough_lines2(pLines);
	cout << "f_line.size(): " <<  fLines.size() << endl;
//	cout << "length norm(fLines[0]): " << norm(fLines[0]) << endl;
	draw_hough_lines(mCenter, fLines);
	vector<vector<Point2f> > cornerPoints = get_corner_points(fLines, fVecCO2, src.clone());

	//find_groups(mfiltered, vector<ContourObject> fVecCO2);

	p_transform(src, cornerPoints);

	waitKey(0);
	return (0);
}


//callback function for trackbar
void bw_thresh_callback(int, void*) {

	threshold(gray, skel, thresh, max_thresh, cv::THRESH_BINARY);
	make_skelekton(skel);
//	resize(skel, skel, blank.size());
	imshow(skel_window, skel);
}



void p_transform(Mat src, vector<vector<Point2f> > cornerPoints) {


	Mat transform = Mat::zeros(300, 500, CV_8UC3);


//	Mat cropImage;
//	Rect roi;
//	for (int i = 0; i < cornerPoints.size(); i++) {
//		roi = boundingRect(cornerPoints[i]);
//	}
//
//	Size s(roi.width * .1, roi.height * .1);
//	Point offset(s.width / 2, s.height / 2);			//shifting the rectangle
//	roi += s;
//	roi -= offset;
//
//	Mat croppedRef(src, roi);						//crop and copy the region
//	croppedRef.copyTo(cropImage);
//	namedWindow("cropped", 0);
//	imshow("cropped", cropImage);



	///zoom out
	for (int i = 0; i < cornerPoints.size(); i++) {
		cornerPoints[i][0].x -= 50;
		cornerPoints[i][0].y -= 50;

		cornerPoints[i][1].x -= 50;
		cornerPoints[i][1].y += 50;

		cornerPoints[i][2].x += 50;
		cornerPoints[i][2].y -= 50;

		cornerPoints[i][3].x += 50;
		cornerPoints[i][3].y += 50;
	}

	///sort corners to correct order
	for (int i = 0; i < cornerPoints.size(); i++) {

		Point2f tmpP2, tmpP3, tmpP4;

		tmpP4 = cornerPoints[i][1];
		tmpP3 = cornerPoints[i][3];
		tmpP2 = cornerPoints[i][2];

		cornerPoints[i][1] = tmpP2;
		cornerPoints[i][2] = tmpP3;
		cornerPoints[i][3] = tmpP4;
	}

	vector<Point2f> quad_pts;
	quad_pts.push_back(Point2f(0, 0));
	quad_pts.push_back(Point2f(transform.cols, 0));
	quad_pts.push_back(Point2f(transform.cols, transform.rows));
	quad_pts.push_back(Point2f(0, transform.rows));



	for (int i = 0; i < cornerPoints.size(); i++) {
		Mat transmtx = getPerspectiveTransform(cornerPoints[i], quad_pts);
		warpPerspective(src, transform, transmtx, transform.size());
		imshow("transform", transform);
	}

}

