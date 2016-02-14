/*
 * helperFunctions.cpp
 *
 *  Created on: 10.12.2015
 *      Author: Standardbenutzer
 */
#include "helperFunctions.h"

/*
 * !not in use!
 * brings a Point back into the valid range
 * if it was outside of the matrix
 */
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

/*
 * checks if there's a white Pixel in the Area (defined by dst)
 * around the centerPoint
 */
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

/*
 * calculates the distance between 2 points
 * or rather the length of the vector
 */
float get_length(Point p1, Point p2) {
	float x = p2.x - p1.x;
	float y = p2.y - p1.y;
	return sqrt(x * x + y * y);
}

/*
 * !not in use!
 * calculates the distance between 2 points
 * via the chess length method
 */
int chess_length(Point p1, Point p2) {
	return max(abs(p2.x - p1.x), abs(p2.y - p1.y));
}

/*
 * (!not in use!)
 * counts the white Pixels on a given matrix in an area which is defined
 * by the topLeft and the bottomRight Pixel
 */
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

/*
 * (!not in use!)
 * gets the top left white pixel of a white pixel area
 */
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

/*
 * !not in use!
 * calculates and returns the ratio of white pixels to black pixel
 * in an area of a rectangle
 */
double getfilledRatio(Mat& img, RotatedRect& rect){

    double non_zero = 0;
    double total = 0;
    Point2f rect_points[4];
    rect.points( rect_points );

      for(Point2f i=rect_points[0]; norm(i-rect_points[1])>1;
    		  i+=Point2f((rect_points[1]-i).x/norm((rect_points[1]-i)),
    				  (rect_points[1]-i).y/norm((rect_points[1]-i))))  {
        Point2f destination = i+rect_points[2]-rect_points[1];
          for(Point2f j=i; norm(j-destination)>1;
        		  j+=Point2f((destination.x-j.x)/norm((destination-j)),
        				  (destination.y-j.y)/norm((destination-j)))) {
            if(wPxl_in_Area(j, img, 0)){
                non_zero+=1;
            }
            total+=1;
        }
    }

    return non_zero/total;
}

/*
 * !not in use!
 * creates the skeleton of a given bw-matrix
 * this function has side-effects and manipulates the paramter
 */
