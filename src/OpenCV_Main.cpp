#include "OpenCV_Main.h"

//global variables
//Mat skel3, gray;//, //blank;

//const char* skel_window = "Skel Window";
int get_Barcode( Mat&, SettingObject& s);

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
//		if (!showAllSteps) {
//			stepByStep = false;
//		}
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

	if (s.isWebcam()) {
		src = get_image_from_webcam(webcamVersion, webcamStyle);
	}
	else {
//		cout << "load image from file" << endl;
		src = imread(s.getFile(), CV_LOAD_IMAGE_COLOR);
	}

	cout << "get_Barcode(src, s): " << get_Barcode(src, s) << endl;
	return 0;
}

int get_Barcode( Mat& src, SettingObject& s) {

	if (!s.isBasic()) {
	s.printSettings();
	}

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
	if (!s.isBasic()) {
	cout << "pxl_Sum: " << pxl_Sum << endl;
	}

//	Mat canny_output;
	Mat bw_thresh, gray;

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
	waitAnyKey(s);
	}
//	namedWindow(skel_window, CV_WINDOW_AUTOSIZE);

	adaptiveThreshold(gray, bw_thresh, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -8);

//	gray += ~gray;
//	gray /= 2;
	gray += .4 * bw_thresh; //*.4
//	gray += .4 * canny_output;

//	medianBlur ( gray, gray, 3 );
	if (s.isShowAllSteps()) {
	namedWindow( "Gray", CV_WINDOW_AUTOSIZE );
	imshow( "Gray", gray );
	waitAnyKey(s);
	}

//	find_moments(canny_output, 3, skel3, gray.size());

	vector<ContourObject> vecCO(find_mser(gray, s));

	if (!s.isBasic()) {
	cout << "vecCO.size() = " << vecCO.size() << endl;
	}

	Mat mfiltered  = Mat::zeros( src.size(), CV_8UC3 );
//	skel3.copyTo(mfiltered);
//	cvtColor(mfiltered, mfiltered, CV_GRAY2BGR);


	vector<ContourObject> fVecCO(filter_by_rect(vecCO, bw_thresh, .4, 5 )); //.95, 7

	vector<ContourObject> fVecCO2(fVecCO);
//	vector<ContourObject> fVecCO2(filter_by_dst(fVecCO, pxl_Sum, 0.0003, skel3.size() )); //0.00001

	if (!s.isBasic()) {
	cout << "fVecCO.size(): " << fVecCO.size() << endl;
	cout << "fVecCO2.size(): " << fVecCO2.size() << endl;
	}

	draw_minRectangles(fVecCO2, mfiltered);

	if (s.isShowAllSteps()) {
	namedWindow("mFiltered", CV_WINDOW_AUTOSIZE);	//CV_WINDOW_NORMAL
	imshow("mFiltered", mfiltered);
	waitAnyKey(s);
	}

	//cluster_rect was previously used to find potential barcode areas
	/*
	Mat cluster = Mat::zeros(mfiltered.size(),CV_8UC1);
	//cout << "mfilteed size: " << mfiltered.size();
	cluster_rect(cluster, fVecCO2);
	 */

	Mat mCenter = draw_massCenter(fVecCO2, mfiltered.size(), s);

	vector<Vec4i> pLines = get_probabilistic_hough_lines(mCenter);
	if (!s.isBasic()) {
	/// Show the result
	cout << "p_line.size(): " <<  pLines.size() << endl;
	}

	vector<Vec4i> fLines = filter_hough_lines2(pLines);
	if (!s.isBasic()) {
	cout << "f_line.size(): " <<  fLines.size() << endl;
	}

//	cout << "length norm(fLines[0]): " << norm(fLines[0]) << endl;
	if (s.isShowAllSteps()) {
	draw_hough_lines(mCenter, fLines);
	waitAnyKey(s);
	}
	vector<vector<Point2f> > cornerPoints = get_corner_points(fLines, fVecCO2, src.clone(), s);
	if (s.isShowAllSteps()) {
	waitAnyKey(s);
	}

	//find_groups(mfiltered, vector<ContourObject> fVecCO2);

	vector<Mat> mBarcodes;
	mBarcodes = p_transform(src, cornerPoints, s);
	waitAnyKey(s);

	/*vector<string> barcode;
	vector<string> type;
	vector<float> angle;*/

	string barcode;
	string type;
	float angle;
	bool readSuccess = false;
	size_t mBarcodesSize = mBarcodes.size();

	if (mBarcodesSize > 0) {
		for (size_t i = 0; i < mBarcodes.size(); i++) {
			readSuccess = get_barcode_string(mBarcodes[i], barcode, type, angle, i, s);

			if (readSuccess) {
				cout << "Barcode was successfully decoded." << endl;
				cout << "type: " << type << endl;
				cout << "barcode-symbol: "  << barcode << endl;
				cout << "angle: " << angle << endl;
//				string speak = string("espeak.exe -v de \"") + string("Typ: ") + type + string("barcode: ") + barcode + string("\"");
//				int retCode = system(speak.c_str());
				if (s.isBasic()) {
					return 1;
				}
			}
			else {
				if (!s.isBasic()) {
				cout << "no barcode could be read." << endl;
				}
			}

			//if the transformation failed to be read, sent the whole src image to zBar
			if (i == mBarcodesSize-1 && barcode.length() == 0) {
				//add the source image to the list
				Mat src_bw;
				src.copyTo(src_bw);
				make_adaptiv_bw(src_bw);

				mBarcodes.push_back(src_bw);
				mBarcodes.push_back(src);

			}
		}
	}


	String article, descr;
//	article = "Dontodent - Fresh White Zahnpflegekaugummis";
//	descr = "Zuckerfreie Kaugummi-Dragées mit Süßungsmitteln und Aroma glutenfrei laktosefrei ";

//	speak_article_descr(article, descr);


	bool searchSuccess = false;
//	cout << "type.: " << type << endl;
	if ( !s.isBasic() && type.find("EAN") != unsigned(-1) ) {
		const char* pathProductCSV = "media/database/product_database.csv";
		const char* pathAdderCSV = "media/database/adder_database.csv";
		cout << "i want to search!" << s.isSearch() << endl;
		waitAnyKey(s);
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
	} else if (!s.isBasic()){
		cout << "no article description was found." << endl;
	}

	if (!s.isBasic()) {
	waitKey(0);
	}
	return (0);
}
