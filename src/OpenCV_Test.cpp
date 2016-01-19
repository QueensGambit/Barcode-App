#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "objects/ContourObject.h"
#include "objects/Vektor2d.h"
#include "objects/SettingObject.h"

#include "functions/helperFunctions.h"
#include "functions/drawingFunctions.h"
#include "functions/filterFunctions.h"
#include "functions/finderFunctions.h"
#include "functions/loaderFunctions.h"
#include "functions/transformerFunctions.h"

using namespace cv;
using namespace std;

//global variables
Mat skel, skel2, skel3, gray;//, //blank;

//const char* skel_window = "Skel Window";
int get_Barcode(const Mat&, SettingObject& s);

int main(int argc, const char** argv ) {

	//example input: Barcode-App.exe false false false true false media/internet/chips.jpg i s
	const char* execute;
	bool stepByStep, showAllSteps, search, webcam, speach;
	const char* file;
	char webcamVersion, webcamStyle;

	execute = argv[0];
	SettingObject s(execute);

	if (argc >= 7) { //6 is the least number of paramters
		stepByStep = getBoolValue(argv[1]);
		showAllSteps = getBoolValue(argv[2]);
		//step by step doesn't make sense if there aren't multiple windows
		if (!showAllSteps) {
			stepByStep = false;
		}
		search = getBoolValue(argv[3]);
		webcam = getBoolValue(argv[4]);
		speach = getBoolValue(argv[5]);
		file = argv[6];
		if (webcam) {
		webcamVersion = argv[7][0];//i(ntern) | e(xtern)
		webcamStyle = argv[8][0]; //s(ingle frame) | m(any frames (loop))
		}
		else {
		//0 means invalid setting
		webcamVersion = '0';
		webcamStyle = '0';
		}

		s.setValues(stepByStep, showAllSteps, search, webcam, speach, file, webcamVersion, webcamStyle);
		//s = s2;
	}

	Mat src;

	if (webcam) {
		src = get_image_from_webcam(webcamVersion, webcamStyle);
	}
	else {
	src = imread(s.getFile(), CV_LOAD_IMAGE_COLOR);
	}

	get_Barcode(src, s);

	return 0;
}

