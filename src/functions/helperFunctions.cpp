/*
 * helperFunctions.cpp
 *
 *  Created on: 10.12.2015
 *      Author: Standardbenutzer
 */
#include "helperFunctions.h"

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

//-------------------------------------------------------------
/*checks if there's a white Pixel in the Area (defined by dst)
/around the centerPoint*/
//-------------------------------------------------------------
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

//function with side-effects
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