void make_skelekton(Mat img) {

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

/*
 * returns the nearest ContourObject (specified by its index) to a given Point
 */
int get_Contour_Min_Dst (vector<ContourObject> vecCO, Point2f p) {
	int minDst = INT_MAX;
	int dst;
	int index = -1;
	for (int i = 0; i < vecCO.size(); i++) {
		dst = norm(vecCO[i].getMassCenter() - p);
		if (dst < minDst) {
			minDst = dst;
			index = i;
		}
	}

	return index;
}

/*
 * returns the nearest point of the rectangle
 * to the first point of the first point of the rectangle
 */
int get_min_Point_from_Rect (Point2f rect_point[4]) {

	int minDst = INT_MAX;
	int dst;
	int index = -1;
	for (int i = 1; i < 4; i++) {
		dst = norm(rect_point[i] - rect_point[0]);
		if (dst < minDst) {
			minDst = dst;
			index = i;
		}
	}

	return index;
}

/*
 * returns the 2 points which are the mean values
 * of the 2 top and the 2 bottom points of the rectangle.
 * The 2 points are represented by 4 integers values.
 */
Vec4i get_Border_Points_from_Rect(Point2f rect_point[4], bool b) {

	Point2f p = rect_point[0];
	int partner = get_min_Point_from_Rect(rect_point);
//	cout << "partner: " << partner << endl;
	int aPartner1, aPartner2;

	if (partner == 1 ) {
		aPartner1 = 2;
		aPartner2 = 3;
	}
	if (partner == 2 ) {
		aPartner1 = 1;
		aPartner2 = 3;
	}
	if (partner == 3 ) {
		aPartner1 = 1;
		aPartner2 = 2;
	}


	Vec4i v;
	v[0] = (p.x + rect_point[partner].x) / 2;
	v[1] = (p.y + rect_point[partner].y) / 2;
	v[2] = (rect_point[aPartner1].x + rect_point[aPartner2].x) / 2;
	v[3] = (rect_point[aPartner1].y + rect_point[aPartner2].y) / 2;
	return v;
}

/*
 * calculates the length of a vector given with 4 integer values
 * the norm(Vec4i) function doesn't seem to work correctly and caused bugs
 */
float get_length(Vec4i v) {

	return norm(Point(v[0], v[1]) - Point(v[2], v[3]));
}

/*
 * manipulates a given matrix by converting its color space
 * and adaptive thresholding it
 */
void make_adaptiv_bw(Mat& m) {
	cvtColor(m, m, CV_BGR2GRAY);
//	blur(m, m, Size(5, 5));
	adaptiveThreshold(m, m, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 0);
	cvtColor(m, m, CV_GRAY2BGR);
}

/*
 * return true if the arg is "true" otherwise false
 * manual conversion from char to bool
 */
bool getBoolValue(const char* arg) {
	if (string(arg) == "true") {
		return true;
	}
	return false;
}

/*
 * pauses the program by waiting for any key,
 * if the setting object allows it
 */
void waitAnyKey(const SettingObject& s) {
	if (s.isStepByStep()) {
		//idea only to accept the arrows keys was later replaced
		//by any key
		/*int key;// 2555904= ->  100=d
		while (key != 2555904 && key != 100) {
			key = waitKey(0);
	//		cout << "key: " << key << endl;
		}*/
		waitKey(0);
	}
}

/*
 * starts an executable as a parallel external process
 * this function is used to start the command-line-exe of eSpeaks
 */
int start_executable(string command) {
	 STARTUPINFO si;
	    PROCESS_INFORMATION pi;

	    ZeroMemory( &si, sizeof(si) );
	    si.cb = sizeof(si);
	    ZeroMemory( &pi, sizeof(pi) );

	    LPSTR s_command = const_cast<char *>(command.c_str());
//	    ".\\src\\eSpeak\\command_line\\espeak.exe --path=\".\\src\\eSpeak\" -v de -p 30 -s 145 -g 3 -m \" hallo das ist ein Test"
	    if (!CreateProcess(NULL, s_command, NULL, NULL, FALSE,
	            0, NULL, NULL, &si, &pi )) {
	        cout << "Error occured" <<  GetLastError() << endl;
	        // more on http://msdn.microsoft.com/en-us/library/ms680582%28VS.85%29.aspx

	        return 1;
	    }

	    cout << "Process started correctly, Process-ID:" << pi.dwProcessId << endl;
       int i = 0;

	     // Close process and thread handles.
	     CloseHandle( pi.hProcess );
	     CloseHandle( pi.hThread );
   return 0;
}

/*
 * converts the src-image into a gray image with enhanced edges
 * and a bw-image via the adaptive-threshold method
 */
void prepareSrc(const Mat& src, Mat& bw_thresh, Mat& gray) {
	cvtColor(src, gray, CV_BGR2GRAY);

	///blurring was used prior the canny filter
//	blur(gray, canny_output, Size(3, 3));

	//invert the image !
	gray = ~gray;

	//earlier attempt with the canny filter
//	Canny(gray, canny_output, 50, 150, 3);
//	namedWindow("Canny", CV_WINDOW_AUTOSIZE);
//	imshow("Canny", canny_output);

//resizing is an option if the src image is too big for the screen display
//	resize(src, src, blank.size());

//	namedWindow(skel_window, CV_WINDOW_AUTOSIZE);

	adaptiveThreshold(gray, bw_thresh, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -8);

	//other ideas to use the canny filter to enhance edges
//	gray += ~gray;
//	gray /= 2;
//	gray += .4 * canny_output;

	gray += .4 * bw_thresh;

}