int get_Barcode(const Mat& src, SettingObject& s) {

	s.printSettings();

	/// Load source image, convert it to gray and blur it
//	Mat src;	//, gray;

//	src = get_image_from_webcam();
//	src = imread("media/gut/joghurt_scaled.jpg");
//	src = imread("media/gut/highQu_scaled.jpg");
//	src = imread("media/gut/toffifee_scaled.jpg");
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

//	Mat canny_output;

	cvtColor(src, gray, CV_BGR2GRAY);
//	blur(gray, canny_output, Size(3, 3));

	//invert the image !
	gray = ~gray;

//	Canny(gray, canny_output, 50, 150, 3);
//	namedWindow("Canny", CV_WINDOW_AUTOSIZE);
//	imshow("Canny", canny_output);

//	resize(src, src, blank.size());
	if (s.isShowAllSteps()) {
	namedWindow("Source", CV_WINDOW_AUTOSIZE);
	imshow("Source", src);
	waitArrowKey(s);
	}
//	namedWindow(skel_window, CV_WINDOW_AUTOSIZE);

	adaptiveThreshold(gray, skel3, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -8);

//	gray += ~gray;
//	gray /= 2;
	gray += .4 * skel3; //*.4
//	gray += .4 * canny_output;

//	medianBlur ( gray, gray, 3 );
	if (s.isShowAllSteps()) {
	namedWindow( "Gray", CV_WINDOW_AUTOSIZE );
	imshow( "Gray", gray );
	waitArrowKey(s);
	}

//	find_moments(canny_output, 3, skel3, gray.size());

	vector<ContourObject> vecCO(find_mser(gray, s));

	cout << "vecCO.size() = " << vecCO.size() << endl;

	Mat mfiltered  = Mat::zeros( src.size(), CV_8UC3 );
//	skel3.copyTo(mfiltered);
//	cvtColor(mfiltered, mfiltered, CV_GRAY2BGR);


	vector<ContourObject> fVecCO(filter_by_rect(vecCO, skel3, .4, 5 )); //.95, 7

	vector<ContourObject> fVecCO2(fVecCO);
//	vector<ContourObject> fVecCO2(filter_by_dst(fVecCO, pxl_Sum, 0.0003, skel3.size() )); //0.00001

	cout << "fVecCO.size(): " << fVecCO.size() << endl;
	cout << "fVecCO2.size(): " << fVecCO2.size() << endl;

	draw_minRectangles(fVecCO2, mfiltered);

	if (s.isShowAllSteps()) {
	namedWindow("mFiltered", CV_WINDOW_AUTOSIZE);	//CV_WINDOW_NORMAL
	imshow("mFiltered", mfiltered);
	waitArrowKey(s);
	}

	//cluster_rect was previously used to find potential barcode areas
	/*
	Mat cluster = Mat::zeros(mfiltered.size(),CV_8UC1);
	//cout << "mfilteed size: " << mfiltered.size();
	cluster_rect(cluster, fVecCO2);
	 */

	Mat mCenter = draw_massCenter(fVecCO2, mfiltered.size(), s);

	vector<Vec4i> pLines = get_probabilistic_hough_lines(mCenter);
	vector<Vec4i> fLines = filter_hough_lines2(pLines);
	cout << "f_line.size(): " <<  fLines.size() << endl;
//	cout << "length norm(fLines[0]): " << norm(fLines[0]) << endl;
	if (s.isShowAllSteps()) {
	draw_hough_lines(mCenter, fLines);
	waitArrowKey(s);
	}
	vector<vector<Point2f> > cornerPoints = get_corner_points(fLines, fVecCO2, src.clone(), s);
	if (s.isShowAllSteps()) {
	waitArrowKey(s);
	}

	//find_groups(mfiltered, vector<ContourObject> fVecCO2);

	vector<Mat> mBarcodes;
	mBarcodes = p_transform(src, cornerPoints, s);
	waitArrowKey(s);

	/*vector<string> barcode;
	vector<string> type;
	vector<float> angle;*/

	string barcode;
	string type;
	float angle;

	if (mBarcodes.size() > 0) {
		for (size_t i = 0; i < mBarcodes.size(); i++) {
			bool readSuccess = get_barcode_string(mBarcodes[i], barcode, type, angle, i);

			if (readSuccess) {
				cout << "Barcode was successfully decoded." << endl;
				cout << "type: " << type << endl;
				cout << "barcode-symbol: "  << barcode << endl;
				cout << "angle: " << angle << endl;
//				string speak = string("espeak.exe -v de \"") + string("Typ: ") + type + string("barcode: ") + barcode + string("\"");
//				int retCode = system(speak.c_str());
			}
			else {
				cout << "no barcode could be read." << endl;
			}
		}
	}
	waitArrowKey(s);
	String article, descr;
	article = "Dontodent - Fresh White Zahnpflegekaugummis";
	descr = "Zuckerfreie Kaugummi-Dragées mit Süßungsmitteln und Aroma glutenfrei laktosefrei ";

//	speak_article_descr(article, descr);

	const char* pathProductCSV = "media/database/product_database.csv";
	const char* pathAdderCSV = "media/database/adder_database.csv";

	bool searchSuccess = false;
//	cout << "type.: " << type << endl;
	if (type.find("EAN") != unsigned(-1)) {
		searchSuccess = get_article_descr_csv(pathProductCSV, barcode, article, descr) ||
			get_article_descr_csv(pathAdderCSV, barcode, article, descr);
		if (searchSuccess) {
			cout << "article:" << article << endl;
			cout << "beschr: " << descr << endl;
		} else if (s.isSearch()){
			cout << "look-up at codecheck.info..." << endl;
			searchSuccess = get_article_description_internet(barcode, article, descr);
			if (searchSuccess) {
				add_article_descr_to_csv(pathAdderCSV, barcode, article, descr);
			}
		}
	}

	if (searchSuccess) {
//		cout << "article: " << article << endl;
		draw_article_description(article, descr);
		if (s.isSpeach()) {
		speak_article_descr(article, descr);
		}
	} else {
		cout << "no article description was found." << endl;
	}

	waitKey(0);

	return (0);
}
