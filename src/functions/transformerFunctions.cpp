/*
 * transformerFunctions.cpp
 *
 *  Created on: 09.01.2016
 *      Author: Anonym
 */

#include "transformerFunctions.h"


vector<Mat> p_transform(Mat src, vector<vector<Point2f> > cornerPoints) {

	vector<Mat> mBarcode;

	String strTransform = "transform ";

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



	///sort corners to correct order
	for (int i = 0; i < cornerPoints.size(); i++) {
/*
		Point2f tmpP2, tmpP3, tmpP4;

		tmpP4 = cornerPoints[i][1];
		tmpP3 = cornerPoints[i][3];
		tmpP2 = cornerPoints[i][2];

		cornerPoints[i][1] = tmpP2;
		cornerPoints[i][2] = tmpP3;
		cornerPoints[i][3] = tmpP4;
		*/



		Point2f tmpP[4];

		tmpP[0] = cornerPoints[i][1];
		tmpP[1] = cornerPoints[i][0];
		tmpP[2] = cornerPoints[i][2];
		tmpP[3] = cornerPoints[i][3];

		cornerPoints[i][0] = tmpP[0];
		cornerPoints[i][1] = tmpP[1];
		cornerPoints[i][2] = tmpP[2];
		cornerPoints[i][3] = tmpP[3];

		/*for (int z = 0; z < 4; z++) {
			cout << "cornerPoints[" << z << "]:" << cornerPoints[i][z] << endl;
		}*/
	}


		///zoom out
		for (int i = 0; i < cornerPoints.size(); i++) {

			/*for (int z = 0; z < 4; z++) {
				cout << "cornerPoints[" << z << "]:" << cornerPoints[i][z] << endl;
			}*/
/*
			cornerPoints[i][0].x -= scaleVal;
			cornerPoints[i][0].y -= scaleVal;

			cornerPoints[i][1].x -= scaleVal;
			cornerPoints[i][1].y += scaleVal;

			cornerPoints[i][2].x += scaleVal;
			cornerPoints[i][2].y += scaleVal;

			cornerPoints[i][3].x += scaleVal;
			cornerPoints[i][3].y -= scaleVal;
		*/
			Point2f v[4][2];
			Point2f w[4];
			Point2f w_norm[4];

			v[0][1] = cornerPoints[i][1] - cornerPoints[i][0];
			v[0][2] = cornerPoints[i][3] - cornerPoints[i][0];

			v[1][1] = cornerPoints[i][0] - cornerPoints[i][1];
			v[1][2] = cornerPoints[i][2] - cornerPoints[i][1];

			v[2][1] = cornerPoints[i][1] - cornerPoints[i][2];
			v[2][2] = cornerPoints[i][3] - cornerPoints[i][2];

			v[3][1] = cornerPoints[i][0] - cornerPoints[i][3];
			v[3][2] = cornerPoints[i][2] - cornerPoints[i][3];

			float width1 = max(norm(v[0][1]), norm(v[0][2]));
			float height1 = min(norm(v[0][1]), norm(v[0][2]));
			float width2= max(norm(v[2][1]), norm(v[2][2]));
			float height2 = min(norm(v[2][1]), norm(v[2][2]));

			float width = min(width1, width2);
			float height = max(height1, height2);
/*
			cout << "width1: " << width1 << endl;
			cout << "height1: " << height1 << endl;
			cout << "width2: " << width2 << endl;
			cout << "height2: " << height2 << endl;*/


			//2 Variants for each Barcode
			//1 is Original Size, the other is half the size

		Mat transform;
		float newWidth = width * ((300 / height));
		if (newWidth > 1400) {
			newWidth = 1400;
		}
		if (width / height < 4) {

			transform = Mat::zeros(300, newWidth, CV_8UC3);
		}
		else {
			transform = Mat::zeros(300, newWidth * 0.5, CV_8UC3);
		}

//		int scaleVal = 20;
		cout << "new Width" << newWidth << endl;
		cout << "width * height = " << width * height << endl;
		int scaleVal = width * height * 0.003;
//			Mat transform = Mat::zeros(300, 800, CV_8UC3);
		//see more on:
		//http://stackoverflow.com/questions/7995547/enlarge-and-restrict-a-quadrilateral-polygon-in-opencv-2-3-with-c
			//(The computation is componentwise for the x and y coordinates respectively).
//			In the shrunk polygon the vertex p is moved to p' along the line which halves the angle a between the vectors v1 and v2.
//			The vector w in this direction is
			for (int z = 0; z < 4; z++) {
				w[z] = v[z][1] + v[z][2];

	//			and the unit vector v in this direction is
	//			v0 = w0 / |w0|

				w_norm[z].x = w[z].x / norm(w[z]);
				w_norm[z].y = w[z].y / norm(w[z]);

	//			The new point p' is
//				int k = 15;
				cornerPoints[i][z] -= scaleVal * w_norm[z];
	//			p_x' = p_x + k * v_x
	//			p_y' = p_y + k * v_y
			}


		vector<Point2f> quad_pts;
		quad_pts.push_back(Point2f(0, 0));
		quad_pts.push_back(Point2f(0, transform.rows));
		quad_pts.push_back(Point2f(transform.cols, transform.rows));
		quad_pts.push_back(Point2f(transform.cols, 0));

//		cout << "transform.rows" << transform.rows << endl;
//		cout << "transform.cols" << transform.cols << endl;

	//	for (int i = 0; i < cornerPoints.size(); i++) {
			Mat transmtx = getPerspectiveTransform(cornerPoints[i], quad_pts);
			warpPerspective(src, transform, transmtx, transform.size());
			Mat src_bw;
			cvtColor(transform, transform, CV_BGR2GRAY);
//			cvtColor(transform, src_bw, CV_BGR2GRAY);
			transform.copyTo(src_bw);
//			equalizeHist(src_bw, src_bw);
//			blur(src_bw, src_bw, Size(3, 3));
//			adaptiveThreshold(src_bw, src_bw, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 0);

			//blur is not useful
			int blurFac = newWidth / 200;
			if (blurFac == 0) {
			blurFac = 2;
			}
			blur(src_bw, src_bw, Size(blurFac, blurFac)); //def: 5, 5
			adaptiveThreshold(src_bw, src_bw, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 0);
			~transform += .6 * ~src_bw;
//			transform = ~transform;
			cvtColor(src_bw, src_bw, CV_GRAY2BGR);
			cvtColor(transform, transform, CV_GRAY2BGR);
	//		blur(src_bw, src_bw, Size(3, 3));


	//		Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 8));
	//		morphologyEx(b, b, MORPH_CLOSE, kernel);

	//		dilate(src_bw, src_bw, kernel);
			/*dilate(src_bw, src_bw, kernel);
			erode(src_bw, src_bw, kernel);
			erode(src_bw, src_bw, kernel);
			erode(src_bw, src_bw, kernel);
			erode(src_bw, src_bw, kernel);*/
	//		cout << "0:" << int('0') << endl;
	//		cout << "strTransform" << endl;
			//char(i+48) -> conversion to char

			imshow(strTransform + char(i+48), src_bw); //+ "." + char(u+48)).c_str()
			imshow(strTransform + char(i+48) + ".2", transform);
			mBarcode.push_back(src_bw);
			mBarcode.push_back(transform);


	}
		//add the source image to the list
		Mat src_enhance, src_bw;
		src.copyTo(src_enhance);
		cvtColor(src_enhance, src_enhance, CV_BGR2GRAY);
		blur(src_enhance, src_bw, Size(5, 5));
		adaptiveThreshold(src_bw, src_bw, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 0);
		~src_enhance += .6 * ~src_bw;
		cvtColor(src_bw, src_bw, CV_GRAY2BGR);
		cvtColor(src_enhance, src_enhance, CV_GRAY2BGR);

//		mBarcode.push_back(src_enhance);
		mBarcode.push_back(src_bw);
		mBarcode.push_back(src);
	return mBarcode;

}


bool speak_article_descr(const string& article, const string& descr) {

	// -p: Pitch default: 50
	// -s: words per Minut: def: 175 (80 - 450)
	// -g: Word gap. Pause between words: def 10ms of def speed
	// -m: Enables SSML for adding a pause/break time

	string speak = string("espeak.exe -v de -p 30 -s 135 -g 4 -m \"") + string("artikel: ")
			+ article + string("<break time = \'1000\'/> beschreibung: ") + descr + string("\"");
	waitKey(100);
	system(speak.c_str());
}
