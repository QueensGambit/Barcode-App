/*
 * drawingFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "drawingFunctions.h"

RNG rng(12345);
Mat draw_massCenter(vector<ContourObject> vecCO, Size size, const SettingObject& s){
	Mat m = Mat::zeros(size, CV_8UC1);
	vector<Point2f> massPoints;
	Scalar white = Scalar(255,255,255);

	for(int i = 0; i<vecCO.size(); i++){
		m.at<uchar>(vecCO[i].getMassCenter())=255;
		circle(m, vecCO[i].getMassCenter(), 2, white, -1, 8, 0);
		massPoints.push_back(vecCO[i].getMassCenter());

	}

//	draw_probabilistic_hough(m,vecCO);
//	draw_fit_line(massPoints, m);

	if (s.isShowAllSteps()) {
	namedWindow("malen", CV_WINDOW_AUTOSIZE);
	imshow("malen", m);
	waitAnyKey(s);
	}
	return m;
}
void draw_Circles(vector<Point2f> mc, Mat m) {

	cout << "mc.size() = " << mc.size() << endl;

	for (int i = 0; i < mc.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		circle(m, mc[i], 0, color, -1, 8, 0);
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

void draw_Lines(vector<ContourObject> vecCO, Mat m) {

	cout << "mc.size() lines = " << vecCO.size() << endl;
	for (int i = 0; i < vecCO.size(); i++) {

		vecCO[i].drawContourOnMat(m);

	}
}

void draw_minRectangles (vector<ContourObject> vecCO, Mat m) {

	  for( int i = 0; i< vecCO.size(); i++ )
	     {
	       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

	       Point2f rect_points[4];
	       vecCO[i].getRectPoints(rect_points);

	       Point intRect_points[4];
	       for( int j = 0; j < 4; j++ ) {
	    	  intRect_points[j] = rect_points[j];
	       }

	       fillConvexPoly( m, intRect_points, 4, color, 8, 0);//white = Scalar(255,255,255)

	     }
}

void draw_fit_line(vector<Point2f> points, Mat m) {

//	Mat mLines;
//	cvtColor(m, mLines, COLOR_GRAY2BGR);
	vector<Vec4i> lines;
    fitLine(points,lines,CV_DIST_L1,0,0.01,0.01);
	/*for (size_t i = 0; i < lines.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
						rng.uniform(0, 255));
		Vec4i l = lines[i];
//		line(mLines, Point(l[0], l[1]), Point(l[2], l[3]), color, 1);

	}*/

	namedWindow("fitLines Window", WINDOW_AUTOSIZE);
//	imshow("fitLines Window", mLines); //hough
}

void draw_hough_lines(Mat m, vector<Vec4i> p_lines) {

	Mat hough(m);
	cvtColor(m, hough, COLOR_GRAY2BGR);

	RNG rng(12345);
//	RNG rng(12335);

	for (size_t i = 0; i < p_lines.size(); i++) {
//			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),rng.uniform(0, 255));
			Scalar color = Scalar(rng.uniform(0, 2) * 255, rng.uniform(0, 2)* 255,rng.uniform(0, 2) * 255);

//			cout << "length in draw:" << norm(p_lines[i]) << endl;
			Vec4i l = p_lines[i];
//			Vec4i l = Vec4i(p_lines[i]);
	//		Vec4i l = p_lines[0];
//			cout << "l: " << l << endl;
//			cout << "l: " << l[0] << ", " << l[1] << ", " << l[2] << ", " << l[3] << endl;
//			cout << "length in draw:" << norm(Point(p_lines[i][0], p_lines[i][1])-Point(p_lines[i][2], p_lines[i][3])) << endl;
//			cout << "length in draw:" << norm(l) << endl;
			line(hough, Point(l[0], l[1]), Point(l[2], l[3]), color, 1);
//			line(hough, Point(0, 0), Point(l[0], l[1]), color, 1);

		}

		namedWindow("hough Window", WINDOW_AUTOSIZE);
		imshow("hough Window", hough); //hough
}

void draw_article_description(const string& article, const string& descr) {

	Mat canvas = Mat::zeros(300, 1200, CV_8UC3);
	canvas = Scalar(255,255,255);
//	int fontFace = FONT_HERSHEY_DUPLEX;
	int fontFace = FONT_HERSHEY_TRIPLEX;
	double fontScale = 0.8;
	int thickness = 1;

	int baseline=0;
	Size textSize = getTextSize(article, fontFace,
	                            fontScale, thickness, &baseline);
	baseline += thickness;

	// center the text
	Point textOrg((canvas.cols - textSize.width)/2,
	              (canvas.rows + textSize.height)/2);

	Point leftCorner = Point(20, 70);
	Point leftCornerLine2 = leftCorner;
	leftCornerLine2.y += 50;
	Point leftCornerLine3 = leftCornerLine2;
	leftCornerLine3.y += 50;
	/*
	text = "This is \n some text"
	y0, dy = 50, 4
	for i, line in enumerate(text.split('\n')):
	    y = y0 + i*dy
	    cv2.putText(img, line, (50, y ), cv2.FONT_HERSHEY_SIMPLEX, 1, 2)
	    */
	// then put the text itself
	putText(canvas, "article: " + article, //"article: " + article + "\n" + "description: " + descr
			leftCorner, fontFace, fontScale, //Scalar::all(255) = white
	        Scalar(0, 0, 0), thickness, 8);

	putText(canvas, "description: " + descr , //"article: " + article + "\n" + "description: " + descr
			leftCornerLine2, fontFace, fontScale, //Scalar::all(255) = white
		        Scalar(0, 0, 0), thickness, 8);

	putText(canvas, "information retrieved via codecheck.info ", //"article: " + article + "\n" + "description: " + descr
			leftCornerLine3, fontFace, fontScale/2, //Scalar::all(255) = white
		        Scalar(0, 0, 0), thickness, 8);

	namedWindow("Article Description", WINDOW_AUTOSIZE);

	imshow("Article Description", canvas);
}


void draw_barcode(Mat& img, const string& barcode, const string& type, size_t& number) {
	// imshow("imgout.jpg",imgout);
	//    namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
	//    imshow("MyWindow", imgout);

//		int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
//		int fontFace = FONT_HERSHEY_SIMPLEX;
//		int fontFace = FONT_HERSHEY_DUPLEX;
	int fontFace = FONT_HERSHEY_TRIPLEX;
	double fontScale = 2;
	int thickness = 3;

	int baseline=0;
	Size textSize = getTextSize(barcode, fontFace,
	                            fontScale, thickness, &baseline);
	baseline += thickness;

	// center the text
	Point textOrg((img.cols - textSize.width)/2,
	              (img.rows + textSize.height)/2);

	textOrg.y -= 20;

	putText(img, type, textOrg, fontFace, fontScale, //Scalar::all(255) = white
	        Scalar(255, 128, 0), thickness, 8);

	// then put the text itself
	putText(img, barcode, Point(textOrg.x, textOrg.y+60), fontFace, fontScale, //Scalar::all(255) = white
	        Scalar(255, 128, 0), thickness, 8);

}

